#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

void onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        LOG_INFO << "Connected to server";
        conn->send("Hello from Client!\n");
    }
    else
    {
        LOG_INFO << "Disconnected from server";
    }
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
    string msg = buf->retrieveAllAsString();
    LOG_INFO << "Received from server: " << msg;
}

int main(int argc, char* argv[])
{
    EventLoop loop;
    InetAddress serverAddr("127.0.0.1", 8888);
    TcpClient client(&loop, serverAddr, "EchoClient");

    client.setConnectionCallback(onConnection);
    client.setMessageCallback(onMessage);

    client.connect();
    loop.loop();
}
