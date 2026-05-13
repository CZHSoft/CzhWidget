#ifndef UDPCOMM_H
#define UDPCOMM_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include "ICommInterface.h"

namespace Services {
namespace Comm {

class UdpComm : public QObject, public ICommInterface
{
    Q_OBJECT
    Q_INTERFACES(Services::Comm::ICommInterface)

public:
    explicit UdpComm(QObject* parent = nullptr);
    ~UdpComm() override;
    
    bool connect() override;
    bool disconnect() override;
    bool isConnected() const override;
    
    qint64 sendData(const QByteArray& data) override;
    QByteArray receiveData() override;
    
    QString getErrorString() const override;
    QString getConnectionInfo() const override;
    
    void setConfig(const CommConfig& config) override;
    CommConfig getConfig() const override;
    
    void setBroadcastMode(bool enable);
    void joinMulticastGroup(const QString& groupAddress);
    void leaveMulticastGroup(const QString& groupAddress);

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);

private slots:
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QUdpSocket* _udpSocket;
    CommConfig _config;
    QString _lastError;
    QHostAddress _lastSenderAddress;
    quint16 _lastSenderPort;
};

} // namespace Comm
} // namespace Services

#endif // UDPCOMM_H
