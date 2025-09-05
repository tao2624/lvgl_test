#include "PageManager.hpp"
#include "UI.hpp"
#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

void PageManager::init() {
    // 使用 make_unique 创建智能指针，更安全
    pages_map[PageType::MAINPAGE] = std::make_unique<MainPage>();
    pages_map[PageType::FACEPAGE] = std::make_unique<FaceDetectPage>();
}

void PageManager::switchToPage(PageType pagetype) {
    // auto it = pages_map.find(pagetype);
    // if (it != pages_map.end() && it->second) {
    //     it->second->show();
    // }
    if (current_page_) {
        current_page_->hide();
    }
    current_page_ = pages_map[pagetype].get();
    if (current_page_) {
        current_page_->show();
    }
}

BasePage* PageManager::getPage(PageType pagetype) const {
    auto it = pages_map.find(pagetype);
    return (it != pages_map.end()) ? it->second.get() : nullptr;
}

// void PageManager::addPage(PageType pagetype, std::unique_ptr<BasePage> page) {
//     pages_map[pagetype] = std::move(page);
// }

// void PageManager::removePage(PageType pagetype) {
//     pages_map.erase(pagetype);  // unique_ptr 会自动清理内存
// }



