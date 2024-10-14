#ifndef BLOCKCHAIN_SERVICE_IMPL_H
#define BLOCKCHAIN_SERVICE_IMPL_H

#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>
#include "blockchain_rpc.pb.h"

using namespace muduo;
using namespace muduo::net;

class BlockchainServiceImpl {
public:
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime);

private:
    void GetBlock(const core::GetBlockRequest& request, core::GetBlockResponse* response);
    void GetBlockchainInfo(core::GetBlockchainInfoResponse *response);
    void GetBlockCount(core::GetBlockCountResponse* response);
    void GetRawTransaction(const core::GetRawTransactionRequest& request, core::GetRawTransactionResponse* response);
    void SendRawTransaction(const core::SendRawTransactionRequest& request, core::SendRawTransactionResponse* response);

    void sendResponse(const TcpConnectionPtr& conn, const core::GetBlockResponse& response);
    void sendResponse(const TcpConnectionPtr& conn, const core::GetBlockchainInfoResponse& response);
    void sendResponse(const TcpConnectionPtr& conn, const core::GetBlockCountResponse& response);
    void sendResponse(const TcpConnectionPtr& conn, const core::GetRawTransactionResponse& response);
    void sendResponse(const TcpConnectionPtr& conn, const core::SendRawTransactionResponse& response);
};

#endif // BLOCKCHAIN_SERVICE_IMPL_H
