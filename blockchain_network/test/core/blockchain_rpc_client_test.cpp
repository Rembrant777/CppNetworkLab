#include <gtest/gtest.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/TcpConnection.h>
#include <map>
#include <string>
#include <memory>
#include "blockchain_rpc.pb.h" 
#include "blockchain_rpc_client.h"

using namespace muduo; 
using namespace muduo::net; 

class MockTcpServer {
public:
    MockTcpServer(EventLoop* loop, const InetAddress& addr) 
        : server(loop, addr, "MockServer") {
        server.setMessageCallback(
            [&](const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTimestamp) {
                onMessage(conn, buf, receiveTimestamp); 
            }
        ); 
        // here we start the mock server 
        server.start(); 
    }

    void setResponseForMessageType(core::MessageType messageType, const std::string& response) {
        responses[messageType] = response; // Store expected response for the message type
    }

    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime) {
        while (buf->readableBytes() > 0) {
            if (buf->readableBytes() < 2) {
                break; 
            }

            uint16_t type; 
            memcpy(&type, buf->peek(), sizeof(type)); 
            core::MessageType messageType = static_cast<core::MessageType>(ntohs(type)); 
            buf->retrieve(sizeof(type)); 

            if (responses.count(messageType)) {
                sendResponse(conn, responses[messageType]); 
            } else {
                LOG_ERROR<<"Unknown message type received!" << messageType; 
            }

            // here clear buffer after handle
            buf->retrieveAll(); 
        }
    }

    void sendResponse(const TcpConnectionPtr& conn,  const std::string& responseData) {
        conn->send(responseData); 
    }

private:
    // instance of muduo tcp server 
    TcpServer server; 
    std::map<core::MessageType, std::string> responses; 
}; 

class BlockchainClientTest : public ::testing::Test {
protected:
    EventLoop loop;
    InetAddress serverAddr; 
    MockTcpServer mockServer;  
    BlockchainClient client; 

    void SetUp() override {
        serverAddr = InetAddress("127.0.0.1", 8888);  
        mockServer = MockTcpServer(&loop, serverAddr);  
        client = BlockchainClient(&loop, serverAddr); 

        // Start the mock server in a separate thread or non-blocking manner
        loop.runAfter(0, [&]() { client.connect(); });
    }

    void TearDown() override {
        client.close(); 
    }
}; 

TEST_F(BlockchainClientTest, TestGetBlockRequest) {
    // set up the expected response in the mock server 
    core::GetBlockResponse mockResponse; 
    mockResponse.set_block_data("Mock Block Data for Height 1"); 
    mockServer.setResponseForMessageType(core::GET_BLOCK, mockResponse); 

    // send a request to the mock server 
    client.sendGetBlockRequest(1); 

    // run the event loop to process the request and response 
    loop.loop(); 
}

TEST_F(BlockchainClientTest, TestGetBlockchainInfoRequest) {
    core::GetBlockchainInfoResponse mockResponse; 
    mockResponse.set_chain_id("chain_id_1"); 
    mockResponse.set_block_count(100); 
    mockServer.setResponseForMessageType(core::GET_BLOCKCHAIN_INFO, mockResponse); 

    client.sendGetBlockchainInfoRequest(); 
    loop.loop(); 
}

TEST_F(BlockchainClientTest, TestGetBlockCountRequest) {
    core::GetBlockCountResponse mockResponse; 
    mockResponse.set_block_count(100); 
    mockServer.setResponseForMessageType(core::GET_BLOCK_COUNT, mockResponse); 

    client.sendGetBlockCountRequest(); 
    loop.loop(); 
}

TEST_F(BlockchainClientTest, TestGetRawTransactionRequest) {
    core::GetRawTransactionResponse mockResponse; 
    mockResponse.set_raw_transaction("Mock Raw Transaction Data"); 
    mockServer.setResponseForMessageType(core::GET_RAW_TRANSACTION, mockResponse); 

    client.sendGetRawTransactionRequest("txid_1"); 
    loop.loop(); 
}

TEST_F(BlockchainClientTest, TestSendRawTransactionRequest) {
    core::SendRawTransactionResponse mockResponse; 
    mockResponse.set_success(true); 
    mockServer.setResponseForMessageType(core::SEND_RAW_TRANSACTION, mockResponse); 

    client.setSendRawTransactionRequest("Mock Raw Transaction"); 
    loop.loop(); 
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}