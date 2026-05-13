#include "Presenters/MonitorPresenter.h"
#include "Views/Pages/MonitorPage.h"
#include "Presenters/AutoSendPresenter.h"
#include "Views/Panels/AutoSendPanel.h"

#include <QDateTime>

namespace Presenters {

MonitorPresenter::MonitorPresenter(Views::Pages::MonitorPage* view, QObject* parent)
    : QObject(parent)
    , _view(view)
    , _model(new Models::MonitorModel(this))
    , _autoSendPresenter(nullptr)
    , _hexDisplayMode(false)
    , _appendNewLineEnabled(true)
{
}

MonitorPresenter::~MonitorPresenter()
{
}

void MonitorPresenter::init()
{
    connect(_model, &Models::MonitorModel::connected,
            this, &MonitorPresenter::onModelConnected);
    connect(_model, &Models::MonitorModel::disconnected,
            this, &MonitorPresenter::onModelDisconnected);
    connect(_model, &Models::MonitorModel::dataReceived,
            this, &MonitorPresenter::onModelDataReceived);
    connect(_model, &Models::MonitorModel::errorOccurred,
            this, &MonitorPresenter::onModelErrorOccurred);
    connect(_model, &Models::MonitorModel::connectionStatusChanged,
            this, &MonitorPresenter::onModelConnectionStatusChanged);
    connect(_model, &Models::MonitorModel::serialPortsRefreshed,
            this, &MonitorPresenter::onModelSerialPortsRefreshed);
}

void MonitorPresenter::setAutoSendPanel(Views::Panels::AutoSendPanel* panel)
{
    if (panel) {
        _autoSendPresenter = new AutoSendPresenter(panel, this);
        _autoSendPresenter->init();
    }
}

void MonitorPresenter::onConnectButtonClicked(Services::Comm::CommProtocol protocol)
{
    _model->setProtocol(protocol);
    _model->setConfig(_currentConfig);

    bool success = _model->connectComm();
    if (!success) {
        emit errorOccurred(_model->getErrorString());
    }
}

void MonitorPresenter::onDisconnectButtonClicked()
{
    _model->disconnectComm();
}

void MonitorPresenter::onSendData(const QString& data, bool hexMode, bool appendNewLine)
{
    QByteArray sendData;

    if (hexMode) {
        QString hexData = QString(data).replace(" ", "");
        for (int i = 0; i < hexData.size(); i += 2) {
            QString hexByte = hexData.mid(i, 2);
            bool ok;
            uchar byte = hexByte.toUInt(&ok, 16);
            if (ok) {
                sendData.append(byte);
            }
        }
    } else {
        sendData = data.toUtf8();
        if (appendNewLine) {
            sendData.append("\r\n");
        }
    }

    qint64 bytesSent = _model->sendData(sendData);
    if (bytesSent < 0) {
        emit errorOccurred("Failed to send data");
    }
}

void MonitorPresenter::onRefreshSerialPorts()
{
    _model->refreshSerialPorts();
}

void MonitorPresenter::onProtocolChanged(Services::Comm::CommProtocol protocol)
{
    if (_model->isConnected()) {
        _model->disconnectComm();
    }
    _model->setProtocol(protocol);
}

void MonitorPresenter::onHexDisplayModeChanged(bool enabled)
{
    _hexDisplayMode = enabled;
}

void MonitorPresenter::onAppendNewLineChanged(bool enabled)
{
    _appendNewLineEnabled = enabled;
}

void MonitorPresenter::updateSerialConfig(const QString& port, int baudRate, int dataBits,
                                        const QString& parity, int stopBits)
{
    _currentConfig.protocolType = Services::Comm::protocolToString(Services::Comm::CommProtocol::Serial);
    _currentConfig.serialPort = port;
    _currentConfig.baudRate = baudRate;
    _currentConfig.dataBits = dataBits;
    _currentConfig.stopBits = stopBits;

    if (parity == "Odd") {
        _currentConfig.parity = "1";
    } else if (parity == "Even") {
        _currentConfig.parity = "2";
    } else if (parity == "Mark") {
        _currentConfig.parity = "3";
    } else if (parity == "Space") {
        _currentConfig.parity = "4";
    } else {
        _currentConfig.parity = "None";
    }
}

void MonitorPresenter::updateTcpClientConfig(const QString& ip, int port, bool autoReconnect)
{
    _currentConfig.protocolType = Services::Comm::protocolToString(Services::Comm::CommProtocol::TcpClient);
    _currentConfig.tcpHost = ip;
    _currentConfig.tcpPort = port;
    // _currentConfig.autoReconnect = autoReconnect;
}

void MonitorPresenter::updateTcpServerConfig(int port)
{
    _currentConfig.protocolType = Services::Comm::protocolToString(Services::Comm::CommProtocol::TcpServer);
    _currentConfig.tcpPort = port;
}

void MonitorPresenter::updateUdpConfig(int localPort, const QString& targetIp, bool broadcast)
{
    _currentConfig.protocolType = Services::Comm::protocolToString(Services::Comm::CommProtocol::Udp);
    _currentConfig.udpLocalPort = localPort;
    _currentConfig.udpLocalAddr = targetIp;
    _currentConfig.isBroadcast = broadcast;
}

void MonitorPresenter::onModelConnected()
{
    QString protocolName = Services::Comm::protocolToString(_model->getProtocol());
    emit connectionStatusChanged(true, protocolName);
}

void MonitorPresenter::onModelDisconnected()
{
    QString protocolName = Services::Comm::protocolToString(_model->getProtocol());
    emit connectionStatusChanged(false, protocolName);
}

void MonitorPresenter::onModelDataReceived(const QByteArray& data)
{
    QString formattedData = formatData(data, _hexDisplayMode);
    emit dataReceived(formattedData, _appendNewLineEnabled);
}

void MonitorPresenter::onModelErrorOccurred(const QString& error)
{
    emit errorOccurred(error);
}

void MonitorPresenter::onModelConnectionStatusChanged(bool connected)
{
    QString protocolName = Services::Comm::protocolToString(_model->getProtocol());
    emit connectionStatusChanged(connected, protocolName);

    // 更新自动发送面板的连接状态
    if (_autoSendPresenter) {
        _autoSendPresenter->setConnectionStatus(connected);
    }
}

void MonitorPresenter::onModelSerialPortsRefreshed(const QStringList& ports)
{
    emit serialPortsUpdated(ports);
}

QString MonitorPresenter::formatData(const QByteArray& data, bool hexMode)
{
    if (hexMode) {
        return data.toHex(' ').toUpper();
    }
    return QString::fromUtf8(data);
}

}
