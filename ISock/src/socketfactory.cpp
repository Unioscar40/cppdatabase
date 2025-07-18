#include <ISock/socketfactory.h>

# ifdef _WIN32
#include "windowssocket.h"
# endif

namespace net {
namespace core {

std::unique_ptr<ISocket>
SocketFactory::createSocket() 
{
#ifdef _WIN32
    return std::make_unique<WindowsSocket>();
#endif
}

} // namespace core
} // namespace net