#include "TcpServerComm.h"

namespace Services {
namespace Comm {

TcpServerComm::TcpServerComm(QObject* parent)
    : QObject(parent), _tcpServer(new QTcpServer(this)), _lastError(""), _lastActiveClient(nullptr)
{
    QObject::connect(_tcpServer, &QTcpServer::newConnection, this, &TcpServerComm::onNewConnection);
}

TcpServerComm::~TcpServerComm()
{
    disconnect();
}

bool TcpServerComm::connect()
{
    if (_tcpServer->isListening()) {
        _tcpServer->close();
    }

    bool success = _tcpServer->listen(QHostAddress::Any, _config.listenPort);
    if (success) {
        emit connected();
    } else {
        _lastError = _tcpServer->errorString();
        emit errorOccurred(_lastError);
    }

    return success;
}

bool TcpServerComm::disconnect()
{
    if (_tcpServer->isListening()) {
        qDeleteAll(_clientList);
        _clientList.clear();
        _tcpServer->close();
        emit disconnected();
        return true;
    }
    return false;
}

bool TcpServerComm::isConnected() const
{
    return _tcpServer->isListening();
}

qint64 TcpServerComm::sendData(const QByteArray& data)
{
    if (!_tcpServer->isListening()) {
        _lastError = "Server not listening";
        return -1;
    }

    QByteArray sendData = data;
    if (_config.appendNewLine) {
        sendData += _config.newLineType.toUtf8();
    }

    qint64 totalSent = 0;
    for (QTcpSocket* client : _clientList) {
        if (client->state() == QAbstractSocket::ConnectedState) {
            totalSent += client->write(sendData);
        }
    }

    return totalSent;
}

QByteArray TcpServerComm::receiveData()
{
    if (!_tcpServer->isListening()) {
        return QByteArray();
    }

    QByteArray data;
    for (QTcpSocket* client : _clientList) {
        if (client->bytesAvailable() > 0) {
            _lastActiveClient = client;
            data = client->readAll();
            break;
        }
    }

    return data;
}

QString TcpServerComm::getErrorString() const
{
    return _lastError;
}

QString TcpServerComm::getConnectionInfo() const
{
    if (!_tcpServer->isListening()) {
        return "Not listening";
    }
    return QString("Listening on port %1, %2 clients connected")
           .arg(_config.listenPort)
           .arg(_clientList.size());
}

void TcpServerComm::setConfig(const CommConfig& config)
{
    _config = config;
}

CommConfig TcpServerComm::getConfig() const
{
    return _config;
}

int TcpServerComm::getClientCount() const
{
    return _clientList.size();
}

QList<QString> TcpServerComm::getClientList() const
{
    QList<QString> clients;
    for (QTcpSocket* client : _clientList) {
        QString clientInfo = QString("%1:%2")
                            .arg(client->peerAddress().toString())
                            .arg(client->peerPort());
        clients.append(clientInfo);
    }
    return clients;
}

void TcpServerComm::onNewConnection()
{
    QTcpSocket* client = _tcpServer->nextPendingConnection();
    if (client) {
        _clientList.append(client);
        QObject::connect(client, &QTcpSocket::disconnected, this, &TcpServerComm::onClientDisconnected);
        QObject::connect(client, &QTcpSocket::readyRead, this, &TcpServerComm::onReadyRead);
        QObject::connect(client, &QTcpSocket::errorOccurred, this, &TcpServerComm::onErrorOccurred);

        QString clientInfo = QString("%1:%2")
                            .arg(client->peerAddress().toString())
                            .arg(client->peerPort());
        emit clientConnected(clientInfo);
    }
}

void TcpServerComm::onClientDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        QString clientInfo = QString("%1:%2")
                            .arg(client->peerAddress().toString())
                            .arg(client->peerPort());
        _clientList.removeOne(client);
        client->deleteLater();

        if (_lastActiveClient == client) {
            _lastActiveClient = nullptr;
        }

        emit clientDisconnected(clientInfo);
    }
}

void TcpServerComm::onReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        _lastActiveClient = client;
        QByteArray data = client->readAll();
        if (!data.isEmpty()) {
            emit dataReceived(data);
        }
    }
}

void TcpServerComm::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        _lastError = client->errorString();
    } else {
        _lastError = _tcpServer->errorString();
    }
    emit errorOccurred(_lastError);
}

} // namespace Comm
} // namespace Services
