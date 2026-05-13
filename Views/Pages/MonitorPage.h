#ifndef MONITORPAGE_H
#define MONITORPAGE_H

#include "ElaScrollPage.h"
#include "ElaTabWidget.h"
#include "ElaPlainTextEdit.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaSpinBox.h"
#include "ElaCheckBox.h"
#include "Widgets/ToggleSwitch.h"
#include "Widgets/ComboBox.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>

namespace Presenters {
class MonitorPresenter;
}

namespace Services {
namespace Comm {
enum class CommProtocol;
}
}

namespace Views {
namespace Pages {

class MonitorPage : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit MonitorPage(QWidget* parent = nullptr);
    ~MonitorPage() override;

    void setPresenter(Presenters::MonitorPresenter* presenter);
    Presenters::MonitorPresenter* getPresenter() const;

    void appendReceivedData(const QString& data);
    void updateConnectionStatus(bool connected, const QString& protocolName);
    void updateSerialPorts(const QStringList& ports);
    void showError(const QString& error);

    Widgets::ComboBox* getSerialPortCombo() const { return _serialPortCombo; }
    Widgets::ComboBox* getBaudRateCombo() const { return _baudRateCombo; }
    Widgets::ComboBox* getDataBitsCombo() const { return _dataBitsCombo; }
    Widgets::ComboBox* getParityCombo() const { return _parityCombo; }
    Widgets::ComboBox* getStopBitsCombo() const { return _stopBitsCombo; }
    ElaPushButton* getSerialConnectButton() const { return _serialConnectButton; }

    ElaLineEdit* getTcpServerIpEdit() const { return _tcpServerIpEdit; }
    ElaSpinBox* getTcpServerPortSpin() const { return _tcpServerPortSpin; }
    ElaPushButton* getTcpClientConnectButton() const { return _tcpClientConnectButton; }
    ElaCheckBox* getTcpAutoReconnectBox() const { return _tcpAutoReconnectBox; }

    ElaSpinBox* getTcpListenPortSpin() const { return _tcpListenPortSpin; }
    ElaPushButton* getTcpServerStartButton() const { return _tcpServerStartButton; }
    ElaLineEdit* getTcpClientListEdit() const { return _tcpClientListEdit; }

    ElaSpinBox* getUdpLocalPortSpin() const { return _udpLocalPortSpin; }
    ElaLineEdit* getUdpTargetIpEdit() const { return _udpTargetIpEdit; }
    ElaPushButton* getUdpBindButton() const { return _udpBindButton; }
    ElaCheckBox* getUdpBroadcastBox() const { return _udpBroadcastBox; }

signals:
    void connectButtonClicked(Services::Comm::CommProtocol protocol);
    void disconnectButtonClicked();
    void sendData(const QString& data, bool hexMode, bool appendNewLine);
    void refreshSerialPorts();
    void protocolChanged(Services::Comm::CommProtocol protocol);
    void hexDisplayModeChanged(bool enabled);

private slots:
    void onHexDisplaySwitchToggled(bool checked);

protected:
    void changeEvent(QEvent* event) override;

private:
    void initUI();
    void initCommConfigTab();
    void initReceiveArea();
    void initSendArea();
    void initConnections();
    void retranslateUi();

    void updateConnectButtonState(bool connected);
    QString formatDataWithTimestamp(const QString& data);
    void setAutoNewLineEnabled(bool enabled);
    void appendDataWithoutAutoNewLine(const QString& data);

    Presenters::MonitorPresenter* _presenter;

    ElaTabWidget* _commConfigTabWidget;
    ElaPlainTextEdit* _receiveTextEdit;
    ElaLineEdit* _sendLineEdit;
    ElaPushButton* _sendButton;
    ElaPushButton* _clearReceiveButton;
    ElaPushButton* _clearSendButton;

    Widgets::ToggleSwitch* _hexDisplaySwitch;
    Widgets::ToggleSwitch* _timestampSwitch;
    Widgets::ToggleSwitch* _autoScrollSwitch;
    Widgets::ToggleSwitch* _hexSendSwitch;
    Widgets::ToggleSwitch* _appendNewLineSwitch;

    QWidget* _serialConfigWidget;
    QWidget* _tcpClientConfigWidget;
    QWidget* _tcpServerConfigWidget;
    QWidget* _udpConfigWidget;

    Widgets::ComboBox* _serialPortCombo;
    Widgets::ComboBox* _baudRateCombo;
    Widgets::ComboBox* _dataBitsCombo;
    Widgets::ComboBox* _parityCombo;
    Widgets::ComboBox* _stopBitsCombo;
    ElaPushButton* _serialConnectButton;

    ElaLineEdit* _tcpServerIpEdit;
    ElaSpinBox* _tcpServerPortSpin;
    ElaPushButton* _tcpClientConnectButton;
    ElaCheckBox* _tcpAutoReconnectBox;

    ElaSpinBox* _tcpListenPortSpin;
    ElaPushButton* _tcpServerStartButton;
    ElaLineEdit* _tcpClientListEdit;

    ElaSpinBox* _udpLocalPortSpin;
    ElaLineEdit* _udpTargetIpEdit;
    ElaPushButton* _udpBindButton;
    ElaCheckBox* _udpBroadcastBox;
    ElaCheckBox* _udpMulticastBox;

    ElaText* _hexDisplayLabel;
    ElaText* _timestampLabel;
    ElaText* _autoScrollLabel;
    ElaText* _hexSendLabel;
    ElaText* _newLineLabel;
    ElaText* _portLabel;
    ElaText* _baudLabel;
    ElaText* _dataLabel;
    ElaText* _parityLabel;
    ElaText* _stopLabel;
    ElaText* _ipLabel;
    ElaText* _portLabel2;
    ElaText* _listenPortLabel;
    ElaText* _clientListLabel;
    ElaText* _localPortLabel;
    ElaText* _targetIpLabel;

    bool _isConnected;
    bool _autoNewLineEnabled;
};

}
}

#endif // MONITORPAGE_H
