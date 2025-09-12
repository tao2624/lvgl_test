#include "Camera.hpp"
#include <chrono>


Camera::Camera(const std::string& device) {
    capture_.open(device);
    capture_.set(cv::CAP_PROP_FPS, 30);
    capture_.set(cv::CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    capture_.set(cv::CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);

}


void Camera::start() {
    // if(is_capturing_) {
    //     return;  // 已经启动，直接返回
    // }
    
    is_capturing_ = true;
    capture_thread_ = std::thread(&Camera::captureLoop, this);
    return;
}

/* 生产者 */
void Camera::captureLoop() {
    while(is_capturing_) {
        // cv::Mat temp_frame;
        // if(capture_.read(temp_frame) && !temp_frame.empty()) {
            std::unique_lock<std::mutex> lock(frame_mutex);
            /* 入队前等待空 */
            condition_.wait(lock,[this](){ return frame_queue_.empty(); });
            // frame = temp_frame.clone();
            std::shared_ptr<cv::Mat> frame_ptr = std::make_shared<cv::Mat>();
            capture_.read(*frame_ptr);   
            frame_queue_.push(frame_ptr);   /* 入队 */
            /* 入队后通知非空 */
            condition_.notify_one();

        // }
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30fps
    }
}


void Camera::stop() {
    is_capturing_ = false;
    capture_thread_.join();
    return;
}


std::shared_ptr<cv::Mat> Camera::getFrame() {
    std::unique_lock<std::mutex> lock(frame_mutex);
    /* 出队前等待非空 */
    condition_.wait(lock, [this](){ return !frame_queue_.empty(); });
    std::shared_ptr<cv::Mat> frame = frame_queue_.front();
    frame_queue_.pop();
    /* 出队后通知 */
    condition_.notify_one();
    return frame;   // move()好一点
}

