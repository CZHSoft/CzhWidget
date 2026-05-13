#include "UdpComm.h"

namespace Services {
namespace Comm {

UdpComm::UdpComm(QObject* parent)
    : QObject(parent), _udpSocket(new QUdpSocket(this)), _lastError("")
{
    QObject::connect(_udpSocket, &QUdpSocket::readyRead, this, &UdpComm::onReadyRead);
    QObject::connect(_udpSocket, &QUdpSocket::errorOccurred, this, &UdpComm::onErrorOccurred);
}

UdpComm::~UdpComm()
{
    disconnect();
}

bool UdpComm::connect()
{
    if (_udpSocket->isOpen()) {
        _udpSocket->close();
    }

    bool success = _udpSocket->bind(QHostAddress(_config.udpLocalAddr),
                                     _config.udpLocalPort,
                                     QUdpSocket::ShareAddress);
    if (success) {
        if (_config.isBroadcast) {
            // Skip BroadcastOption for compatibility
        }
        emit connected();
    } else {
        _lastError = _udpSocket->errorString();
        emit errorOccurred(_lastError);
    }

    return success;
}

bool UdpComm::disconnect()
{
    if (_udpSocket->isOpen()) {
        _udpSocket->close();
        emit disconnected();
        return true;
    }
    return false;
}

bool UdpComm::isConnected() const
{
    return _udpSocket->isOpen();
}

qint64 UdpComm::sendData(const QByteArray& data)
{
    if (!_udpSocket->isOpen()) {
        _lastError = "UDP socket not bound";
        return -1;
    }

    QByteArray sendData = data;
    if (_config.appendNewLine) {
        sendData += _config.newLineType.toUtf8();
    }

    QHostAddress targetAddr(_config.udpRemoteAddr);
    return _udpSocket->writeDatagram(sendData, targetAddr, _config.udpRemotePort);
}

QByteArray UdpComm::receiveData()
{
    if (!_udpSocket->isOpen()) {
        return QByteArray();
    }

    QByteArray data;
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(_udpSocket->pendingDatagramSize());
        _udpSocket->readDatagram(buffer.data(), buffer.size(),
                                  &_lastSenderAddress, &_lastSenderPort);
        data += buffer;
    }

    return data;
}

QString UdpComm::getErrorString() const
{
    return _lastError;
}

QString UdpComm::getConnectionInfo() const
{
    if (!_udpSocket->isOpen()) {
        return "Not bound";
    }
    return QString("Local: %1:%2, Remote: %3:%4")
           .arg(_config.udpLocalAddr).arg(_config.udpLocalPort)
           .arg(_config.udpRemoteAddr).arg(_config.udpRemotePort);
}

void UdpComm::setConfig(const CommConfig& config)
{
    _config = config;
}

CommConfig UdpComm::getConfig() const
{
    return _config;
}

void UdpComm::setBroadcastMode(bool enable)
{
    _config.isBroadcast = enable;
    // Skip BroadcastOption for compatibility
}

void UdpComm::joinMulticastGroup(const QString& groupAddress)
{
    if (_udpSocket->isOpen()) {
        _udpSocket->joinMulticastGroup(QHostAddress(groupAddress));
    }
}

void UdpComm::leaveMulticastGroup(const QString& groupAddress)
{
    if (_udpSocket->isOpen()) {
        _udpSocket->leaveMulticastGroup(QHostAddress(groupAddress));
    }
}

void UdpComm::onReadyRead()
{
    QByteArray data;
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(_udpSocket->pendingDatagramSize());
        _udpSocket->readDatagram(buffer.data(), buffer.size(),
                                  &_lastSenderAddress, &_lastSenderPort);
        data += buffer;
    }

    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}

void UdpComm::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    _lastError = _udpSocket->errorString();
    emit errorOccurred(_lastError);
}

} // namespace Comm
} // namespace Services
