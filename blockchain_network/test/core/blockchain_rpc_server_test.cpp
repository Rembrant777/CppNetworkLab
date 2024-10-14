#include <gtest/gtest.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
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

// class BlockchainServerTest : public ::testing::Test {}