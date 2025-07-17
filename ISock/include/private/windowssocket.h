#pragma once

#include <memory>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Isock/isocket.h>

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
    WindowsSocket(SOCKET socket);
    ~WindowsSocket();

    SocketError create() override;
    SocketError bind(int port, const std::string &ip) override;
    SocketError listen() override;
    std::pair<SocketError, std::unique_ptr<ISocket>> accept() override;
    SocketError connect(const std::string &ip, int port) override;
    SocketError send(const void* buffer, size_t length) override;
    SocketError receive(void* buffer, size_t length) override;
    void close() override;
    bool isValid() const override;
};

}
}
}