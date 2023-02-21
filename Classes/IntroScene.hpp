//
//  MainScene.h
//  miners
//
//  Created by 이동현 on 2023/01/27.
//

#ifndef IntroScene_h
#define IntroScene_h

#include "axmol.h"
#include "Client.hpp"
#include "PacketBuilder.hpp"
#include "MainScene.h"
#include <functional>
#include <string>

class Intro : public ax::Scene
{
private:
    ax::Label* mTextField;
    ax::Label* mLabel;
    ax::DrawNode* mBlackScreen;
    ax::Label* mConnectingMessage;
    std::function<void(std::string_view)> mSendCallback;
public:
    virtual bool init() override;
    
    void resetToConnectingStatus();
    void setConnectedStatus();
    void setSendEventCallback(std::function<void(std::string_view)> callback);
    void sendCallback(Ref* sender);
    
};

#endif /* MainScene_h */
