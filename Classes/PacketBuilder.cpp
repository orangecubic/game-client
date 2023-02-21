#include "PacketBuilder.hpp"
#include "protocol/game_packet_generated.h"
#include "protocol/protocol_generated.h"
#include <algorithm>
#include <vector>

void PacketBuilder::addConnectReqMessage(const std::string& nickname) {
    auto offset = game::CreateConnectReqDirect(mBuilder, nickname.c_str());

    mPayloadOffsetList.push_back(offset.Union());
    mPayloadTypeList.push_back(game::Payload_ConnectReq);
}

void PacketBuilder::addMathReqMessage(bool cancel) {
    auto offset = game::CreateMatchReq(mBuilder, cancel);

    mPayloadOffsetList.push_back(offset.Union());
    mPayloadTypeList.push_back(game::Payload_MatchReq);
}

void PacketBuilder::addBattleReadyReqMessage() {
    auto offset = game::CreateBattleReadyReq(mBuilder);

    mPayloadOffsetList.push_back(offset.Union());
    mPayloadTypeList.push_back(game::Payload_BattleReadyReq);
}

void PacketBuilder::addChangePlayerStatusReqMessage(const std::vector<uint8_t>& actions) {
    auto offset = game::CreateChangePlayerStatusReqDirect(mBuilder, &actions);

    mPayloadOffsetList.push_back(offset.Union());
    mPayloadTypeList.push_back(game::Payload_ChangePlayerStatusReq);
}

void PacketBuilder::addPingAckMessage() {
    auto offset = game::CreatePingAck(mBuilder);

    mPayloadOffsetList.push_back(offset.Union());
    mPayloadTypeList.push_back(game::Payload_PingAck);
}

iovec PacketBuilder::complete() {
    auto packet = game::CreatePacket(mBuilder, 256, mBuilder.CreateVector(mPayloadTypeList), mBuilder.CreateVector(mPayloadOffsetList));
    mBuilder.Finish(packet);
    
    return {mBuilder.GetBufferPointer(), mBuilder.GetSize()};
}

void PacketBuilder::reset() {
    mBuilder.Reset();
    mPayloadTypeList.clear();
    mPayloadOffsetList.clear();
}

bool PacketBuilder::remained() {
    return !mPayloadTypeList.empty();
}
