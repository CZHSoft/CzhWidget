#include "Models/MonitorModel.h"
#include <QSerialPortInfo>

namespace Models {

MonitorModel::MonitorModel(QObject* parent)
    : QObject(parent)
    , _commManager(Services::Comm::CommManager::getInstance())
{
    QObject::connect(_commManager, &Services::Comm::CommManager::dataReceived,
                     this, &MonitorModel::onDataReceived);
    QObject::connect(_commManager, &Services::Comm::CommManager::errorOccurred,
                     this, &MonitorModel::onErrorOccurred);
    QObject::connect(_commManager, &Services::Comm::CommManager::connected,
                     this, &MonitorModel::onConnected);
    QObject::connect(_commManager, &Services::Comm::CommManager::disconnected,
                     this, &MonitorModel::onDisconnected);
    QObject::connect(_commManager, &Services::Comm::CommManager::clientConnected,
                     this, &MonitorModel::onClientConnected);
    QObject::connect(_commManager, &Services::Comm::CommManager::clientDisconnected,
                     this, &MonitorModel::onClientDisconnected);
}

MonitorModel::~MonitorModel()
{
}

void MonitorModel::setProtocol(Services::Comm::CommProtocol protocol)
{
    _commManager->setProtocol(protocol);
}

Services::Comm::CommProtocol MonitorModel::getProtocol() const
{
    return _commManager->getCurrentProtocol();
}

void MonitorModel::setConfig(const Services::Comm::CommConfig& config)
{
    _commManager->setConfig(config);
}

Services::Comm::CommConfig MonitorModel::getConfig() const
{
    return _commManager->getConfig();
}

bool MonitorModel::connectComm()
{
    bool success = _commManager->connect();
    return success;
}

bool MonitorModel::disconnectComm()
{
    bool success = _commManager->disconnect();
    return success;
}

bool MonitorModel::isConnected() const
{
    return _commManager->isConnected();
}

qint64 MonitorModel::sendData(const QByteArray& data)
{
    return _commManager->sendData(data);
}

QString MonitorModel::getErrorString() const
{
    return _commManager->getErrorString();
}

QString MonitorModel::getConnectionInfo() const
{
    return _commManager->getConnectionInfo();
}

void MonitorModel::refreshSerialPorts()
{
    QStringList ports = this->getAvailableSerialPorts();
    emit serialPortsRefreshed(ports);
}

QStringList MonitorModel::getAvailableSerialPorts() const
{
    QStringList portList;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        QString portName = info.portName();
        if (!portName.isEmpty()) {
            portList << portName;
        }
    }
    return portList;
}

void MonitorModel::onDataReceived(const QByteArray& data)
{
    emit dataReceived(data);
}

void MonitorModel::onErrorOccurred(const QString& error)
{
    emit errorOccurred(error);
}

void MonitorModel::onConnected()
{
    emit connected();
    emit connectionStatusChanged(true);
}

void MonitorModel::onDisconnected()
{
    emit disconnected();
    emit connectionStatusChanged(false);
}

void MonitorModel::onClientConnected(const QString& clientInfo)
{
    emit clientConnected(clientInfo);
}

void MonitorModel::onClientDisconnected(const QString& clientInfo)
{
    emit clientDisconnected(clientInfo);
}

}