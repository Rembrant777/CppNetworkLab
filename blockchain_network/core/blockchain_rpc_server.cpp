#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include "blockchain_rpc_server.h"  
#include "blockchain_rpc.pb.h"

using namespace muduo; 
using namespace muduo::net; 

void BlockchainServiceImpl::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime) {
    while (buf->readableBytes() > 0) {
        if (buf->readableBytes() < 2) {
            break; 
        }

        uint16_t type; 
        memcpy(&type, buf->peek(), sizeof(type)); 
        // Convert to host endian (little endian)
        core::MessageType messageType = static_cast<core::MessageType>(ntohs(type)); 

        // Move pointer to the start offset of message content 
        buf->retrieve(sizeof(type)); 

        switch (messageType) {
            case core::GET_BLOCK: {
                // Handle GET_BLOCK request
                core::GetBlockRequest request;
                request.ParseFromArray(buf->peek(), buf->readableBytes()); // Assume request data is available
                buf->retrieve(buf->readableBytes()); // Remove data from buffer
                core::GetBlockResponse response;
                GetBlock(request, &response);
                sendResponse(conn, response);
                break;
            }
            case core::GET_BLOCKCHAIN_INFO: {
                // Handle GET_BLOCKCHAIN_INFO request
                core::GetBlockchainInfoResponse response;
                GetBlockchainInfo(&response);
                sendResponse(conn, response);
                break;
            }
            case core::GET_BLOCK_COUNT: {
                // Handle GET_BLOCK_COUNT request
                core::GetBlockCountResponse response;
                GetBlockCount(&response);
                sendResponse(conn, response);
                break;
            }
            case core::GET_RAW_TRANSACTION: {
                // Handle GET_RAW_TRANSACTION request
                core::GetRawTransactionRequest request;
                request.ParseFromArray(buf->peek(), buf->readableBytes()); // Assume request data is available
                buf->retrieve(buf->readableBytes()); // Remove data from buffer
                core::GetRawTransactionResponse response;
                GetRawTransaction(request, &response);
                sendResponse(conn, response);
                break;
            }
            case core::SEND_RAW_TRANSACTION: {
                // Handle SEND_RAW_TRANSACTION request
                core::SendRawTransactionRequest request;
                request.ParseFromArray(buf->peek(), buf->readableBytes()); // Assume request data is available
                buf->retrieve(buf->readableBytes()); // Remove data from buffer
                core::SendRawTransactionResponse response;
                SendRawTransaction(request, &response);
                sendResponse(conn, response);
                break;
            }
            default: 
                LOG_ERROR << "Unknown message type received"; 
                break; 
        } 

        // Remove message content from the buffer 
        buf->retrieveAll(); 
    }
}

void BlockchainServiceImpl::GetBlock(const core::GetBlockRequest& request, core::GetBlockResponse* response) {
    // Retrieve block data 
    int32_t block_height = request.block_height(); 
    response->set_block_data("Block data for height: " + std::to_string(block_height));         
}

void BlockchainServiceImpl::GetBlockchainInfo(core::GetBlockchainInfoResponse *response) {
    // Get blockchain metadata info 
    response->set_chain_id("chain_id_1"); 
    response->set_block_count(100); 
}

void BlockchainServiceImpl::GetBlockCount(core::GetBlockCountResponse* response) {
    // Return current block count 
    response->set_block_count(100); 
}

void BlockchainServiceImpl::GetRawTransaction(const core::GetRawTransactionRequest& request, core::GetRawTransactionResponse* response) {
    // Get raw transaction by id 
    response->set_raw_transaction("Raw transaction data for txid: " + request.txid()); 
}

void BlockchainServiceImpl::SendRawTransaction(const core::SendRawTransactionRequest& request, core::SendRawTransactionResponse* response) {
    // Send raw transaction 
    bool success = true; 
    response->set_success(success); 
}

void BlockchainServiceImpl::sendResponse(const TcpConnectionPtr& conn, const core::GetBlockResponse& response) {
    std::string responseData; 
    response.SerializeToString(&responseData);
    conn->send(responseData);  
}

void BlockchainServiceImpl::sendResponse(const TcpConnectionPtr& conn, const core::GetBlockchainInfoResponse& response) {
    std::string responseData; 
    response.SerializeToString(&responseData); 
    conn->send(responseData); 
}

void BlockchainServiceImpl::sendResponse(const TcpConnectionPtr& conn, const core::GetBlockCountResponse& response) {
    std::string responseData; 
    response.SerializeToString(&responseData);
    conn->send(responseData);  
}

void BlockchainServiceImpl::sendResponse(const TcpConnectionPtr& conn, const core::GetRawTransactionResponse& response) {
    std::string responseData;
    response.SerializeToString(&responseData);
    conn->send(responseData);
}

void BlockchainServiceImpl::sendResponse(const TcpConnectionPtr& conn, const core::SendRawTransactionResponse& response) {
    std::string responseData;
    response.SerializeToString(&responseData);
    conn->send(responseData);
}