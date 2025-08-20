#pragma once

#include "Lvgl.hpp"


// class BasePage {
// protected:
//     LvObject * screen_obj;
// public:
//     BasePage() : screen_obj(nullptr) {}
// };

class MainPage : public LvObject {
    
private:
    LvObject * main_screen;

public:
    MainPage();

    void create();
};
