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

static LvImage * image_;
static LvImageDsc * image_dsc_ = new LvImageDsc();;
static LvTimer * timer_;


// FaceDetectPage::FaceDetectPage() : LvObject(nullptr), main_screen(this) {
FaceDetectPage::FaceDetectPage(Camera& camera_param) : BasePage(), camera_(camera_param), main_screen(new LvObject(NULL)) {

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

        image_ = new LvImage(main_screen->raw()); 
        image_->align(LV_ALIGN_CENTER, 0, -50);
        // image_->set_width(800).set_height(450);

        // 构造函数中 更新界面
        timer_ = new LvTimer([&](lv_timer_t * , void *){
            // 从Camera部分获取frame;
            std::shared_ptr<cv::Mat> mat_frame = camera_.getFrame();
            // 检查frame是否有效
            if(mat_frame->empty()) {
                LV_LOG_WARN("Frame is empty");
                return;
            }
            cv::resize(*mat_frame, *mat_frame, cv::Size(800, 450));
            time_t now = time(nullptr);
            char time_str_s[20];
            strftime(time_str_s, sizeof(time_str_s), "%Y-%m-%d %H:%M:%S", localtime(&now));
            cv::putText(*mat_frame, time_str_s, cv::Point(mat_frame->cols - 800, mat_frame->rows-50),
                        cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, cv::LINE_8);
            // 转换为RGB格式（OpenCV默认是BGR）
            // cv::Mat rgb_frame;
            // cv::cvtColor(mat_frame, rgb_frame, cv::COLOR_BGR2RGB);            

            // cv::resize(mat_frame, mat_frame, cv::Size(800, 450));  // 使用resize调整大小
            image_dsc_->raw()->header.cf = LV_COLOR_FORMAT_RGB888;
            image_dsc_->raw()->header.w = mat_frame->cols;   // cols rows 不要搞反
            image_dsc_->raw()->header.h = mat_frame->rows;   // 不然会乱码
            image_dsc_->raw()->data = mat_frame->data;
            image_dsc_->raw()->data_size = mat_frame->rows * mat_frame->cols * 3;
            image_->set_src(image_dsc_);            
        },
        33,
        nullptr);

        timer_->pause();



}


void FaceDetectPage::show() {
    camera_.start();  // 启动摄像头
    lv_screen_load(main_screen->raw());
    timer_->resume();

    /* 推理线程 */
    
    
}


void FaceDetectPage::hide() {
    // lv_obj_add_flag(main_screen->raw(), LV_OBJ_FLAG_HIDDEN);
    timer_->pause();
    camera_.stop();
}



