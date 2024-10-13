#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include "blockchain_rpc.pb.h"

using namespace muduo; 
using namespace muduo::net; 

class BlockchainServiceImpl {
public:
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime) {
        while (buf->readableBytes() > 0) {
            if (buf->readableBytes() < 2) {
                break; 
            }

            uint16_t type; 
            memcpy(&type, buf->peek(), sizeof(type)); 
            // convert to host endian(little endian)
            core::MessageType messageType = static_cast<core::MessageType>(ntohs(type)); 

            // move pointer to the start offset of message content 
            buf->retrieve(sizeof(type)); 

            switch (messageType) {
                case core::GET_BLOCK: {}
                case core::GET_BLOCKCHAIN_INFO: {}
                case core::GET_BLOCK_COUNT: {}
                case core::GET_RAW_TRANSACTION: {}
                case core::SEND_RAW_TRANSACTION: {}
                default: 
                    LOG_ERROR << "Unknown message type received"; 
                    break; 
            } 

            // remove message content from the buffer 
            buf->retrieveAll(); 
        }
    }     
private:
    void GetBlock(const core::GetBlockRequest& request, core::GetBlockResponse* response) {
        // retrieve block data 
        int32_t block_height = request.block_height(); 
        response->set_block_data("Block data for height: " + std::to_string(block_height));         
    }

    void GetBlockchainInfo(core::GetBlockchainInfoResponse *response) {
        // get blockchain metadata info 
        response->set_chain_id("chain_id_1"); 
        response->set_block_count(100); 
    }

    void GetBlockCount(core::GetBlockCountResponse* response) {
        // return current block count 
        response->set_block_count(100); 
    }

    void GetRawTransaction(const core::GetRawTransactionRequest& request, core::GetRawTransactionResponse* response) {
        // get raw transaction by id 
        response->set_raw_transaction("Raw transaction data for txid: " + request.txid()); 
    }

    void SendRawTransaction(const core::SendRawTransactionRequest& request, core::SendRawTransactionResponse* response) {
        // send raw transaction 
        bool success = true; 
        response->set_success(success); 
    }

    void sendResponse(const TcpConnectionPtr& conn, const core::GetBlockResponse& response) {
        std::string responseData; 
        response.SerializeToString(&responseData);
        conn->send(responseData);  
    }

    void sendResponse(const TcpConnectionPtr& conn, const core::GetBlockchainInfoResponse& response) {
        std::string responseData; 
        response.SerializeToString(&responseData); 
        conn->send(responseData); 
    }

    void sendResponse(const TcpConnectionPtr& conn, const core::GetBlockCountResponse& response) {
        std::string responseData; 
        response.SerializeToString(&responseData);
        conn->send(responseData);  
    }

    void sendResponse(const TcpConnectionPtr& conn, const core::GetRawTransactionResponse& response) {
        std::string responseData;
        response.SerializeToString(&responseData);
        conn->send(responseData);
    }

    void sendResponse(const TcpConnectionPtr& conn, const core::SendRawTransactionResponse& response) {
        std::string responseData;
        response.SerializeToString(&responseData);
        conn->send(responseData);
    }
}; 

int main(int argc, char* argv[]) {
    EventLoop loop; 
    InetAddress listenAddr(8888); 
    TcpServer server(&loop, listenAddr, "BlockchainServer"); 

    BlockchainServiceImpl service; 
    server.setMessageCallback(
        [&](const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime) {
            service.onMessage(conn, buf, receiveTime); 
        }); 

    server.start(); 
    loop.loop(); 
}