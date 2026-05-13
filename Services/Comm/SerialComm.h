#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include "ICommInterface.h"

namespace Services {
namespace Comm {

class SerialComm : public QObject, public ICommInterface
{
    Q_OBJECT
    Q_INTERFACES(Services::Comm::ICommInterface)

public:
    explicit SerialComm(QObject* parent = nullptr);
    ~SerialComm() override;
    
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
    void onReadyRead();
    void onErrorOccurred(QSerialPort::SerialPortError error);

private:


    QSerialPort* _serialPort;
    CommConfig _config;
    QString _lastError;
};

} // namespace Comm
} // namespace Services

#endif // SERIALCOMM_H
