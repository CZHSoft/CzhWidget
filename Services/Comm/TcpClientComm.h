#ifndef TCPCLIENTCOMM_H
#define TCPCLIENTCOMM_H

#include <QObject>
#include <QTcpSocket>
#include "ICommInterface.h"

namespace Services {
namespace Comm {

class TcpClientComm : public QObject, public ICommInterface
{
    Q_OBJECT
    Q_INTERFACES(Services::Comm::ICommInterface)

public:
    explicit TcpClientComm(QObject* parent = nullptr);
    ~TcpClientComm() override;
    
    bool connect() override;
    bool disconnect() override;
    bool isConnected() const override;
    
    qint64 sendData(const QByteArray& data) override;
    QByteArray receiveData() override;
    
    QString getErrorString() const override;
    QString getConnectionInfo() const override;
    
    void setConfig(const CommConfig& config) override;
    CommConfig getConfig() const override;

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpSocket* _tcpSocket;
    CommConfig _config;
    QString _lastError;
};

} // namespace Comm
} // namespace Services

#endif // TCPCLIENTCOMM_H
