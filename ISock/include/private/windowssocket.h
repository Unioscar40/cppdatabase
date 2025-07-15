#pragma once

#include "isocket.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace net {
namespace core {
namespace isock {

class WindowsSocket : public ISocket {

private:
    bool        _initialize;
    SOCKET      _socket;
    int         _port;
    std::string _ip;
public: 
    WindowsSocket();
    ~WindowsSocket();

    SocketError create() override;
    SocketError bind(int port, const std::string &ip) override;
    SocketError listen() override;
    std::pair<SocketError, ISocket*> accept() override;
    SocketError connect(const std::string &ip, int port) override;
    std::pair<SocketError, int> send(const void* buffer, size_t length) override;
    std::pair<SocketError, int> receive(void* buffer, size_t length) override;
    void close() override;
    bool isValid() const override;

    std::string getErrorString(SocketError error) const = 0;
};

}
}
}