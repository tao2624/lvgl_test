#include "UI.hpp"
#include "Lvgl.hpp"
#include "PageManager.hpp"
#include "my_config.hpp"


extern "C"
{
    LV_IMAGE_DECLARE(bg);
    LV_IMAGE_DECLARE(face_icon);
    LV_IMAGE_DECLARE(security_camera_icon);
}


// FaceDetectPage::FaceDetectPage() : LvObject(nullptr), main_screen(this) {
FaceDetectPage::FaceDetectPage() : BasePage(), main_screen(new LvObject(NULL)) {

    main_screen->set_style_bg_image_src(&bg, 0);
        // .set_flex_flow(LV_FLEX_FLOW_ROW)
        // .set_flex_align(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

        LvButton btn_back = LvButton(main_screen->raw(), "Back");
        btn_back.set_pos(20, 10)
            .set_size(100, 30)
            .add_event_cb(
                [&](lv_event_t * e, void * user_data){
                    LV_LOG_USER("FacePage Back button clicked");
                    PageManager::getInstance().switchToPage(PageManager::PageType::MAINPAGE);
                },
                LV_EVENT_CLICKED,
                nullptr
            );

}


void FaceDetectPage::show() {
    lv_screen_load(main_screen->raw());
}


void FaceDetectPage::hide() {
    // lv_obj_add_flag(main_screen->raw(), LV_OBJ_FLAG_HIDDEN);
}



