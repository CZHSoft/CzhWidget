#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#include <QObject>
#include <QMutex>
#include <memory>
#include "CommProtocol.h"
#include "CommConfig.h"
#include "ICommInterface.h"

namespace Services {
namespace Comm {

class CommManager : public QObject
{
    Q_OBJECT

public:
    static CommManager* getInstance();
    
    void setProtocol(CommProtocol protocol);
    CommProtocol getCurrentProtocol() const;
    
    bool connect();
    bool disconnect();
    bool isConnected() const;
    
    qint64 sendData(const QByteArray& data);
    
    void setConfig(const CommConfig& config);
    CommConfig getConfig() const;
    
    bool saveConfig(const QString& filePath);
    bool loadConfig(const QString& filePath);
    
    QString getErrorString() const;
    QString getConnectionInfo() const;
    
    int getClientCount() const;
    QList<QString> getClientList() const;

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray& data);
    void errorOccurred(const QString& error);
    void clientConnected(const QString& clientInfo);
    void clientDisconnected(const QString& clientInfo);
    void protocolChanged(CommProtocol protocol);

private:
    CommManager(QObject* parent = nullptr);
    ~CommManager() override;
    
    void createCommInstance();
    void connectSignals();
    
    static CommManager* _instance;
    static QMutex _instanceMutex;
    
    CommProtocol _currentProtocol;
    std::unique_ptr<ICommInterface> _currentComm;
    CommConfig _config;
    mutable QMutex _configMutex;
};

} // namespace Comm
} // namespace Services

#endif // COMMMANAGER_H
