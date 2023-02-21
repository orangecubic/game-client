//
//  Engine.hpp
//  miners
//
//  Created by 이동현 on 2023/02/10.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <axmol.h>
#include "Client.hpp"
#include "PacketBuilder.hpp"
#include "IntroScene.hpp"
#include "MainScene.h"
#include "BattleScene.hpp"


class Engine : public Client::Callback{
private:
    Client* mClient;
    PacketBuilder mPacketBuilder;
    
    Intro* mIntroScene;
    Main* mMainScene;
    Battle* mBattleScene;
    
    std::string mNickname;
    
    void recycleIntroScene();
    void recycleMainScene();
    void recycleBattleScene();
public:
    
    void start(std::string_view ip, unsigned short port);
    
    virtual void onConnect() override;
    virtual void onPacket(const game::Packet* packet) override;
    virtual void onDisconnect() override;
};
#endif /* Engine_hpp */
