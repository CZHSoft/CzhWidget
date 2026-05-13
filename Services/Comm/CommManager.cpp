#include "CommManager.h"
#include "CommFactory.h"
#include "TcpServerComm.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

namespace Services {
namespace Comm {

CommManager* CommManager::_instance = nullptr;
QMutex CommManager::_instanceMutex;

CommManager::CommManager(QObject* parent)
    : QObject(parent), _currentProtocol(CommProtocol::Serial)
{
    createCommInstance();
}

CommManager::~CommManager()
{
    disconnect();
}

CommManager* CommManager::getInstance()
{
    QMutexLocker locker(&_instanceMutex);
    if (!_instance) {
        _instance = new CommManager();
    }
    return _instance;
}

void CommManager::setProtocol(CommProtocol protocol)
{
    if (_currentProtocol != protocol) {
        bool wasConnected = isConnected();
        if (wasConnected) {
            disconnect();
        }

        _currentProtocol = protocol;
        createCommInstance();

        if (wasConnected) {
            connect();
        }

        emit protocolChanged(protocol);
    }
}

CommProtocol CommManager::getCurrentProtocol() const
{
    return _currentProtocol;
}

bool CommManager::connect()
{
    if (!_currentComm) {
        return false;
    }

    _currentComm->setConfig(_config);
    return _currentComm->connect();
}

bool CommManager::disconnect()
{
    if (!_currentComm) {
        return false;
    }
    return _currentComm->disconnect();
}

bool CommManager::isConnected() const
{
    if (!_currentComm) {
        return false;
    }
    return _currentComm->isConnected();
}

qint64 CommManager::sendData(const QByteArray& data)
{
    if (!_currentComm) {
        return -1;
    }
    return _currentComm->sendData(data);
}

void CommManager::setConfig(const CommConfig& config)
{
    QMutexLocker locker(&_configMutex);
    _config = config;

    if (_currentComm) {
        _currentComm->setConfig(config);
    }
}

CommConfig CommManager::getConfig() const
{
    QMutexLocker locker(&_configMutex);
    return _config;
}

bool CommManager::saveConfig(const QString& filePath)
{
    QMutexLocker locker(&_configMutex);

    QJsonObject jsonObj;
    QVariantMap map = _config.toVariantMap();

    for (auto it = map.begin(); it != map.end(); ++it) {
        jsonObj[it.key()] = it.value().toJsonValue();
    }

    QJsonDocument doc(jsonObj);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool CommManager::loadConfig(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return false;
    }

    QJsonObject jsonObj = doc.object();
    QVariantMap map = jsonObj.toVariantMap();

    QMutexLocker locker(&_configMutex);
    _config.fromVariantMap(map);

    if (_currentComm) {
        _currentComm->setConfig(_config);
    }

    return true;
}

QString CommManager::getErrorString() const
{
    if (!_currentComm) {
        return "No communication instance";
    }
    return _currentComm->getErrorString();
}

QString CommManager::getConnectionInfo() const
{
    if (!_currentComm) {
        return "No communication instance";
    }
    return _currentComm->getConnectionInfo();
}

int CommManager::getClientCount() const
{
    TcpServerComm* server = dynamic_cast<TcpServerComm*>(_currentComm.get());
    if (server) {
        return server->getClientCount();
    }
    return 0;
}

QList<QString> CommManager::getClientList() const
{
    TcpServerComm* server = dynamic_cast<TcpServerComm*>(_currentComm.get());
    if (server) {
        return server->getClientList();
    }
    return QList<QString>();
}

void CommManager::createCommInstance()
{
    _currentComm = CommFactory::createComm(_currentProtocol);
    connectSignals();
}

void CommManager::connectSignals()
{
    if (!_currentComm) {
        return;
    }

    QObject* obj = dynamic_cast<QObject*>(_currentComm.get());
    if (!obj) {
        return;
    }

    QObject::connect(obj, SIGNAL(connected()), this, SIGNAL(connected()));
    QObject::connect(obj, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    QObject::connect(obj, SIGNAL(dataReceived(QByteArray)), this, SIGNAL(dataReceived(QByteArray)));
    QObject::connect(obj, SIGNAL(errorOccurred(QString)), this, SIGNAL(errorOccurred(QString)));

    TcpServerComm* server = dynamic_cast<TcpServerComm*>(_currentComm.get());
    if (server) {
        QObject::connect(server, SIGNAL(clientConnected(QString)), this, SIGNAL(clientConnected(QString)));
        QObject::connect(server, SIGNAL(clientDisconnected(QString)), this, SIGNAL(clientDisconnected(QString)));
    }
}

} // namespace Comm
} // namespace Services
