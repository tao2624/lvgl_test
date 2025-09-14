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

SecurityPage::SecurityPage(Camera& camera) : camera_(camera) {

}





