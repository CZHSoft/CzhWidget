#ifndef ICOMMINTERFACE_H
#define ICOMMINTERFACE_H

#include <QByteArray>
#include <QString>
#include "CommConfig.h"

namespace Services {
namespace Comm {

class ICommInterface
{
public:
    virtual ~ICommInterface() = default;
    
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    virtual qint64 sendData(const QByteArray& data) = 0;
    virtual QByteArray receiveData() = 0;
    
    virtual QString getErrorString() const = 0;
    virtual QString getConnectionInfo() const = 0;
    
    virtual void setConfig(const CommConfig& config) = 0;
    virtual CommConfig getConfig() const = 0;
};

} // namespace Comm
} // namespace Services

Q_DECLARE_INTERFACE(Services::Comm::ICommInterface, "com.czhwidget.comm.ICommInterface")

#endif // ICOMMINTERFACE_H
