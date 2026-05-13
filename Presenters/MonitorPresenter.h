#ifndef MONITORPRESENTER_H
#define MONITORPRESENTER_H

#include <QObject>
#include <QString>
#include <QByteArray>

#include "Models/MonitorModel.h"
#include "Services/Comm/CommProtocol.h"
#include "Services/Comm/CommConfig.h"

namespace Views {
namespace Pages {
class MonitorPage;
}
namespace Panels {
class AutoSendPanel;
}
}

namespace Presenters {
class AutoSendPresenter;
}

namespace Presenters {

class MonitorPresenter : public QObject
{
    Q_OBJECT

public:
    explicit MonitorPresenter(Views::Pages::MonitorPage* view, QObject* parent = nullptr);
    ~MonitorPresenter() override;

    void init();
    void setAutoSendPanel(Views::Panels::AutoSendPanel* panel);

    void onConnectButtonClicked(Services::Comm::CommProtocol protocol);
    void onDisconnectButtonClicked();
    void onSendData(const QString& data, bool hexMode, bool appendNewLine);
    void onRefreshSerialPorts();
    void onProtocolChanged(Services::Comm::CommProtocol protocol);
    void onHexDisplayModeChanged(bool enabled);
    void onAppendNewLineChanged(bool enabled);

    void updateSerialConfig(const QString& port, int baudRate, int dataBits,
                            const QString& parity, int stopBits);
    void updateTcpClientConfig(const QString& ip, int port, bool autoReconnect);
    void updateTcpServerConfig(int port);
    void updateUdpConfig(int localPort, const QString& targetIp, bool broadcast);

signals:
    void connectionStatusChanged(bool connected, const QString& protocolName);
    void dataReceived(const QString& data, bool autoNewLine);
    void errorOccurred(const QString& error);
    void serialPortsUpdated(const QStringList& ports);

private slots:
    void onModelConnected();
    void onModelDisconnected();
    void onModelDataReceived(const QByteArray& data);
    void onModelErrorOccurred(const QString& error);
    void onModelConnectionStatusChanged(bool connected);
    void onModelSerialPortsRefreshed(const QStringList& ports);

private:
    Views::Pages::MonitorPage* _view;
    Models::MonitorModel* _model;
    AutoSendPresenter* _autoSendPresenter;
    Services::Comm::CommConfig _currentConfig;
    bool _hexDisplayMode;
    bool _appendNewLineEnabled;

    QString formatData(const QByteArray& data, bool hexMode);
};

}

#endif // MONITORPRESENTER_H
