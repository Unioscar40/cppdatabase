#include <thread>

#include <gtest/gtest.h>
#include <ISock/socketfactory.h>

namespace {

std::unique_ptr<net::core::ISocket> 
createSocket()
{
    using namespace net::core;

    auto socket = SocketFactory::createSocket();

    SocketError err = socket->create();
    EXPECT_EQ(err, SocketError::None);
    EXPECT_TRUE(socket->isValid());

    return socket;
}

std::unique_ptr<net::core::ISocket> 
createInvalidSocket()
{
    using namespace net::core;

    auto socket = SocketFactory::createSocket();

    EXPECT_FALSE(socket->isValid());

    return socket;
}

}

TEST(SocketTest, CreationWorks) 
{
    using namespace net::core;
    auto s = createSocket();
}

TEST(SocketTest, CreationFailed)
{
    using namespace net::core;
    auto s = createSocket();
}

TEST(SocketTest, BindSocket)
{
    using namespace net::core;
    auto s = createSocket();
    
    SocketError err = s->bind(54000, "127.0.0.1");
    EXPECT_EQ(err, SocketError::None);
}

TEST(SocketTest, ErrorBindSocket)
{
    using namespace net::core;
    auto s = createSocket();
    
    SocketError err = s->bind(8080, "72.0.0.1");
    EXPECT_EQ(err, SocketError::BindFailed);
}

TEST(SocketTest, SocketListen)
{
    using namespace net::core;
    auto s = createSocket();

    SocketError err = s->bind(54000, "127.0.0.1");
    if (isError(err)) {
        EXPECT_TRUE(false);
        return;
    }
    err = s->listen();
    EXPECT_EQ(err, SocketError::None);
}

TEST(SocketTest, InvalidSocketListen)
{
    using namespace net::core;
    auto s = createSocket();

    auto err = s->listen();
    EXPECT_EQ(err, SocketError::ListenFailed);
}

TEST(SocketTest, AcceptConexion)
{
    using namespace net::core;
    auto s = createSocket();

    SocketError err = s->bind(54000, "127.0.0.1");
    if (isError(err)) {
        EXPECT_TRUE(false);
        return;
    }

    err = s->listen(); 
    if (isError(err)) {
        EXPECT_TRUE(false);
        return;
    }

    std::thread clientThread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto clientSocket = createSocket();
        auto errConnect = clientSocket->connect("127.0.0.1", 54000);
        EXPECT_EQ(errConnect, SocketError::None);
        if (isError(errConnect)) {
            std::cerr << errorString(errConnect) << std::endl;
            return;
        }
    });

    auto [result, client] = s->accept();
    EXPECT_EQ(result, SocketError::None);
    EXPECT_TRUE(client->isValid());

    clientThread.join();
}

TEST(SocketTest, AcceptConexionFailed) 
{
    using namespace net::core;
    auto s = createSocket();

    SocketError err = s->bind(54000, "127.0.0.1");
    if (isError(err)) {
        EXPECT_TRUE(false);
        return;
    }

    auto [result, client] = s->accept();
    EXPECT_EQ(result, SocketError::AcceptFailed);
    EXPECT_EQ(client, nullptr);
}

TEST(SocketTest, ConnectFailed) 
{
    using namespace net::core;

    auto clientSocket = createInvalidSocket();
    auto errConnect = clientSocket->connect("127.0.0.1", 54000);

    EXPECT_EQ(errConnect, SocketError::InvalidSocket);
}

TEST(SocketTest, SendReceiveMessage) 
{
    using namespace net::core;
    auto s = createSocket();

    SocketError err = s->bind(54000, "127.0.0.1");
    if (isError(err)) {
        EXPECT_TRUE(false);
        return;
    }

    err = s->listen(); 
    if (isError(err)) {
        EXPECT_TRUE(false);
        return;
    }

    std::thread clientThread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto clientSocket = createSocket();
        auto errConnect = clientSocket->connect("127.0.0.1", 54000);
        EXPECT_EQ(errConnect, SocketError::None);
        if (isError(errConnect)) {
            std::cerr << errorString(errConnect) << std::endl;
            return;
        }

        const char * message = "hola server";
        auto errSend = clientSocket->send(message, strlen(message));
        EXPECT_EQ(errSend, SocketError::None);

    });

    auto [result, client] = s->accept();
    EXPECT_EQ(result, SocketError::None);
    EXPECT_TRUE(client->isValid());

    char buffer[1024]; 
    int bytes;
    client->receive(buffer, sizeof(buffer), bytes);
    buffer[bytes] = '\0';
    EXPECT_EQ(strcmp("hola server", buffer), 0);

    clientThread.join();
}

TEST(SocketTest, SendReceiveFailed) 
{
    using namespace net::core;

    auto clientSocket = createSocket();

    const char * message = "hola server";
    auto errSend = clientSocket->send(message, strlen(message));
    EXPECT_EQ(errSend, SocketError::SendFailed);

}