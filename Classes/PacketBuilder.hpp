#pragma once

#include "flatbuffers/flatbuffers.h"
#include <sys/uio.h>
#include <vector>

struct Packet_User {
    std::string_view nickname;
    int id;
};

class PacketBuilder {
private:
    flatbuffers::FlatBufferBuilder mBuilder;
    std::vector<uint8_t> mPayloadTypeList;
    std::vector<flatbuffers::Offset<void>> mPayloadOffsetList;

public:

    void addConnectReqMessage(const std::string& nickname);
    void addMathReqMessage(bool cancel);
    
    void addBattleReadyReqMessage();
    
    void addChangePlayerStatusReqMessage(const std::vector<uint8_t>& actions);
    void addPingAckMessage();
  
    bool remained();
    iovec complete();
    
    void reset();
    
};
