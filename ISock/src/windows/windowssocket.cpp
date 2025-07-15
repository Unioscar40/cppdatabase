#include "windowssocket.h"

namespace net {
namespace core {
namespace isock {

WindowsSocket::WindowsSocket() : _initialize(false), _socket(INVALID_SOCKET) { }

WindowsSocket::~WindowsSocket()
{
    if (_initialize) 
        WSACleanup();
    if (_socket != INVALID_SOCKET)
        closesocket(_socket);

}

SocketError
WindowsSocket::create()
{
    if (!_initialize) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
            return SocketError::CreationFailed;
        _initialize = true;
    }

    _socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET)
        return SocketError::CreationFailed;

    return SocketError::None;
}

SocketError 
WindowsSocket::bind(int port, const std::string &ip) 
{
    sockaddr_in hint; 
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    if (::bind(_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) 
        return SocketError::BindFailed;
        
    
}

} // namespace net
} // namespace core
} // namespace isock