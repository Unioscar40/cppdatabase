#include <ISock/error.h>

namespace net {
namespace core {

std::string
errorString(SocketError err) 
{
    switch (err) {
        case SocketError::AcceptFailed:
            return "accept failed";
        case SocketError::BindFailed:
            return "bind failed";
        case SocketError::ConnectFailed:
            return "socket connection failed";
        case SocketError::CreationFailed:
            return "socket creation failed";
        case SocketError::InvalidSocket:
            return "invalid socket";
        case SocketError::ListenFailed:
            return "socket listen failed";
        case SocketError::None:
            return "";
        case SocketError::ReceiveFailed:
            return "receive failed";
        case SocketError::SendFailed:
            return "send failed";
        default:
            return "unknown error";
    }
}

bool
isError(SocketError err)
{
    return (err != SocketError::None);
}

} // namespace core
} // namespace net