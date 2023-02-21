/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2021 Bytedance Inc.

 https://axmolengine.github.io/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"
#include "BattleScene.hpp"
#include "IntroScene.hpp"
#include <thread>
USING_NS_AX;

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in front of filenames in "
        "HelloWorldScene.cpp\n");
}


// on "init" you need to initialize your instance
bool Main::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    matchingItem = MenuItemFont::create("Matching", AX_CALLBACK_1(Main::matchingCallback, this));
    
    matchingItem->setAnchorPoint(Vec2(0.5, 0.5));
    matchingItem->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2));
    
    auto cancelLabel = Label::createWithTTF("Cancel", "fonts/arial.ttf", 24);
    
    cancelItem = MenuItemLabel::create(cancelLabel, AX_CALLBACK_1(Main::cancelCallback, this));
    
    cancelItem->setAnchorPoint(Vec2(0.5, 0.5));
    cancelItem->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2));
    cancelItem->setVisible(false);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(matchingItem, cancelItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
}

void Main::cancelCallback(Ref* sender) {
    mCancelEventCallback();
}

void Main::matchingCallback(Ref* sender) {
    mMatchEventCallback();
}

void Main::resetToConnectedStatus(std::string_view nickname) {
    mNickname = nickname;
}

void Main::matchRequest() {
    matchingItem->setVisible(false);
    cancelItem->setVisible(true);
}

void Main::cancelMatching() {
    matchingItem->setVisible(true);
    matchingItem->setEnabled(false);
    std::thread([=](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        this->_director->getScheduler()->performFunctionInCocosThread([=](){
            this->matchingItem->setEnabled(true);
        });
        
    }).detach();
    cancelItem->setVisible(false);
}

void Main::setTouchEventCallback(std::function<void()> matchCallback, std::function<void()> cancelCallback) {
    mMatchEventCallback = matchCallback;
    mCancelEventCallback = cancelCallback;
}
