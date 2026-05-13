#include "TcpClientComm.h"

namespace Services {
namespace Comm {

TcpClientComm::TcpClientComm(QObject* parent)
    : QObject(parent), _tcpSocket(new QTcpSocket(this)), _lastError("")
{
    QObject::connect(_tcpSocket, &QTcpSocket::connected, this, &TcpClientComm::onConnected);
    QObject::connect(_tcpSocket, &QTcpSocket::disconnected, this, &TcpClientComm::onDisconnected);
    QObject::connect(_tcpSocket, &QTcpSocket::readyRead, this, &TcpClientComm::onReadyRead);
    QObject::connect(_tcpSocket, &QTcpSocket::errorOccurred, this, &TcpClientComm::onErrorOccurred);
}

TcpClientComm::~TcpClientComm()
{
    disconnect();
}

bool TcpClientComm::connect()
{
    if (_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        _tcpSocket->disconnectFromHost();
    }

    _tcpSocket->connectToHost(_config.tcpHost, _config.tcpPort);

    if (!_tcpSocket->waitForConnected(3000)) {
        _lastError = _tcpSocket->errorString();
        emit errorOccurred(_lastError);
        return false;
    }

    return true;
}

bool TcpClientComm::disconnect()
{
    if (_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        _tcpSocket->disconnectFromHost();
        return true;
    }
    return false;
}

bool TcpClientComm::isConnected() const
{
    return _tcpSocket->state() == QAbstractSocket::ConnectedState;
}

qint64 TcpClientComm::sendData(const QByteArray& data)
{
    if (_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        _lastError = "Not connected to server";
        return -1;
    }

    QByteArray sendData = data;
    if (_config.appendNewLine) {
        sendData += _config.newLineType.toUtf8();
    }

    return _tcpSocket->write(sendData);
}

QByteArray TcpClientComm::receiveData()
{
    if (_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        return QByteArray();
    }
    return _tcpSocket->readAll();
}

QString TcpClientComm::getErrorString() const
{
    return _lastError;
}

QString TcpClientComm::getConnectionInfo() const
{
    if (_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        return "Not connected";
    }
    return QString("%1:%2").arg(_config.tcpHost).arg(_config.tcpPort);
}

void TcpClientComm::setConfig(const CommConfig& config)
{
    _config = config;
}

CommConfig TcpClientComm::getConfig() const
{
    return _config;
}

void TcpClientComm::onConnected()
{
    emit connected();
}

void TcpClientComm::onDisconnected()
{
    emit disconnected();
}

void TcpClientComm::onReadyRead()
{
    QByteArray data = _tcpSocket->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}

void TcpClientComm::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    _lastError = _tcpSocket->errorString();
    emit errorOccurred(_lastError);
}

} // namespace Comm
} // namespace Services
