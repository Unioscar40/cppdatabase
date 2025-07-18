#pragma once

#include <string>
#include "error.h"

namespace net {
namespace core {


class ISocket {
public:

    virtual ~ISocket() = default;

    virtual SocketError create() = 0;
    virtual SocketError bind(int port, const std::string &ip) = 0;
    virtual SocketError listen() = 0; 
    virtual std::pair<SocketError, std::unique_ptr<ISocket>> accept() = 0;
    virtual SocketError connect(const std::string &ip, int port) = 0;
    virtual SocketError send(const void* buffer, size_t lenght) = 0;
    virtual SocketError receive(void* buffer, size_t lenht) = 0;
    virtual void close() = 0;
    virtual bool isValid() const = 0;
};



} // namespace core
} //namespace net