#pragma once

#include "Lvgl.hpp"
#include <unordered_map>
#include <memory>
#include <my_config.hpp>


class BasePage {
// protected:
//     LvObject * main_screen;
public:
    // ~BasePage();        /* ----------------- 这里需要用 virtual ?? */
    virtual void show() = 0;
    virtual void hide() = 0;

};

class PageManager {
public:
    // 定义强类型枚举
    enum class PageType {
        MAINPAGE,
        FACEPAGE
    };
    // PageType currentpage_ = PageType::MAINPAGE;
    BasePage* current_page_{nullptr};
    static PageManager instance;
    // 这里 BasePage 基类，是不能实例化的，
    // 所以不能储存对象BasePage，而应该是 BasePage*
    // 使用智能指针自动管理内存，防止内存泄漏
    std::unordered_map<PageType, std::unique_ptr<BasePage>> pages_map;
    static PageManager& getInstance() {
        static PageManager instance;
        return instance;
    }

    void init(Camera & camera);

    void switchToPage(PageType pagetype);
    
    // 获取页面指针（不转移所有权）
    BasePage* getPage(PageType pagetype) const;
    
    // 添加页面（可用于动态添加）
    void addPage(PageType pagetype, BasePage* page);
    
    // 移除页面（自动清理内存）
    void removePage(PageType pagetype);


private:
    PageManager() = default;
    ~PageManager() = default;

    // 禁用 拷贝和赋值操作
    PageManager(const PageManager&) = delete;
    PageManager& operator=(const PageManager&) = delete;

    PageType pagetype_;
};






