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
    SendFailed,
    ReceiveFailed,
    UnknownError
};

class ISocket {
public:
    virtual ~ISocket() = default;

    virtual SocketError create() = 0;
    virtual SocketError bind(int port, const std::string &ip) = 0;
    virtual SocketError listen() = 0; 
    virtual std::pair<SocketError, ISocket*> accept() = 0;
    virtual SocketError connect(const std::string &ip, int port) = 0;
    virtual std::pair<SocketError, int> send(const void* buffer, size_t lenght) = 0;
    virtual std::pair<SocketError, int> receive(void* buffer, size_t lenht) = 0;
    virtual void close() = 0;
    virtual bool isValid() const = 0;

    virtual std::string getErrorString(SocketError error) const = 0;
};


} // namespace net
} // namespace core
} //namespace isock