//
//  Engine.cpp
//  miners
//
//  Created by 이동현 on 2023/02/10.
//

#include "Engine.hpp"

#include <axmol.h>
#include <memory>
#include <chrono>

void Engine::start(std::string_view ip, unsigned short port) {
    mClient = new Client(this);
    
    auto director = ax::Director::getInstance();
    
    recycleIntroScene();
    mIntroScene->resetToConnectingStatus();
    
    std::thread([=](){
        while(true) {
            this->mClient->start(ip, port);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }).detach();
    
    director->runWithScene(mIntroScene);
}

void Engine::recycleIntroScene() {
    mIntroScene = ax::utils::createInstance<Intro>();
    mIntroScene->setSendEventCallback([=](std::string_view nickname) {
        this->mNickname = std::string(nickname);
        this->mPacketBuilder.addConnectReqMessage(std::string(nickname));
        this->mClient->sendPacket(mPacketBuilder.complete());
        this->mPacketBuilder.reset();
    });
}

void Engine::recycleMainScene() {
    mMainScene = ax::utils::createInstance<Main>();
    mMainScene->setTouchEventCallback([=]() {
        mMainScene->matchRequest();
        this->mPacketBuilder.addMathReqMessage(false);
        this->mClient->sendPacket(this->mPacketBuilder.complete());
        this->mPacketBuilder.reset();
    }, [=](){
        this->mPacketBuilder.addMathReqMessage(true);
        this->mClient->sendPacket(this->mPacketBuilder.complete());
        this->mPacketBuilder.reset();
    });
}

void Engine::recycleBattleScene() {
    mBattleScene = ax::utils::createInstance<Battle>();
    mBattleScene->setEventCallback([=](int action) {
        this->mPacketBuilder.addChangePlayerStatusReqMessage({(uint8_t)action});
        this->mClient->sendPacket(this->mPacketBuilder.complete());
        this->mPacketBuilder.reset();
    }, [=](){
        auto director = ax::Director::getInstance();
        recycleMainScene();
        this->mMainScene->resetToConnectedStatus(mNickname);
        director->replaceScene(this->mMainScene);
    });
}

void Engine::onConnect() {
    mIntroScene->setConnectedStatus();
}
void Engine::onPacket(const game::Packet* packet) {
    auto director = ax::Director::getInstance();
    
    int index = 0;
    for (auto type : *packet->messages_type()) {
        switch (type) {
            case game::Payload_ConnectRep: {
                auto connectRep = reinterpret_cast<const game::ConnectRep*>(packet->messages()->Get(index));
                if (connectRep->success()) {
                    recycleMainScene();
                    mMainScene->resetToConnectedStatus(mNickname);
                    director->replaceScene(mMainScene);
                } else {
                    mIntroScene->setConnectedStatus();
                }
                break;
            }
            case game::Payload_MatchRep: {
                auto matchRep = reinterpret_cast<const game::MatchRep*>(packet->messages()->Get(index));
                
                if (matchRep->code() == game::MatchResultCode_Ok) {
                    
                    
                    std::vector<UserInfo> userInfos;
                    for(auto user : *matchRep->users()) {
                        userInfos.push_back({
                            user->nickname()->str(),
                            user->id()
                        });
                    }
                    
                    recycleBattleScene();
                    mBattleScene->resetToWaitingStatus(mNickname, userInfos, matchRep->setting());
                    director->replaceScene(mBattleScene);
                    
                    mPacketBuilder.addBattleReadyReqMessage();
                    mClient->sendPacket(mPacketBuilder.complete());
                    mPacketBuilder.reset();
                } else {
                    ax::log("cancel matching");
                    mMainScene->cancelMatching();
                }
                break;
            }
            case game::Payload_PingPush: {
                auto pingPush = reinterpret_cast<const game::PingPush*>(packet->messages()->Get(index));
                mBattleScene->refreshPing(pingPush->delay());
                mPacketBuilder.addPingAckMessage();
                mClient->sendPacket(mPacketBuilder.complete());
                mPacketBuilder.reset();
                break;
            }
            case game::Payload_GameStatusPush: {
                auto gameStatus = reinterpret_cast<const game::GameStatusPush*>(packet->messages()->Get(index));
                switch (gameStatus->status_code()) {
                    case game::GameStatusCode_Countdown:
                        mBattleScene->countdown();
                        break;
                        
                    case game::GameStatusCode_Start:
                        mBattleScene->start();
                        break;
                        
                    case game::GameStatusCode_End:
                        mBattleScene->endGame(gameStatus->winner_id());
                }
                break;
            }
            case game::Payload_SpawnEntityPush:
                mBattleScene->spawnEntity(reinterpret_cast<const game::SpawnEntityPush*>(packet->messages()->Get(index)));
                break;
                
            case game::Payload_CollisionEventPush:
                mBattleScene->triggerCollisionEvent(reinterpret_cast<const game::CollisionEventPush*>(packet->messages()->Get(index)));
                break;
                
            case game::Payload_ChangeEntityStatusPush:
                mBattleScene->changeEntityStatus(reinterpret_cast<const game::ChangeEntityStatusPush*>(packet->messages()->Get(index)));
                break;
                
            case game::Payload_ChangePlayerStatusPush:
                mBattleScene->changePlayerStatus(reinterpret_cast<const game::ChangePlayerStatusPush*>(packet->messages()->Get(index)));
                break;
        }
        index++;
    }
}

void Engine::onDisconnect() {
    auto director = ax::Director::getInstance();
    
    if (director->getRunningScene() != mIntroScene) {
        recycleIntroScene();
        director->replaceScene(mIntroScene);
    }
    
    mIntroScene->resetToConnectingStatus();
}
