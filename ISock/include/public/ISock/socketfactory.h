#pragma once 

#include <memory>
#include "isocket.h"

namespace net {
namespace core {

class SocketFactory {
public:
    static std::unique_ptr<ISocket> createSocket();
};

} // namespace core
} // namespace core