#pragma once

#include "Lvgl.hpp"
#include "PageManager.hpp"

// 前向声明
class FaceDetectPage;


// class MainPage : public LvObject 

class MainPage : public BasePage {
private:
    LvObject * main_screen;
    FaceDetectPage * faceDetectPage;

public:
    // MainPage(FaceDetectPage * faceDetect = nullptr);
    MainPage();

    void show() override;
    void hide() override;
};


/* class FaceDetectPage : public LvObject {
    private:
        LvObject * main_screen;

    public:
        FaceDetectPage();
        void show();

}; */

class FaceDetectPage : public BasePage {
private:
    LvObject * main_screen;
    Camera& camera_;

public:
    FaceDetectPage(Camera& camera_);

    void show() override;
    void hide() override;
};


