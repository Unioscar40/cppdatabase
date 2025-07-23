#pragma once 

#include <memory>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ISock/isocket.h>

namespace net {
namespace core {

class UnixSocket : public ISocket {

private:
    bool _initialize{false};
    int  _socket;

public:
    UnixSocket();
    UnixSocket(int socket);
    ~UnixSocket();

    SocketError create() override;
    SocketError bind(int port, const std::string &ip) override;
    SocketError listen() override;
    std::pair<SocketError, std::unique_ptr<ISocket>> accept() override;
    SocketError connect(const std::string &ip, int port) override;
    SocketError send(const void* buffer, size_t lenght) override;
    SocketError receive(void* buffer, size_t length, int &bytesReceived) override;
    void close() override;
    bool isValid() const override;
};

}
}
