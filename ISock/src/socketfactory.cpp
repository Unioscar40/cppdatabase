#include <ISock/socketfactory.h>

# ifdef _WIN32
#include "windowssocket.h"
#else 
#include "unixsocket.h"
# endif

namespace net {
namespace core {

std::unique_ptr<ISocket>
SocketFactory::createSocket() 
{
#ifdef _WIN32
    return std::make_unique<WindowsSocket>();
#else 
    return std::make_unique<UnixSocket>();
#endif
}

} // namespace core
} // namespace net