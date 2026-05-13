#ifndef COMMCONFIG_H
#define COMMCONFIG_H

#include <QString>
#include <QVariantMap>

namespace Services {
namespace Comm {

struct CommConfig
{
    QString protocolType;
    
    QString serialPort;
    int baudRate = 115200;
    int dataBits = 8;
    QString parity = "None";
    int stopBits = 1;
    
    QString tcpHost = "127.0.0.1";
    int tcpPort = 8080;
    int listenPort = 8080;
    
    QString udpLocalAddr = "0.0.0.0";
    int udpLocalPort = 5000;
    QString udpRemoteAddr = "127.0.0.1";
    int udpRemotePort = 5000;
    bool isBroadcast = false;
    
    bool hexMode = false;
    bool appendNewLine = false;
    QString newLineType = "\r\n";
    
    QVariantMap toVariantMap() const
    {
        QVariantMap map;
        map["protocolType"] = protocolType;
        map["serialPort"] = serialPort;
        map["baudRate"] = baudRate;
        map["dataBits"] = dataBits;
        map["parity"] = parity;
        map["stopBits"] = stopBits;
        map["tcpHost"] = tcpHost;
        map["tcpPort"] = tcpPort;
        map["listenPort"] = listenPort;
        map["udpLocalAddr"] = udpLocalAddr;
        map["udpLocalPort"] = udpLocalPort;
        map["udpRemoteAddr"] = udpRemoteAddr;
        map["udpRemotePort"] = udpRemotePort;
        map["isBroadcast"] = isBroadcast;
        map["hexMode"] = hexMode;
        map["appendNewLine"] = appendNewLine;
        map["newLineType"] = newLineType;
        return map;
    }
    
    void fromVariantMap(const QVariantMap& map)
    {
        protocolType = map["protocolType"].toString();
        serialPort = map["serialPort"].toString();
        baudRate = map["baudRate"].toInt();
        dataBits = map["dataBits"].toInt();
        parity = map["parity"].toString();
        stopBits = map["stopBits"].toInt();
        tcpHost = map["tcpHost"].toString();
        tcpPort = map["tcpPort"].toInt();
        listenPort = map["listenPort"].toInt();
        udpLocalAddr = map["udpLocalAddr"].toString();
        udpLocalPort = map["udpLocalPort"].toInt();
        udpRemoteAddr = map["udpRemoteAddr"].toString();
        udpRemotePort = map["udpRemotePort"].toInt();
        isBroadcast = map["isBroadcast"].toBool();
        hexMode = map["hexMode"].toBool();
        appendNewLine = map["appendNewLine"].toBool();
        newLineType = map["newLineType"].toString();
    }
};

} // namespace Comm
} // namespace Services

#endif // COMMCONFIG_H
