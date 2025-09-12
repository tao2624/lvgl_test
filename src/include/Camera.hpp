#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>


#define CAMERA_WIDTH    1280
#define CAMERA_HEIGHT   720

class Camera {
public:
    Camera(const std::string& device="/dev/video1");
    ~Camera() = default;
    void start();
    void captureLoop();
    void stop();
    std::shared_ptr<cv::Mat> getFrame();



private:
    cv::VideoCapture capture_;
    cv::Mat frame;
    std::mutex frame_mutex;
    std::thread capture_thread_;
    std::atomic<bool> is_capturing_;
    std::queue<std::shared_ptr<cv::Mat>> frame_queue_;
    std::condition_variable condition_;

};





