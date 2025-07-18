#include <gtest/gtest.h>
#include <ISock/error.h>

class ErrorStringTest : public testing::TestWithParam<std::pair<net::core::SocketError, std::string>> {};

TEST_P(ErrorStringTest, convertToString) 
{
    using namespace net::core;

    auto test = GetParam();
    EXPECT_EQ(errorString(test.first), test.second);
}

INSTANTIATE_TEST_SUITE_P(ErrorStrings,
                         ErrorStringTest,
                         ::testing::Values(std::make_pair(net::core::SocketError::AcceptFailed, "accept failed"), 
                                           std::make_pair(net::core::SocketError::BindFailed, "bind failed"),
                                           std::make_pair(net::core::SocketError::ConnectFailed, "socket connection failed"),
                                           std::make_pair(net::core::SocketError::CreationFailed, "socket creation failed"),
                                           std::make_pair(net::core::SocketError::InvalidSocket, "invalid socket"),
                                           std::make_pair(net::core::SocketError::ListenFailed, "socket listen failed"),
                                           std::make_pair(net::core::SocketError::None, ""),
                                           std::make_pair(net::core::SocketError::ReceiveFailed, "receive failed"),
                                           std::make_pair(net::core::SocketError::SendFailed, "send failed"),
                                           std::make_pair(net::core::SocketError::UnknownError, "unknown error")
                                        ));

TEST(ErrorTest, isErrorValid)
{
    using namespace net::core;

    EXPECT_TRUE(!isError(SocketError::None));
}