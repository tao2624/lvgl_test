#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/src/core/lv_global.h"
#include "testPage.hpp"
#include "UI.hpp"


#if LV_USE_WAYLAND
#include "backends/interface.h"
#endif

#if LV_USE_SDL
#include "lvgl/src/drivers/sdl/lv_sdl_window.h"
#include "lvgl/src/drivers/sdl/lv_sdl_mouse.h"
#include "lvgl/src/drivers/sdl/lv_sdl_keyboard.h"
#endif

uint16_t window_width;
uint16_t window_height;
bool fullscreen;
bool maximize;

static void configure_simulator(int argc, char **argv);

static const char *getenv_default(const char *name, const char *dflt)
{
    return getenv(name) ? : dflt;
}

#if LV_USE_EVDEV

#if LV_USE_X11
static void lv_linux_init_input_pointer(lv_display_t * disp)
{
    // X11后端通常自动处理鼠标输入，不需要手动指定设备文件
    // 系统会自动检测鼠标设备
    
    // 如果需要自定义鼠标指针图标
    LV_IMAGE_DECLARE(mouse_cursor_icon);
    lv_obj_t * cursor_obj = lv_image_create(lv_display_get_screen_active(disp));
    lv_image_set_src(cursor_obj, &mouse_cursor_icon);
    
    // 获取默认的输入设备（X11会自动创建）
    lv_indev_t * mouse = lv_indev_get_next(NULL);
    if(mouse && lv_indev_get_type(mouse) == LV_INDEV_TYPE_POINTER) {
        lv_indev_set_cursor(mouse, cursor_obj);
    }
}
#else
static void lv_linux_init_input_pointer(lv_display_t *disp)
{
    /* Enables a pointer (touchscreen/mouse) input device
     * Use 'evtest' to find the correct input device. /dev/input/by-id/ is recommended if possible
     * Use /dev/input/by-id/my-mouse-or-touchscreen or /dev/input/eventX
     * 
     * If LV_LINUX_EVDEV_POINTER_DEVICE is not set, automatic evdev disovery will start
     */
    const char *input_device = getenv("LV_LINUX_EVDEV_POINTER_DEVICE");
    if(input_device == NULL) {
        input_device = getenv_default("LV_LINUX_EVDEV_POINTER_DEVICE", "/dev/input/event6");
        // input_device = "/dev/input/event8";
    }


    lv_indev_t *touch = lv_evdev_create(LV_INDEV_TYPE_POINTER, input_device);
    lv_indev_set_display(touch, disp);

    /* Set the cursor icon */
    LV_IMAGE_DECLARE(mouse_cursor_icon);
    lv_obj_t * cursor_obj = lv_image_create(lv_display_get_screen_active(disp));
    lv_image_set_src(cursor_obj, &mouse_cursor_icon);
    lv_indev_set_cursor(touch, cursor_obj);
}
#endif
#endif

#if LV_USE_LINUX_FBDEV
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    lv_display_t * disp = lv_linux_fbdev_create();

#if LV_USE_EVDEV
    lv_linux_init_input_pointer(disp);
#endif

    lv_linux_fbdev_set_file(disp, device);
}
#elif LV_USE_LINUX_DRM
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_DRM_CARD", "/dev/dri/card0");
    lv_display_t * disp = lv_linux_drm_create();

#if LV_USE_EVDEV
    lv_linux_init_input_pointer(disp);
#endif

    lv_linux_drm_set_file(disp, device, -1);
}
#elif LV_USE_SDL
static void lv_linux_disp_init(void)
{
    lv_display_t * disp = lv_sdl_window_create(window_width, window_height);
    
    /* Create mouse input device for SDL2 */
    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_display(mouse, disp);
    
    /* Create keyboard input device for SDL2 */
    lv_indev_t * keyboard = lv_sdl_keyboard_create();
    lv_indev_set_display(keyboard, disp);
    lv_indev_set_group(keyboard, lv_group_get_default());
}
#elif LV_USE_X11
static void lv_linux_disp_init(void)
{
    lv_display_t * disp = lv_x11_window_create("LVGL Simulator", window_width, window_height);

#if LV_USE_EVDEV
    lv_linux_init_input_pointer(disp);
#endif
}
#elif LV_USE_WAYLAND
    // /* lv_linux_disp_init and lv_linux_run_loop are implemented in backends/wayland.c */
    // extern void lv_linux_disp_init(void);
    // extern void lv_linux_run_loop(void);
#else
#error Unsupported configuration
#endif

#if LV_USE_WAYLAND == 0
void lv_linux_run_loop(void)
{
    uint32_t idle_time;

    /*Handle LVGL tasks*/
    while(1) {
        idle_time = lv_timer_handler(); /*Returns the time to the next timer execution*/
        
        /* For SDL2, we need to ensure the event processing timer runs properly */
        if(idle_time > 5) idle_time = 5; /* SDL2 event handler runs every 5ms */
        
        usleep(idle_time * 1000);
    }
}
#else
    /* lv_linux_run_loop is implemented in backends/wayland.c when using Wayland */
#endif

/*
 * Process command line arguments and environment
 * variables to configure the simulator
 */
static void configure_simulator(int argc, char **argv)
{

    int opt = 0;
    bool err = false;

    /* Default values */
    fullscreen = maximize = false;
    window_width = atoi(getenv("LV_SIM_WINDOW_WIDTH") ? : "1200");
    window_height = atoi(getenv("LV_SIM_WINDOW_HEIGHT") ? : "800");

    /* Parse the command-line options. */
    while ((opt = getopt (argc, argv, "fmw:h:")) != -1) {
        switch (opt) {
        case 'f':
            fullscreen = true;
            if (LV_USE_WAYLAND == 0) {
                fprintf(stderr, "The SDL driver doesn't support fullscreen mode on start\n");
                exit(1);
            }
            break;
        case 'm':
            maximize = true;
            if (LV_USE_WAYLAND == 0) {
                fprintf(stderr, "The SDL driver doesn't support maximized mode on start\n");
                exit(1);
            }
            break;
        case 'w':
            window_width = atoi(optarg);
            break;
        case 'h':
            window_height = atoi(optarg);
            break;
        case ':':
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            exit(1);
        case '?':
            fprintf (stderr, "Unknown option -%c.\n", optopt);
            exit(1);
        }
    }
}

int main(int argc, char **argv)
{

    configure_simulator(argc, argv);

    /* Initialize LVGL. */
    lv_init();

    /* Initialize the configured backend SDL2, FBDEV, libDRM or wayland */
    lv_linux_disp_init();

    /*Create a Demo*/
    // base_example_2();
    // test_gui_create();
    // lv_example_style_1();
    // lv_demo_widgets();
    // lv_demo_widgets_start_slideshow();

    /* cpp try */
    // 创建一个新屏幕
    MainPage * main_screen = new MainPage();
    main_screen->create();

    lv_linux_run_loop();

    return 0;
}
