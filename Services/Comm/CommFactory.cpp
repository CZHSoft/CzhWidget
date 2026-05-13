#include "CommFactory.h"
#include "SerialComm.h"
#include "TcpClientComm.h"
#include "TcpServerComm.h"
#include "UdpComm.h"

namespace Services {
namespace Comm {

std::unique_ptr<ICommInterface> CommFactory::createComm(CommProtocol protocol)
{
    switch (protocol) {
    case CommProtocol::Serial:
        return std::make_unique<SerialComm>();
    case CommProtocol::TcpClient:
        return std::make_unique<TcpClientComm>();
    case CommProtocol::TcpServer:
        return std::make_unique<TcpServerComm>();
    case CommProtocol::Udp:
        return std::make_unique<UdpComm>();
    default:
        return nullptr;
    }
}

void CommFactory::destroyComm(ICommInterface* comm)
{
    delete comm;
}

} // namespace Comm
} // namespace Services
