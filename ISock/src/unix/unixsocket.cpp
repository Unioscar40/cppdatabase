#include "unixsocket.h"

#define LISTEN_BACKLOG 50

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

    if(::bind(_socket, (sockaddr*)&addr, sizeof(addr)) == -1) 
        return SocketError::BindFailed;
    
    return SocketError::None;
}

SocketError
UnixSocket::listen()
{
    if (invalidSocket(_socket))
        return SocketError::InvalidSocket;

    if (::listen(_socket, LISTEN_BACKLOG) == -1) 
        return SocketError::ListenFailed;

    return SocketError::None;
}

std::pair<SocketError, std::unique_ptr<ISocket>>
UnixSocket::accept()
{
    if (invalidSocket(_socket))
        return std::make_pair<SocketError, std::unique_ptr<ISocket>>(SocketError::InvalidSocket, nullptr);

    int clientSocket = ::accept(_socket, nullptr, nullptr);
    if (invalidSocket(clientSocket))
        return std::make_pair<SocketError, std::unique_ptr<ISocket>>(SocketError::AcceptFailed, nullptr);
    return std::make_pair<SocketError, std::unique_ptr<ISocket>>(
        SocketError::None, 
        std::make_unique<UnixSocket>(clientSocket)
    );
}



}
}