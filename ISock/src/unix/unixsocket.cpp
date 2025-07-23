#include "unixsocket.h"

namespace {

bool 
invalidSocket(int socket)
{
    return socket == -1;
}

}

namespace net {
namespace core {

UnixSocket::UnixSocket() : _initialize(false), _socket(-1) {}

UnixSocket::UnixSocket(int socket)
{
    _socket = socket;
}

UnixSocket::~UnixSocket()
{
    if (_initialize)
        ::close(_socket);
}

SocketError
UnixSocket::create()
{
    /* try create */
    if (invalidSocket(_socket))
        _socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (invalidSocket(_socket))
        return SocketError::CreationFailed;
    return SocketError::None;
}

SocketError
UnixSocket::bind(int port, const std::string &ip)
{
    if (invalidSocket(_socket))
        return SocketError::InvalidSocket;
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if(::bind(_socket, (sockaddr*)&addr, sizeof(addr)) == -1) {
        return SocketError::BindFailed;
    }
    return SocketError::None;
}

}
}