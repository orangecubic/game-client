//
//  Client.hpp
//  ALL_BUILD
//
//  Created by 이동현 on 2023/01/31.
//

#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <atomic>
#include <memory>
#include <functional>
#include "flatbuffers/flatbuffers.h"
#include "protocol/protocol_generated.h"
#include "protocol/game_packet_generated.h"
#include "BlockingQueue.h"

class Client {
private:
    int mFd;
    std::atomic_bool mRun = false;
    BlockingQueue<iovec> mSendQueue;
    
public:
    
    class Callback {
    public:
        virtual void onConnect() = 0;
        virtual void onPacket(const game::Packet* packet) = 0;
        virtual void onDisconnect() = 0;
        
    };
    Client(Callback* callback);
    
    void start(std::string_view ip, unsigned short port);
    
    void stop();
    
    void sendPacket(iovec vec);
    
private:
    Callback* mCallback;
    
};

#endif /* Client_hpp */
