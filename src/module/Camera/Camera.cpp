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

void Camera::captureLoop() {
    while(is_capturing_) {
        cv::Mat temp_frame;
        if(capture_.read(temp_frame) && !temp_frame.empty()) {
            std::lock_guard<std::mutex> lock(frame_mutex);
            frame = temp_frame.clone();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30fps
    }
}


void Camera::stop() {
    is_capturing_ = false;
    capture_thread_.join();
    return;
}


cv::Mat Camera::getFrame() {
    std::lock_guard<std::mutex> lock(frame_mutex);
    return frame.clone();
}

