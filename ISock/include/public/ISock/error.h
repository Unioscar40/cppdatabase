#pragma once 

#include <string>

namespace net {
namespace core {
namespace isock {

enum class SocketError {
    None = 0,
    CreationFailed,
    BindFailed,
    ListenFailed,
    ConnectFailed,
    AcceptFailed,
    SendFailed,
    ReceiveFailed,
    InvalidSocket,
    UnknownError
};

std::string errorString(SocketError err);

bool isError(SocketError err);

} // namespace net
} // namespace core
} // namespace isock