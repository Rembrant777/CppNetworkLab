#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include "blockchain_client.h"
#include "blockchain_rpc.pb.h"

using namespace muduo; 
using namespace muduo::net; 

BlockchainClient::BlockchainClient(EventLoop* loop, const InetAddress& serverAddr) 
    : client(loop, serverAddr, "BlockchainClient") {
    client.setConnectionCallback(
        [&](const TcpConnectionPtr& conn) {
            if (conn->connected()) {
                LOG_INFO << "Connected to server"; 
            }
        }
    );

    client.setMessageCallback(
        [&](const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime) {
            core::GetBlockResponse response; 
            if (response.ParseFromArray(buf->peek(), buf->readableBytes())) {
                LOG_INFO << "Received: " << response.block_data(); 
            }
        }
    );  
}

void BlockchainClient::connect() {
    client.connect(); 
}

void BlockchainClient::sendGetBlockRequest(int32_t blockHeight) {
    core::GetBlockRequest request; 
    request.set_block_height(blockHeight); 

    // set message type into GET_BLOCK 
    sendRequest(core::GET_BLOCK, request); 
}

void BlockchainClient::sendGetBlockchainInfoRequest() {
    core::GetBlockchainInfoRequest request; 

    // set message type into GET_BLOCKCHAIN_INFO 
    sendRequest(core::GET_BLOCKCHAIN_INFO, request); 
}

void BlockchainClient::sendGetBlockCountRequest() {
    core::GetBlockCountRequest request; 

    // set message type into GET_BLOCK_COUNT 
    sendRequest(core::GET_BLOCK_COUNT, request); 
}

void BlockchainClient::sendGetRawTransactionRequest(const std::string& txid) {
    core::GetRawTransactionRequest request; 
    request.set_txid(txid); 

    // set message type info GET_RAW_TRANSACTION 
    sendRequest(core::GET_RAW_TRANSACTION, request); 
}

void BlockchainClient::setSendRawTransactionRequest(const std::string& rawTransaction) {
    core::SendRawTransactionRequest request; 
    request.set_raw_transaction(rawTransaction); 

    // modify message type into SEND_RAW_TRANSACTION 
    sendRequest(core::SEND_RAW_TRANSACTION, request); 
}

void BlockchainClient::onConnection(const TcpConnectionPtr& conn) {
    if (conn->connected()) {
        LOG_INFO << "Connected to server"; 
    } else {
        LOG_INFO << "Disconnected from server"; 
    }
}

void BlockchainClient::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime) {
    // server side response handle 
    std::string responseData(buf->retrieveAllAsString());
    core::GetBlockResponse response;
    if (response.ParseFromString(responseData)) {
        LOG_INFO << "Received GetBlockResponse: " << response.block_data();
    } else {
        LOG_ERROR << "Failed to parse GetBlockResponse";
    }
}

void BlockchainClient::sendRequest(core::MessageType messageType, const google::protobuf::Message& request) {
    std::string serializedRequest;
    request.SerializeToString(&serializedRequest);

    // add message type to the message header 
    uint16_t type = htons(static_cast<uint16_t>(messageType));
    std::string messageData(reinterpret_cast<const char*>(&type), sizeof(type));
    messageData += serializedRequest;

    client.connection()->send(messageData);
}