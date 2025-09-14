#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>


// #define CAMERA_WIDTH    1280
// #define CAMERA_HEIGHT   720
#define CAMERA_WIDTH 1920
#define CAMERA_HEIGHT 1080

class Camera {
public:
    static Camera & getInstance(std::string device) {
        static Camera instance(device);
        return instance;
    }
    void start();
    void captureLoop();
    void stop();
    std::shared_ptr<cv::Mat> getFrame();



private:
    Camera(const std::string& device);
    ~Camera() = default;
    cv::VideoCapture capture_;
    cv::Mat frame;
    std::mutex frame_mutex;
    std::thread capture_thread_;
    std::atomic<bool> is_capturing_;
    std::queue<std::shared_ptr<cv::Mat>> frame_queue_;
    std::condition_variable condition_;
    // 禁用拷贝、移动构造 和 赋值重载
    Camera(const Camera &) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(const Camera&) = delete;


};





