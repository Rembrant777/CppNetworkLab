#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

void onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        LOG_INFO << "Client connected: " << conn->peerAddress().toIpPort();
    }
    else
    {
        LOG_INFO << "Client disconnected: " << conn->peerAddress().toIpPort();
    }
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
    string msg = buf->retrieveAllAsString();
    LOG_INFO << "Received: " << msg;
    conn->send(msg);  // Echo the message back
}

int main(int argc, char* argv[])
{
    LOG_INFO << "Starting Echo Server";

    EventLoop loop;
    InetAddress listenAddr(8888);
    TcpServer server(&loop, listenAddr, "EchoServer");

    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);

    server.start();
    loop.loop();
}
