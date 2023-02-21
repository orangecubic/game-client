//
//  BattleScene.hpp
//  ALL_BUILD
//
//  Created by 이동현 on 2023/01/27.
//

#ifndef BattleScene_hpp
#define BattleScene_hpp

#include "axmol.h"
#include "box2d/box2d.h"
#include "extensions/cocos-ext.h"
#include "simulator/Simulator.h"
#include "protocol/game_packet_generated.h"
#include <unordered_map>
#include <functional>
#include <string>
#include <thread>
#include <chrono>

struct UserInfo {
    std::string nickname;
    int id;
};

class Battle : public ax::Scene, public Simulator::EventCallback
{
public:

    virtual bool init() override;

    void update(float dt) override;
    
    void resetToWaitingStatus(std::string_view myNickname, const std::vector<UserInfo>& users, const game::RoomSetting* roomSetting);
    
    void countdown();
    void start();
    
    void spawnEntity(const game::SpawnEntityPush* entity);
    void triggerCollisionEvent(const game::CollisionEventPush* collisionEvent);
    void changeEntityStatus(const game::ChangeEntityStatusPush* entityStatus);
    
    void changePlayerStatus(const game::ChangePlayerStatusPush* playerStatus);
    
    void endGame(int winnerId);
    
    void setEventCallback(std::function<void(int)> playerActionCallback, std::function<void()> closeResultMenuCallback);
    
    void refreshPing(int ping);
    
    virtual ~Battle();
    
    virtual void onPlayerContactMap(Player*, IMap*) override;
    virtual void onPlayerHitGunshot(Player*, GunShot*) override;
    virtual void onRockfallHitGunshot(Rockfall*, GunShot*) override;
    virtual void onHitRockfall(Player*, Rockfall*) override;
    virtual void onPickupHp(Player*, HpItem*) override;
    
    
private:
    int mCurrentPing = 0;
    int myId;
    std::string mMyNickname;
    Simulator* mSimulator;

    ax::DrawNode* drawBox2D;
    ax::extension::PhysicsDebugNodeBox2D g_debugDraw;
    
    ax::DrawNode* mBlackScreen;
    ax::Label* mCountdownText;
    ax::Label* mPingText;
    ax::Label* mPlayerLabel = nullptr;
    
    std::unordered_map<int, ax::Label*> mLabelMap;
    std::unordered_map<int, std::string> mNicknameMap;
    bool mKeyDownStatus[3] = {false, false, false};
    int mCurrentMovementStatus = PlayerMovementStatus_Stop;
    Player* mCurrentPlayer = nullptr;
    Player* mOpponentPlayer = nullptr;
    
    game::GameStatusCode mGameStatus;
    game::RoomSetting mRoomSetting;
    std::vector<UserInfo> mUsers;
    
    std::function<void(int)> mPlayerActionCallback;
    std::function<void()> mCloseResultMenuCallback;
    
    std::thread mCountdownThreadHandle;
    
    std::chrono::system_clock::time_point mLastShotTime;
    
    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    void closeResultMenu(Ref* sender);
};
#endif /* BattleScene_hpp */
