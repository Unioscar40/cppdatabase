#include <gtest/gtest.h>
#include <ISock/socketfactory.h>

TEST(SocketTest, CreationWorks) 
{
    using namespace net::core;

    auto socket = SocketFactory::createSocket();
    ASSERT_NE(socket, nullptr);

    SocketError err = socket->create();
    EXPECT_EQ(err, SocketError::None);
    EXPECT_TRUE(socket->isValid());
}
