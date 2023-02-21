//
//  Client.cpp
//  ALL_BUILD
//
//  Created by 이동현 on 2023/01/31.
//

#include "Client.hpp"
#include <axmol.h>
#include <thread>
#include <unistd.h>

constexpr unsigned char PROTOCOL_VERSION[] = {0, 1};
constexpr unsigned short PROTOCOL_VERSION_SCALAR = (PROTOCOL_VERSION[1] * 256) + PROTOCOL_VERSION[0];

#define RECV(retVar, recvSizeVar, buf, size) {  \
    retVar = recv(mFd, buf, size, 0);                          \
    ax::log("recv data from server %ld", retVar); \
    if (retVar < 0) { \
        director->getScheduler()->performFunctionInCocosThread([=](){ \
            mCallback->onDisconnect();  \
        }); \
        close(mFd); \
        mRun = false; \
        return; \
    }                                                          \
    if (retVar == 0) {                                         \
        director->getScheduler()->performFunctionInCocosThread([=](){ \
            mCallback->onDisconnect();  \
        });\
        close(mFd); \
        mRun = false; \
        return; \
    }                                                          \
    recvSizeVar += retVar;                                      \
}

class ThHandle {
public:
    std::thread& handle;
    BlockingQueue<iovec>& sendQueue;
    ThHandle(std::thread& handle, BlockingQueue<iovec>& sendQueue) : handle(handle), sendQueue(sendQueue) {}
    ~ThHandle() {
        sendQueue.Push({nullptr, 0});
        handle.join();
        ax::log("finish th");
    }
};

Client::Client(Callback* callback) {
    mCallback = callback;
}
void Client::start(std::string_view ip, unsigned short port) {
    
    auto director = ax::Director::getInstance();
    
    sockaddr_in addr;
    socklen_t size;
    int recv_length;
    char* buffer = new char[2048];
    
    mFd=socket(PF_INET, SOCK_STREAM, 0);
    
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr = inet_addr(std::string(ip).c_str());
    memset(&(addr.sin_zero),0,8);
    
    int result = connect(mFd, (sockaddr*)&addr, sizeof(struct sockaddr));
    if (result < 0) {
        ax::log("%d", errno);
        director->getScheduler()->performFunctionInCocosThread([=](){
            this->mCallback->onDisconnect();
        });
        return;
    }
    
    director->getScheduler()->performFunctionInCocosThread([=](){
        this->mCallback->onConnect();
    });
    
    mRun = true;
    auto thHandle = std::thread([=](){
        char buffer[2048];
        while(this->mRun) {
            auto vec = this->mSendQueue.Pop();
            if (vec.iov_base == nullptr) {
                return;
            }
            
            unsigned short packetLength = vec.iov_len;
            
            memcpy(buffer, &packetLength, 2);
            memcpy(buffer+2, vec.iov_base, vec.iov_len);
            
            send(this->mFd, buffer, vec.iov_len + 2, 0);
            delete[] static_cast<char*>(vec.iov_base);
        }
    });
    auto handle = ThHandle(thHandle, mSendQueue);
    
    ssize_t retVal = 0;
    ssize_t recvSizeVar = 0;
    while(mRun) {
        while (recvSizeVar < 2)
            RECV(retVal, recvSizeVar, buffer, 2048);
        
        unsigned short length = 0;
        memcpy(&length, buffer, 2);
        ax::log("recv packet from server length %d", length);
        
        while (recvSizeVar - 2 < length)
            RECV(retVal, recvSizeVar, buffer, 2048);
        
        auto packet = game::GetPacket(buffer + 2);
        
        
        flatbuffers::Verifier verifier((const uint8_t*)(buffer+2), length);
        if (!game::VerifyPacketBuffer(verifier)) {
            ax::log("invalid packet");
            close(mFd);
            director->getScheduler()->performFunctionInCocosThread([=](){
                this->mCallback->onDisconnect();
            });
            mRun = false;
            return;
        } else if (packet->version() != PROTOCOL_VERSION_SCALAR) {
            director->getScheduler()->performFunctionInCocosThread([=](){
                this->mCallback->onDisconnect();
            });
            mRun = false;
            return;
        } else {
            auto newBuffer = new char[2048];
            memcpy(newBuffer, buffer+2, length);
            director->getScheduler()->performFunctionInCocosThread([=](){
                this->mCallback->onPacket(game::GetPacket(newBuffer));
                delete[] newBuffer;
            });
        }
        
        ssize_t leastDataSize = recvSizeVar - (length + 2);
        if (leastDataSize > 0) {
            memcpy(buffer, buffer + length + 2, leastDataSize);
            recvSizeVar = leastDataSize;
        } else {
            recvSizeVar = 0;
        }
    }
}

void Client::sendPacket(iovec vec) {
    char* newBuffer = new char[vec.iov_len];
    memcpy(newBuffer, vec.iov_base, vec.iov_len);
    vec.iov_base = newBuffer;
    
    mSendQueue.Push({newBuffer, vec.iov_len});
}
