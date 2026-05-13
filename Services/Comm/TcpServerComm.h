#ifndef TCPSERVERCOMM_H
#define TCPSERVERCOMM_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "ICommInterface.h"

namespace Services {
namespace Comm {

class TcpServerComm : public QObject, public ICommInterface
{
    Q_OBJECT
    Q_INTERFACES(Services::Comm::ICommInterface)

public:
    explicit TcpServerComm(QObject* parent = nullptr);
    ~TcpServerComm() override;
    
    bool connect() override;
    bool disconnect() override;
    bool isConnected() const override;
    
    qint64 sendData(const QByteArray& data) override;
    QByteArray receiveData() override;
    
    QString getErrorString() const override;
    QString getConnectionInfo() const override;
    
    void setConfig(const CommConfig& config) override;
    CommConfig getConfig() const override;
    
    int getClientCount() const;
    QList<QString> getClientList() const;

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);
    void clientConnected(const QString& clientInfo);
    void clientDisconnected(const QString& clientInfo);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpServer* _tcpServer;
    QList<QTcpSocket*> _clientList;
    CommConfig _config;
    QString _lastError;
    QTcpSocket* _lastActiveClient;
};

} // namespace Comm
} // namespace Services

#endif // TCPSERVERCOMM_H
