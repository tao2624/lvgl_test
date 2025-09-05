
#include "Lvgl.hpp"
#include "UI.hpp"
// #include "lv_color.h"
#include "PageManager.hpp"
#include "my_config.hpp"


extern "C"
{
    LV_IMAGE_DECLARE(bg);
    LV_IMAGE_DECLARE(face_icon);
    LV_IMAGE_DECLARE(security_camera_icon);
}

// MainPage::MainPage() : LvObject(NULL) , main_screen(this) {
MainPage::MainPage() : BasePage() , main_screen(new LvObject(NULL)) {
// MainPage::MainPage(FaceDetectPage * faceDetect) : BasePage() , main_screen(new LvObject(NULL)), faceDetectPage(faceDetect) {
    // Constructor code
    // main_screen = new LvObject(NULL);
    // Create the main page UI
    main_screen->set_style_bg_image_src(&bg, 0)
        // .set_style_text_font(Font24::get_font(), 0)
        .set_flex_flow(LV_FLEX_FLOW_ROW)
        .set_flex_align(LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // 不需要设置 main_screen 的尺寸，它会自动适配显示屏尺寸
    // main_screen->set_size(1200, 400); // 这行无效，删除
    /*
    LvButton btn{main_screen->raw(), "main_btn"};
    btn.set_size(LV_SIZE_CONTENT, LV_SIZE_CONTENT)
       .set_pos(300, 200)
       .set_align(LV_ALIGN_CENTER)
       .add_flag(LV_OBJ_FLAG_CLICKABLE);

    btn.add_event_cb([&](lv_event_t * e, void * user_data) {
        main_screen->set_style_bg_color(lv_color_hex(0xFF0000), LV_PART_MAIN);
        LV_LOG_USER("Button clicked");
    }, LV_EVENT_CLICKED, nullptr);
    */

    LvObject face_container(main_screen->raw());
    face_container.set_size(LV_SIZE_CONTENT, LV_SIZE_CONTENT)
        .set_flex_flow(LV_FLEX_FLOW_COLUMN)
        .set_flex_align(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER)
        .set_align(LV_ALIGN_CENTER)
        .add_event_cb(
            [&](lv_event_t * e, void * obj) {
                LV_LOG_USER("Face container clicked!\t");
                // if (faceDetectPage) {
                //     faceDetectPage->show();
                // }
                PageManager::getInstance().switchToPage(PageManager::PageType::FACEPAGE);
                
            },
            LV_EVENT_CLICKED,
            nullptr
            );

    LvImage face_container_image{face_container.raw()};
    face_container_image.set_src(&face_icon);
}

void MainPage::show() {
    lv_screen_load(main_screen->raw());
}

void MainPage::hide() {
    // lv_obj_add_flag(main_screen->raw(), LV_OBJ_FLAG_HIDDEN);
}

