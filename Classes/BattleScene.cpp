/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2021 @aismann; Peter Eismann, Germany; dreifrankensoft

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


#include "BattleScene.hpp"
#include "extensions/cocos-ext.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "flatbuffers/flatbuffers.h"
#include "IntroScene.hpp"
#include "MainScene.h"
#include <thread>

USING_NS_AX;
USING_NS_AX_EXT;

#define PTM_RATIO 64

inline simVec2 vec2ToVec2(const game::Vec2& vec2) {
    return simVec2(vec2.x(), vec2.y());
}

bool Battle::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    auto visibleSize = _director->getVisibleSize();
    auto dispatcher = Director::getInstance()->getEventDispatcher();

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = AX_CALLBACK_2(Battle::onKeyPressed, this);
    listener->onKeyReleased = AX_CALLBACK_2(Battle::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    mBlackScreen = DrawNode::create();
    Vec2 rectangle[4];
    rectangle[0] = Vec2(0, 0);
    rectangle[1] = Vec2(visibleSize.x, 0);
    rectangle[2] = Vec2(visibleSize.x, visibleSize.y);
    rectangle[3] = Vec2(0, visibleSize.y);
    
    Color4F black(0, 0, 0, 1);
    mBlackScreen->drawPolygon(rectangle, 4, black, 1, black);
    mBlackScreen->setOpacity(130);
    addChild(mBlackScreen, 999);
    
    mCountdownText = TextFieldTTF::createWithTTF("", "fonts/arial.ttf", 24);
    mCountdownText->setAnchorPoint(Vec2(0.5, 0.5));
    mCountdownText->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2 + 100));
    
    mCountdownText->setVisible(false);
    
    addChild(mCountdownText, 1000);
    
    mPingText = TextFieldTTF::createWithTTF("0ms", "fonts/arial.ttf", 24);
    mPingText->setAnchorPoint(Vec2(0.5, 1));
    mPingText->setPosition(Vec2(visibleSize.x/2, visibleSize.y-10));
    
    addChild(mPingText, 500);
    
    scheduleUpdate();
    return true;
}

std::string intToString(int value) {
    std::stringstream sstream;
    sstream<<value;
    
    return sstream.str();
}

void Battle::refreshPing(int ping) {
    mCurrentPing = ping;
    std::stringstream sstream;
    sstream<<ping<<" ms";
    
    mPingText->setString(sstream.str());
}

void Battle::resetToWaitingStatus(std::string_view myNickname, const std::vector<UserInfo>& users, const game::RoomSetting* roomSetting) {
    auto visibleSize = _director->getVisibleSize();
    
    mUsers = users;
    mRoomSetting = *roomSetting;
    mMyNickname = myNickname;
    for (auto user : users) {
        std::stringstream sstream;
        
        if (user.nickname == myNickname) {
            myId = user.id;
            sstream<<user.nickname<<"(You) : " << (int)roomSetting->initial_player_hp();
            auto playerHpLabel = TextFieldTTF::createWithTTF(sstream.str(), "fonts/arial.ttf", 20);
            playerHpLabel->setPosition(Vec2(10, visibleSize.height-10));
            playerHpLabel->setAnchorPoint(Vec2(0.0, 1));
            mLabelMap[user.id] = playerHpLabel;
            addChild(playerHpLabel, 500);
        } else {
            sstream<<user.nickname<<" : " << (int)roomSetting->initial_player_hp();
            auto playerHpLabel = TextFieldTTF::createWithTTF(sstream.str(), "fonts/arial.ttf", 20);
            playerHpLabel->setPosition(Vec2(visibleSize.x-10, visibleSize.height-10));
            playerHpLabel->setAnchorPoint(Vec2(1, 1));
            mLabelMap[user.id] = playerHpLabel;
            addChild(playerHpLabel, 500);
        }
        mNicknameMap[user.id] = user.nickname;

    }
    
    mSimulator = new Simulator(0, this, mRoomSetting.world_gravity(), {
        EntityType::Map | EntityType::Player,
        EntityType::Map | EntityType::Rockfall,
        EntityType::Map | EntityType::PlayerGunShot,
    });
    
    uint32 flags = 0;
    flags += 1 * b2Draw::e_shapeBit;
    flags += 1 * b2Draw::e_jointBit;
    flags += 0 * b2Draw::e_aabbBit;
    flags += 0 * b2Draw::e_centerOfMassBit;

    g_debugDraw.SetFlags(flags);
    g_debugDraw.mRatio          = PTM_RATIO;
    g_debugDraw.debugNodeOffset = {0, 0};
    mSimulator->getWorld()->SetDebugDraw(&g_debugDraw);
    
    drawBox2D = g_debugDraw.GetDrawNode();
    addChild(drawBox2D, 100);
    drawBox2D->setOpacity(100);
    
    mGameStatus = game::GameStatusCode_Wait;
}

