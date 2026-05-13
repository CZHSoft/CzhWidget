#ifndef COMMPROTOCOL_H
#define COMMPROTOCOL_H

#include <QString>

namespace Services {
namespace Comm {

enum class CommProtocol
{
    Serial,
    TcpClient,
    TcpServer,
    Udp
};

inline QString protocolToString(CommProtocol protocol)
{
    switch (protocol) {
    case CommProtocol::Serial:
        return "Serial";
    case CommProtocol::TcpClient:
        return "TcpClient";
    case CommProtocol::TcpServer:
        return "TcpServer";
    case CommProtocol::Udp:
        return "Udp";
    default:
        return "Unknown";
    }
}

inline CommProtocol stringToProtocol(const QString& str)
{
    if (str == "Serial")
        return CommProtocol::Serial;
    else if (str == "TcpClient")
        return CommProtocol::TcpClient;
    else if (str == "TcpServer")
        return CommProtocol::TcpServer;
    else if (str == "Udp")
        return CommProtocol::Udp;
    return CommProtocol::Serial;
}

} // namespace Comm
} // namespace Services

#endif // COMMPROTOCOL_H
