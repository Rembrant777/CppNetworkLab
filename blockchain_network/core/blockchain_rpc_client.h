// blockchain_client.h
#ifndef BLOCKCHAIN_CLIENT_H
#define BLOCKCHAIN_CLIENT_H

#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/base/Logging.h>
#include "blockchain_rpc.pb.h"

using namespace muduo; 
using namespace muduo::net; 

class BlockchainClient {
public: 
    BlockchainClient(EventLoop* loop, const InetAddress& serverAddr); 
    void connect();
    void sendGetBlockRequest(int32_t blockHeight);
    void sendGetBlockchainInfoRequest();
    void sendGetBlockCountRequest();
    void sendGetRawTransactionRequest(const std::string& txid);
    void setSendRawTransactionRequest(const std::string& rawTransaction);
    void close() {}

private: 
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime);
    void sendRequest(core::MessageType messageType, const google::protobuf::Message& request);

    TcpClient client;
};

#endif // BLOCKCHAIN_CLIENT_H