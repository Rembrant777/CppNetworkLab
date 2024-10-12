#include <gtest/gtest.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/TcpClient.h>
#include <muduo/base/Logging.h>
#include <atomic>
#include <thread>

using namespace muduo;
using namespace muduo::net;

// Performance test
TEST(EchoServerPerformanceTest, MultiThreadedClient) {
    const int clientCount = 10;
    const int messageCount = 100;

    std::atomic<int> receivedMessages{0};
    std::thread clients[clientCount];

    auto clientFunction = [&]() {
        EventLoop loop;
        TcpClient client(&loop, InetAddress("127.0.0.1", 8888), "TestClient");
        
        client.setConnectionCallback([&](const TcpConnectionPtr& conn) {
            for (int i = 0; i < messageCount; ++i) {
                conn->send("Test Message");
            }
        });

        client.setMessageCallback([&](const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {
            string msg = buf->retrieveAllAsString();
            ++receivedMessages;  // Increment count of received messages
        });

        client.connect();
        loop.loop();
    };

    // Start multiple client threads
    for (int i = 0; i < clientCount; ++i) {
        clients[i] = std::thread(clientFunction);
    }

    for (int i = 0; i < clientCount; ++i) {
        clients[i].join();  // Wait for all threads to finish
    }

    // Check that all messages were received
    EXPECT_EQ(receivedMessages.load(), clientCount * messageCount);
}

int main(int argc, char** argv) {
    muduo::Logging::setLogLevel(muduo::Logger::DEBUG);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
