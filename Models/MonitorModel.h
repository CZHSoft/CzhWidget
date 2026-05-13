#ifndef MONITORMODEL_H
#define MONITORMODEL_H

#include <QObject>
#include <QString>
#include <QByteArray>

#include "Services/Comm/CommProtocol.h"
#include "Services/Comm/CommConfig.h"
#include "Services/Comm/CommManager.h"

namespace Models {

class MonitorModel : public QObject
{
    Q_OBJECT

public:
    explicit MonitorModel(QObject* parent = nullptr);
    ~MonitorModel() override;

    void setProtocol(Services::Comm::CommProtocol protocol);
    Services::Comm::CommProtocol getProtocol() const;

    void setConfig(const Services::Comm::CommConfig& config);
    Services::Comm::CommConfig getConfig() const;

    bool connectComm();
    bool disconnectComm();
    bool isConnected() const;

    qint64 sendData(const QByteArray& data);
    QString getErrorString() const;
    QString getConnectionInfo() const;

    void refreshSerialPorts();
    QStringList getAvailableSerialPorts() const;

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);
    void connectionStatusChanged(bool connected);
    void serialPortsRefreshed(const QStringList& ports);
    void clientConnected(const QString& clientInfo);
    void clientDisconnected(const QString& clientInfo);

private slots:
    void onDataReceived(const QByteArray& data);
    void onErrorOccurred(const QString& error);
    void onConnected();
    void onDisconnected();
    void onClientConnected(const QString& clientInfo);
    void onClientDisconnected(const QString& clientInfo);

private:
    Services::Comm::CommManager* _commManager;
};

}

#endif // MONITORMODEL_H