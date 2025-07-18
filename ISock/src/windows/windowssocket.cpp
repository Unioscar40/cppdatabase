#include "windowssocket.h"

namespace net {
namespace core {

WindowsSocket::WindowsSocket() : _initialize(false), _socket(INVALID_SOCKET) { }

WindowsSocket::WindowsSocket(SOCKET socket) {
    _socket = socket;
}

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

    return SocketError::None;
}

SocketError 
WindowsSocket::listen()
{
    if (::listen(_socket, SOMAXCONN), SOCKET_ERROR) 
        return SocketError::ListenFailed;
    
    return SocketError::None;
    
}

std::pair<SocketError, std::unique_ptr<ISocket>>
WindowsSocket::accept()
{
    sockaddr_in client; 
    int clientSize = sizeof(client);
    
    SOCKET clientSocket = ::accept(_socket, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET)
        return std::make_pair<SocketError, std::unique_ptr<ISocket>>(SocketError::AcceptFailed, nullptr);

    return std::make_pair<SocketError, std::unique_ptr<ISocket>>(
        SocketError::None, 
        std::make_unique<WindowsSocket>(clientSocket)
    );
}

SocketError
WindowsSocket::connect(const std::string &ip, int port) 
{
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

    if (::connect(_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
        return SocketError::ConnectFailed;
    return SocketError::None;
}

SocketError
WindowsSocket::send(const void *buffer, size_t length)
{
    const char* bffr = static_cast<const char*>(buffer);
    if (_socket == INVALID_SOCKET) 
        return SocketError::InvalidSocket;

    int sendResult = ::send(_socket, bffr, length, 0);
    if (sendResult == SOCKET_ERROR) 
        return SocketError::SendFailed;
    
    return SocketError::None; 
}

SocketError
WindowsSocket::receive(void* buffer, size_t length) 
{
    if (_socket == INVALID_SOCKET) 
        return SocketError::InvalidSocket;
    
    int bytesReceived = ::recv(_socket, static_cast<char*>(buffer), length, 0);
    
    return SocketError::None;
}

void 
WindowsSocket::close() 
{
    closesocket(_socket);
}

bool 
WindowsSocket::isValid() const
{
    return _socket != INVALID_SOCKET;
}

} // namespace core
} // namespace net