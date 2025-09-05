#include "lvgl/lvgl.h"
#include "testPage.hpp"
// #include "Lvgl.hpp"
// #include "UI.hpp"
#include <stdio.h>

void test_gui_create(void){
    lv_obj_t * scr = lv_screen_active(); // 获取当前活动屏幕
    // 创建一个按钮
    lv_obj_t * btn = lv_button_create(scr);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    // 创建一个标签
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Hello LVGL!");
    lv_obj_center(label);
}

void base_example_1(void)
{
    // 创建一个新屏幕
    lv_obj_t * my_screen = lv_obj_create(NULL);

    // 在新屏幕上添加控件
    lv_obj_t * btn = lv_button_create(my_screen);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    // 创建一个标签
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Hello LVGL!!");
    // lv_obj_center(label);

    // 切换到新屏幕
    lv_screen_load(my_screen);
}

void base_example_2()
{
    static lv_style_t style;
    lv_style_init(&style);
    /* Flex布局设置 */
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);  //ROW: 主轴方向为水平（从左到右） WRAP: 当一行放不下时，自动换行
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);   // 在主轴上均匀分布，包括首尾
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);    // 启用Flex布局系统

    lv_obj_t * src = lv_scr_act();
    lv_obj_set_size(src, 500, 220); // 这样无法设置激活屏幕大小
    lv_obj_t * cont = lv_obj_create(src);
    lv_obj_set_size(cont, 500, 220);
    lv_obj_center(cont);
    lv_obj_add_style(cont, &style, 0);

    uint32_t i;
    for(i = 0; i < 8; i++) {
        lv_obj_t * obj = lv_obj_create(cont);
        lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);

        lv_obj_t * label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "%"LV_PRIu32, i);
        lv_obj_center(label);
    }
}

/* 尝试添加事件 */
lv_obj_t * my_screen;
lv_obj_t * second_show;
static void event_btn(lv_event_t * e) {
    printf("btn has been press\n");
    const char* data = lv_obj_get_user_data(lv_event_get_target(e));
    printf("userdata = %s\n", data);
    lv_screen_load(second_show);
}
static void event_btn2(lv_event_t * e) {
    printf("btn2 has been press\n");
    lv_screen_load(my_screen);
}
void base_example_3(void)
{
    // 创建一个新屏幕
    my_screen = lv_obj_create(NULL);
    second_show = lv_obj_create(NULL);

    // lv_obj_set_size(my_screen, 800, 480);
    // lv_obj_set_size(second_show, 800, 480);

    // 设置背景
    lv_obj_set_style_bg_color(my_screen, lv_color_hex(0x349800), 0);
    lv_obj_set_style_bg_color(second_show, lv_color_hex(0xe74c3c), 0);

    // 在主屏上创建一个按钮
    lv_obj_t * btn = lv_button_create(my_screen);
    lv_obj_set_user_data(btn, "button1");
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    // 在副屏创建一个按钮
    lv_obj_t * btn2 = lv_button_create(second_show);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(btn2, 100, 100);

    // add 事件回调
    // lv_obj_add_event_cb(btn, event_btn, LV_EVENT_ALL, NULL); 使用这个ALL会导致事件被无限触发
    lv_obj_add_event_cb(btn, event_btn, LV_EVENT_CLICKED, NULL); 
    // 返回事件
    lv_obj_add_event_cb(btn2,event_btn2, LV_EVENT_CLICKED, NULL);

    // 创建一个标签
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Hello LVGL!!");
    // lv_obj_center(label);

    // 切换到新屏幕
    lv_screen_load(my_screen);
}

void lv_example_style_1(void)
{
     static lv_style_t style;
     lv_style_init(&style);
     lv_style_set_radius(&style, 5);

     /*Make a gradient*/
     lv_style_set_width(&style, 150);
     lv_style_set_height(&style, LV_SIZE_CONTENT);

     lv_style_set_pad_ver(&style, 20);
     lv_style_set_pad_left(&style, 5);

     lv_style_set_x(&style, lv_pct(50));
     lv_style_set_y(&style, 80);

     /*Create an object with the new style*/
    //  lv_obj_t * obj = lv_obj_create(lv_scr_act());  // lv_scr_act = lv_screen_active
     lv_obj_t * obj = lv_scr_act();             // 不可行
    // lv_obj_t * obj = lv_obj_create(NULL);   // 可行，但需要加load，且无法使用style
     lv_obj_add_style(obj, &style, 0);

     lv_obj_t * label = lv_label_create(obj);
     lv_label_set_text(label, "Hello");
     lv_screen_load(obj);
}



/* -------------------------------- cpp test --------------------------------------- */

// void basic_example_1(void)
// {
//     // 创建一个新屏幕
//     MainPage * main_screen = new MainPage();
//     main_screen->show();
// }