void Battle::countdown() {
    auto visibleSize = _director->getVisibleSize();
    
    mCountdownText->setVisible(true);
    
    mCountdownThreadHandle = std::thread([=](){
        int startSecond = this->mRoomSetting.starting_countdown_second();
        
        for (int count = startSecond; count > 0 && mSimulator != nullptr; count--) {
            this->_director->getScheduler()->performFunctionInCocosThread([=](){
                mCountdownText->setString(intToString(count));
            });
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    mGameStatus = game::GameStatusCode_Countdown;
}

void Battle::start() {
    mBlackScreen->setVisible(false);
    if (mCountdownText != nullptr) {
        mCountdownText->setVisible(false);
    }
    mGameStatus = game::GameStatusCode_Start;
}

void Battle::endGame(int winnerId) {
    mGameStatus = game::GameStatusCode_End;
    mBlackScreen->setVisible(true);
    unscheduleUpdate();
    
    auto visibleSize = _director->getVisibleSize();
    
    auto alertText = TextFieldTTF::createWithTTF(winnerId == myId ? "Win!!!" : "Lose...", "fonts/arial.ttf", 35);
    alertText->setAnchorPoint(Vec2(0.5, 0.5));
    alertText->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2 + 100));
    
    addChild(alertText, 2000);
    
    auto sendMenu = MenuItemFont::create("Main", AX_CALLBACK_1(Battle::closeResultMenu, this));
    sendMenu->setAnchorPoint(Vec2(0.5, 0.5));
    sendMenu->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2 - 50));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(sendMenu, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 3000);
}

void Battle::spawnEntity(const game::SpawnEntityPush* spawnEntity) {
    auto status = spawnEntity->entity_status();
    log("spawn entity %d", spawnEntity->entity_type());
    switch (spawnEntity->entity_type()) {
        case game::EntityType_Player: {
            
            auto player = mSimulator->spawnPlayer(simVec2(status->position().x(), status->position().y()), mRoomSetting.initial_player_hp(), mRoomSetting.player_ground_movement_force(), status->id());
            if (myId == status->id()) {
                mCurrentPlayer = player;
                mPlayerLabel = TextFieldTTF::createWithTTF("You", "fonts/arial.ttf", 20);
                mPlayerLabel->setAnchorPoint(Vec2(0.5, 0.5));
                addChild(mPlayerLabel, 500);
            } else {
                mOpponentPlayer = player;
            }
            break;
        }
        case game::EntityType_Rockfall:
            mSimulator->spawnRockfall(simVec2(status->position().x(), status->position().y()), mRoomSetting.rockfalll_damage(), status->id());
            break;
        case game::EntityType_FieldItem:
            mSimulator->spawnHpItem(simVec2(status->position().x(), status->position().y()), mRoomSetting.hp_item_value(), status->id());
            break;
    }
}
void Battle::triggerCollisionEvent(const game::CollisionEventPush* collisionEvent) {
    static auto setEntity = [](Battle* battle, IEntity* entity, const game::EntityStatus* status) {
        if (entity == nullptr) {
            return;
        }
        entity->setPosition(simVec2(status->position().x(), status->position().y()));
        entity->setLinearVelocity(simVec2(status->linear_velocity().x(), status->linear_velocity().y()));
        
        if (entity->getEntityType() == EntityType::Player) {
            Player* player = (Player*)entity;
            auto playerHpLabel = battle->mLabelMap[player->getEntityId()];
            
            std::stringstream sstream;
            auto me = player->getEntityId() == battle->mCurrentPlayer->getEntityId() ? "(You) : " : " : ";
            sstream<<battle->mNicknameMap[player->getEntityId()]<<me<< (int)(status->hp() < 0 ? 0 : status->hp());
            
            playerHpLabel->setString(sstream.str());
        }
    };
    
    auto entityAStatus = collisionEvent->entity_a_status();
    auto entityBStatus = collisionEvent->entity_b_status();
    
    if (collisionEvent->collision_code() == (EntityType::Map | EntityType::Player)) {
        return;
    }
    
    auto entityA = mSimulator->getEntity(entityAStatus->id());
    auto entityB = mSimulator->getEntity(entityBStatus->id());
    
    
    setEntity(this, entityA, entityAStatus);
    setEntity(this, entityB, entityBStatus);
    
    if (entityA != nullptr && entityB != nullptr) {
        log("trigger collision event");
        mSimulator->triggerCollisionEvent(entityA, entityB);
    }
}

inline float toDistance(const game::Vec2& p1, const simVec2& p2) {
    return std::sqrt(std::pow(p1.x() - p2.x, 2) + std::pow(p1.y() - p2.y, 2));
}

