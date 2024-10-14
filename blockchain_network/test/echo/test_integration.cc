#include <gtest/gtest.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/TcpClient.h>
#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

// Server functionality test
class EchoServerTest : public ::testing::Test {
protected:
    EventLoop loop;
    TcpServer* server;

    void SetUp() override {
        InetAddress listenAddr(8888);
        server = new TcpServer(&loop, listenAddr, "EchoServer");
        server->setConnectionCallback([](const TcpConnectionPtr& conn) {
            if (conn->connected()) {
                LOG_INFO << "Client connected: " << conn->peerAddress().toIpPort();
            }
        });
        server->setMessageCallback([](const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {
            string msg = buf->retrieveAllAsString();
            LOG_INFO << "Received: " << msg;
            conn->send(msg);  // Echo the message back
        });
        server->start();
    }

    void TearDown() override {
        delete server;
    }
};

// Client functionality test
class EchoClientTest : public ::testing::Test {
protected:
    EventLoop loop;
    TcpClient* client;

    void SetUp() override {
        InetAddress serverAddr("127.0.0.1", 8888);
        client = new TcpClient(&loop, serverAddr, "EchoClient");

        client->setConnectionCallback([](const TcpConnectionPtr& conn) {
            if (conn->connected()) {
                LOG_INFO << "Connected to server";
                conn->send("Hello from Client!\n");
            }
        });

        client->setMessageCallback([](const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {
            string msg = buf->retrieveAllAsString();
            LOG_INFO << "Received from server: " << msg;
        });

        client->connect();
    }

    void TearDown() override {
        delete client;
    }
};

// Integration tests
TEST(EchoServerIntegrationTest, BasicFunctionality) {
    // This test could check the basic echo functionality
    // You may want to implement actual checks or assertions
    ASSERT_TRUE(true);  // Placeholder assertion
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
