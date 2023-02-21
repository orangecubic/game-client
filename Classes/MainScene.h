//
//  MainScene.h
//  miners
//
//  Created by 이동현 on 2023/01/27.
//

#ifndef MainScene_h
#define MainScene_h


#include "axmol.h"
#include "Client.hpp"
#include <functional>
#include <string>

class Main : public ax::Scene
{
private:
    std::string mNickname;
    ax::DrawNode* mBox;
    ax::Label* mLabel;
    
    ax::MenuItem* matchingItem;
    ax::MenuItem* cancelItem;
    
    std::function<void()> mMatchEventCallback;
    std::function<void()> mCancelEventCallback;
    
public:
    virtual bool init() override;

    void resetToConnectedStatus(std::string_view nickname);
    
    void cancelMatching();
    void matchRequest();
    
    void setTouchEventCallback(std::function<void()> matchCallback, std::function<void()> cancelCallback);
    void matchingCallback(Ref* sender);
    void cancelCallback(Ref* sender);
    
};

#endif /* MainScene_h */