void Battle::changeEntityStatus(const game::ChangeEntityStatusPush* entityStatus) {
    auto status = entityStatus->status();
    auto entity = mSimulator->getEntity(status->id());
    
    log("%d, %d, %f, %f", status->id(), status->hp(), status->position().x(), status->position().y());

/*
    if (status->id() == mCurrentPlayer->getEntityId()) {
        if (toDistance(status->position(), mCurrentPlayer->getPosition()) > 0.3) {
            entity->setPosition(vec2ToVec2(status->position()));
        }
        return;
    }
    */
    entity->setPosition(vec2ToVec2(status->position()));
    entity->setLinearVelocity(vec2ToVec2(status->linear_velocity()));

    if (entity->getEntityType() == EntityType::Player) {
        auto player = reinterpret_cast<Player*>(entity);
        
        for (auto action : *entityStatus->action()) {
            switch (action) {
                case game::EntityAction_Left:
                    player->setMovementStatus(PlayerMovementStatus_Left);
                    break;
                case game::EntityAction_Right:
                    player->setMovementStatus(PlayerMovementStatus_Right);
                    break;
                case game::EntityAction_Jump:
                    player->jump(simVec2(player->getLinearVelocity().x, mRoomSetting.player_jump_velocity()));
                    break;
                case game::EntityAction_Stop:
                    player->setMovementStatus(PlayerMovementStatus_Stop);
                    break;
                case game::EntityAction_Shot:
                    if (status->id() == mCurrentPlayer->getEntityId()) {
                        mSimulator->spawnPlayerGunShot(mCurrentPlayer, mRoomSetting.gunshot_damage());
                    } else {
                        mSimulator->spawnPlayerGunShot(mOpponentPlayer, mRoomSetting.gunshot_damage());
                    }
            }

        }
    }
    

    
}

void Battle::changePlayerStatus(const game::ChangePlayerStatusPush* playerStatus) {
    
}

void Battle::setEventCallback(std::function<void(int)> playerActionCallback, std::function<void()> closeResultMenuCallback) {
    mPlayerActionCallback = playerActionCallback;
    mCloseResultMenuCallback = closeResultMenuCallback;
}



void Battle::update(float dt)
{
    mSimulator->simulate(dt);
    if (mPlayerLabel != nullptr) {
        simVec2 position = mCurrentPlayer->getPosition();
        mPlayerLabel->setPosition(Vec2(position.x*PTM_RATIO, position.y*PTM_RATIO));
    }
    drawBox2D->clear();
    
    mSimulator->getWorld()->DebugDraw();
}

void Battle::closeResultMenu(Ref* sender) {
    mCloseResultMenuCallback();
}

void Battle::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (mCurrentPlayer == nullptr) {
        return;
    }
    
    if (mGameStatus != game::GameStatusCode_Start) {
        return;
    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        
        //mCurrentPlayer->jump(simVec2(mCurrentPlayer->getLinearVelocity().x, mRoomSetting.player_jump_velocity()));
        mPlayerActionCallback(game::EntityAction_Jump);
        
    } else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW && !mKeyDownStatus[1]) {
        mCurrentMovementStatus = PlayerMovementStatus_Right;
        mKeyDownStatus[1] = true;
        //mCurrentPlayer->setMovementStatus(PlayerMovementStatus_Right);
        mPlayerActionCallback(game::EntityAction_Right);
        
    } else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW && !mKeyDownStatus[0]) {
        mCurrentMovementStatus = PlayerMovementStatus_Left;
        mKeyDownStatus[0] = true;
        //mCurrentPlayer->setMovementStatus(PlayerMovementStatus_Left);
        mPlayerActionCallback(game::EntityAction_Left);
        
    } else if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        auto now = std::chrono::system_clock::now();

        if (std::chrono::duration_cast<std::chrono::seconds>(now - mLastShotTime).count() >= mRoomSetting.player_shot_cooltime()) {
            // mSimulator->spawnPlayerGunShot(mCurrentPlayer, mRoomSetting.gunshot_damage());
            mPlayerActionCallback(game::EntityAction_Shot);
            mLastShotTime = now;
        }
    }
}

void Battle::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (mCurrentPlayer == nullptr) {
        return;
    }
    
    if (mGameStatus != game::GameStatusCode_Start) {
        return;
    }
    auto movementStatus = mCurrentPlayer->getMovementStatus();
    
    if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
        mKeyDownStatus[1] = false;
    }
    
    if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
        mKeyDownStatus[0] = false;
    }
    
    if ((keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) && (!mKeyDownStatus[0] && !mKeyDownStatus[1])) {

        // mCurrentPlayer->setMovementStatus(PlayerMovementStatus_Stop);
        mPlayerActionCallback(PlayerMovementStatus_Stop);
    }
}

void Battle::onPlayerContactMap(Player*, IMap*) {}
void Battle::onPlayerHitGunshot(Player*, GunShot*) {}
void Battle::onRockfallHitGunshot(Rockfall*, GunShot*) {}
void Battle::onHitRockfall(Player* player, Rockfall*) {}
void Battle::onPickupHp(Player*, HpItem*) {}

Battle::~Battle() {
    
    AX_SAFE_DELETE(mSimulator);
    if (mCountdownThreadHandle.joinable()) {
        mCountdownThreadHandle.join();
    }
    
}
