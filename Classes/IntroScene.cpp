
#include "IntroScene.hpp"
#include "MainScene.h"
#include <sstream>
#include <thread>
#include <random>
USING_NS_AX;

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in front of filenames in "
        "HelloWorldScene.cpp\n");
}


// on "init" you need to initialize your instance
bool Intro::init()
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
    
    mBlackScreen = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(visibleSize.x, 0);
    rectangle[2] = Vec2(visibleSize.x, visibleSize.y);
    rectangle[3] = Vec2(0, visibleSize.y);

    Color4F black(0, 0, 0, 1);
    mBlackScreen->drawPolygon(rectangle, 4, black, 1, black);
    
    mConnectingMessage =Label::createWithTTF("Connecting...", "fonts/arial.ttf", 24);
    mConnectingMessage->setAnchorPoint(Vec2(0.5, 0.5));
    mConnectingMessage->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2));
    
    addChild(mBlackScreen, 998);
    addChild(mConnectingMessage, 999);
    
    auto label = Label::createWithTTF("your nickname is", "fonts/arial.ttf", 24);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2 + 200));
    
    std::random_device rd;
    
    std::stringstream sstream;
    
    sstream<<rd()% 1000000;
    
    mTextField = TextFieldTTF::createWithTTF(sstream.str(), "fonts/arial.ttf", 24);
    mTextField->setAnchorPoint(Vec2(0.5, 0.5));
    mTextField->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2 + 100));
    
    auto sendMenu = MenuItemFont::create("Join", AX_CALLBACK_1(Intro::sendCallback, this));
    sendMenu->setAnchorPoint(Vec2(0.5, 0.5));
    sendMenu->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(sendMenu, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    this->addChild(label);
    this->addChild(mTextField);
    return true;
}

void Intro::resetToConnectingStatus() {
    mBlackScreen->setVisible(true);
    mConnectingMessage->setVisible(true);

}

void Intro::setConnectedStatus() {
    mBlackScreen->setVisible(false);
    mConnectingMessage->setVisible(false);
    std::random_device rd;
    
    std::stringstream sstream;
    
    sstream<<rd()% 1000000;
    mTextField->setString(sstream.str());
}

void Intro::setSendEventCallback(std::function<void(std::string_view)> callback) {
    mSendCallback = callback;
}

void Intro::sendCallback(Ref* sender) {
    mSendCallback(mTextField->getString());
}
