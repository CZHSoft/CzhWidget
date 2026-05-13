#include "Views/Pages/MonitorPage.h"
#include "Presenters/MonitorPresenter.h"
#include "Services/Comm/CommProtocol.h"
#include "ElaText.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>

namespace Views {
namespace Pages {

MonitorPage::MonitorPage(QWidget* parent)
    : ElaScrollPage(parent)
    , _commConfigTabWidget(nullptr)
    , _receiveTextEdit(nullptr)
    , _sendLineEdit(nullptr)
    , _sendButton(nullptr)
    , _clearReceiveButton(nullptr)
    , _clearSendButton(nullptr)
    , _hexDisplaySwitch(nullptr)
    , _timestampSwitch(nullptr)
    , _autoScrollSwitch(nullptr)
    , _hexSendSwitch(nullptr)
    , _appendNewLineSwitch(nullptr)
    , _serialConfigWidget(nullptr)
    , _tcpClientConfigWidget(nullptr)
    , _tcpServerConfigWidget(nullptr)
    , _udpConfigWidget(nullptr)
    , _serialPortCombo(nullptr)
    , _baudRateCombo(nullptr)
    , _dataBitsCombo(nullptr)
    , _parityCombo(nullptr)
    , _stopBitsCombo(nullptr)
    , _serialConnectButton(nullptr)
    , _tcpServerIpEdit(nullptr)
    , _tcpServerPortSpin(nullptr)
    , _tcpClientConnectButton(nullptr)
    , _tcpAutoReconnectBox(nullptr)
    , _tcpListenPortSpin(nullptr)
    , _tcpServerStartButton(nullptr)
    , _tcpClientListEdit(nullptr)
    , _udpLocalPortSpin(nullptr)
    , _udpTargetIpEdit(nullptr)
    , _udpBindButton(nullptr)
    , _udpBroadcastBox(nullptr)
    , _presenter(nullptr)
    , _isConnected(false)
    , _autoNewLineEnabled(true)
{
    setWindowTitle(tr("Monitor"));
    setTitleVisible(false);

    initUI();
    initConnections();
}

MonitorPage::~MonitorPage()
{
}

void MonitorPage::setPresenter(Presenters::MonitorPresenter* presenter)
{
    _presenter = presenter;
}

Presenters::MonitorPresenter* MonitorPage::getPresenter() const
{
    return _presenter;
}

void MonitorPage::initUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    initCommConfigTab();
    mainLayout->addWidget(_commConfigTabWidget);

    QWidget* receiveWidget = new QWidget(this);
    QVBoxLayout* receiveLayout = new QVBoxLayout(receiveWidget);
    receiveLayout->setContentsMargins(0, 0, 0, 0);
    receiveLayout->setSpacing(0);

    QWidget* toolbarWidget = new QWidget(this);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbarWidget);
    toolbarLayout->setContentsMargins(5, 5, 5, 5);
    toolbarLayout->setSpacing(20);

    QWidget* hexGroup = new QWidget(this);
    QHBoxLayout* hexLayout = new QHBoxLayout(hexGroup);
    hexLayout->setContentsMargins(0, 0, 0, 0);
    hexLayout->setSpacing(8);
    _hexDisplayLabel = new ElaText(tr("HEX Display"), this);
    _hexDisplayLabel->setTextPixelSize(13);

    _hexDisplaySwitch = new Widgets::ToggleSwitch(this);
    hexLayout->addWidget(_hexDisplayLabel);
    hexLayout->addWidget(_hexDisplaySwitch);
    toolbarLayout->addWidget(hexGroup);

    QWidget* timestampGroup = new QWidget(this);
    QHBoxLayout* timestampLayout = new QHBoxLayout(timestampGroup);
    timestampLayout->setContentsMargins(0, 0, 0, 0);
    timestampLayout->setSpacing(8);
    _timestampLabel = new ElaText(tr("Timestamp"), this);
    _timestampLabel->setTextPixelSize(13);
    _timestampSwitch = new Widgets::ToggleSwitch(this);
    timestampLayout->addWidget(_timestampLabel);
    timestampLayout->addWidget(_timestampSwitch);
    toolbarLayout->addWidget(timestampGroup);

    QWidget* autoScrollGroup = new QWidget(this);
    QHBoxLayout* autoScrollLayout = new QHBoxLayout(autoScrollGroup);
    autoScrollLayout->setContentsMargins(0, 0, 0, 0);
    autoScrollLayout->setSpacing(8);
    _autoScrollLabel = new ElaText(tr("Auto Scroll"), this);
    _autoScrollLabel->setTextPixelSize(13);
    _autoScrollSwitch = new Widgets::ToggleSwitch(this);
    _autoScrollSwitch->setIsToggled(true);
    autoScrollLayout->addWidget(_autoScrollLabel);
    autoScrollLayout->addWidget(_autoScrollSwitch);
    toolbarLayout->addWidget(autoScrollGroup);

    toolbarLayout->addStretch();

    _clearReceiveButton = new ElaPushButton(tr("Clear Receive"), this);
    toolbarLayout->addWidget(_clearReceiveButton);

    receiveLayout->addWidget(toolbarWidget);

    _receiveTextEdit = new ElaPlainTextEdit(this);
    _receiveTextEdit->setReadOnly(true);
    _receiveTextEdit->setPlaceholderText(tr("Received data will be displayed here..."));
    _receiveTextEdit->setMinimumHeight(250);
    receiveLayout->addWidget(_receiveTextEdit);

    mainLayout->addWidget(receiveWidget);

    QWidget* sendWidget = new QWidget(this);
    QVBoxLayout* sendLayout = new QVBoxLayout(sendWidget);
    sendLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* sendConfigWidget = new QWidget(this);
    QHBoxLayout* sendConfigLayout = new QHBoxLayout(sendConfigWidget);
    sendConfigLayout->setContentsMargins(0, 0, 0, 0);
    sendConfigLayout->setSpacing(15);

    _hexSendLabel = new ElaText(tr("HEX Send"), this);
    _hexSendLabel->setTextPixelSize(13);
    _hexSendSwitch = new Widgets::ToggleSwitch(this);
    sendConfigLayout->addWidget(_hexSendLabel);
    sendConfigLayout->addWidget(_hexSendSwitch);

    _newLineLabel = new ElaText(tr("Append \\r\\n"), this);
    _newLineLabel->setTextPixelSize(13);
    _appendNewLineSwitch = new Widgets::ToggleSwitch(this);
    sendConfigLayout->addWidget(_newLineLabel);
    sendConfigLayout->addWidget(_appendNewLineSwitch);

    _clearSendButton = new ElaPushButton(tr("Clear Input"), this);
    sendConfigLayout->addWidget(_clearSendButton);

    sendConfigLayout->addStretch();
    sendLayout->addWidget(sendConfigWidget);

    QHBoxLayout* sendInputLayout = new QHBoxLayout();
    sendInputLayout->setContentsMargins(0, 5, 0, 0);
    sendInputLayout->setSpacing(10);

    _sendLineEdit = new ElaLineEdit(this);
    _sendLineEdit->setPlaceholderText(tr("Enter data to send..."));
    sendInputLayout->addWidget(_sendLineEdit);

    _sendButton = new ElaPushButton(tr("Send"), this);
    _sendButton->setFixedWidth(80);
    sendInputLayout->addWidget(_sendButton);

    sendLayout->addLayout(sendInputLayout);
    mainLayout->addWidget(sendWidget);

    addCentralWidget(centralWidget, true, false, 0);
}

void MonitorPage::initCommConfigTab()
{
    _commConfigTabWidget = new ElaTabWidget(this);
    _commConfigTabWidget->setFixedHeight(120);
    _commConfigTabWidget->setIsTabTransparent(true);
    _commConfigTabWidget->tabBar()->setTabsClosable(false);
    _commConfigTabWidget->setTabSize(QSize(120, 35));

    _serialConfigWidget = new QWidget(this);
    QGridLayout* serialLayout = new QGridLayout(_serialConfigWidget);
    serialLayout->setContentsMargins(10, 10, 10, 10);
    serialLayout->setSpacing(10);

    _portLabel = new ElaText(tr("Port:"), this);
    _portLabel->setTextPixelSize(13);
    _serialPortCombo = new Widgets::ComboBox(this);
    _serialPortCombo->addItems({"COM1", "COM2", "COM3", "COM4", "COM5", "COM6"});
    serialLayout->addWidget(_portLabel, 0, 0);
    serialLayout->addWidget(_serialPortCombo, 0, 1);

    _baudLabel = new ElaText(tr("Baud Rate:"), this);
    _baudLabel->setTextPixelSize(13);
    _baudRateCombo = new Widgets::ComboBox(this);
    _baudRateCombo->addItems({"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"});
    _baudRateCombo->setCurrentText("115200");
    serialLayout->addWidget(_baudLabel, 0, 2);
    serialLayout->addWidget(_baudRateCombo, 0, 3);

    _dataLabel = new ElaText(tr("Data Bits:"), this);
    _dataLabel->setTextPixelSize(13);
    _dataBitsCombo = new Widgets::ComboBox(this);
    _dataBitsCombo->addItems({"5", "6", "7", "8"});
    _dataBitsCombo->setCurrentText("8");
    serialLayout->addWidget(_dataLabel, 0, 4);
    serialLayout->addWidget(_dataBitsCombo, 0, 5);

    _parityLabel = new ElaText(tr("Parity:"), this);
    _parityLabel->setTextPixelSize(13);
    _parityCombo = new Widgets::ComboBox(this);
    _parityCombo->addItems({tr("None"), tr("Odd"), tr("Even"), tr("Mark"), tr("Space")});
    serialLayout->addWidget(_parityLabel, 1, 0);
    serialLayout->addWidget(_parityCombo, 1, 1);

    _stopLabel = new ElaText(tr("Stop Bits:"), this);
    _stopLabel->setTextPixelSize(13);
    _stopBitsCombo = new Widgets::ComboBox(this);
    _stopBitsCombo->addItems({"1", "1.5", "2"});
    _stopBitsCombo->setCurrentText("1");
    serialLayout->addWidget(_stopLabel, 1, 2);
    serialLayout->addWidget(_stopBitsCombo, 1, 3);

    _serialConnectButton = new ElaPushButton(tr("Open Serial"), this);
    _serialConnectButton->setFixedWidth(100);
    serialLayout->addWidget(_serialConnectButton, 1, 5);

    _commConfigTabWidget->addTab(_serialConfigWidget, tr("Serial Port"));

    _tcpClientConfigWidget = new QWidget(this);
    QGridLayout* tcpClientLayout = new QGridLayout(_tcpClientConfigWidget);
    tcpClientLayout->setContentsMargins(10, 10, 10, 10);
    tcpClientLayout->setSpacing(10);

    _ipLabel = new ElaText(tr("Server IP:"), this);
    _ipLabel->setTextPixelSize(13);
    _tcpServerIpEdit = new ElaLineEdit(this);
    _tcpServerIpEdit->setText("127.0.0.1");
    tcpClientLayout->addWidget(_ipLabel, 0, 0);
    tcpClientLayout->addWidget(_tcpServerIpEdit, 0, 1);

    _portLabel2 = new ElaText(tr("Port:"), this);
    _portLabel2->setTextPixelSize(13);
    _tcpServerPortSpin = new ElaSpinBox(this);
    _tcpServerPortSpin->setRange(1, 65535);
    _tcpServerPortSpin->setValue(8080);
    tcpClientLayout->addWidget(_portLabel2, 0, 2);
    tcpClientLayout->addWidget(_tcpServerPortSpin, 0, 3);

    _tcpClientConnectButton = new ElaPushButton(tr("Connect"), this);
    _tcpClientConnectButton->setFixedWidth(100);
    tcpClientLayout->addWidget(_tcpClientConnectButton, 0, 4);

    _tcpAutoReconnectBox = new ElaCheckBox(tr("Auto Reconnect"), this);
    tcpClientLayout->addWidget(_tcpAutoReconnectBox, 1, 0, 1, 2);

    _commConfigTabWidget->addTab(_tcpClientConfigWidget, tr("TCP Client"));

    _tcpServerConfigWidget = new QWidget(this);
    QGridLayout* tcpServerLayout = new QGridLayout(_tcpServerConfigWidget);
    tcpServerLayout->setContentsMargins(10, 10, 10, 10);
    tcpServerLayout->setSpacing(10);

    _listenPortLabel = new ElaText(tr("Listen Port:"), this);
    _listenPortLabel->setTextPixelSize(13);
    _tcpListenPortSpin = new ElaSpinBox(this);
    _tcpListenPortSpin->setRange(1, 65535);
    _tcpListenPortSpin->setValue(8080);
    tcpServerLayout->addWidget(_listenPortLabel, 0, 0);
    tcpServerLayout->addWidget(_tcpListenPortSpin, 0, 1);

    _tcpServerStartButton = new ElaPushButton(tr("Start Listening"), this);
    _tcpServerStartButton->setFixedWidth(100);
    tcpServerLayout->addWidget(_tcpServerStartButton, 0, 2);

    _clientListLabel = new ElaText(tr("Online Clients:"), this);
    _clientListLabel->setTextPixelSize(13);
    tcpServerLayout->addWidget(_clientListLabel, 1, 0);
    _tcpClientListEdit = new ElaLineEdit(this);
    _tcpClientListEdit->setReadOnly(true);
    _tcpClientListEdit->setText(tr("No clients"));
    tcpServerLayout->addWidget(_tcpClientListEdit, 1, 1, 1, 3);

    _commConfigTabWidget->addTab(_tcpServerConfigWidget, tr("TCP Server"));

    _udpConfigWidget = new QWidget(this);
    QGridLayout* udpLayout = new QGridLayout(_udpConfigWidget);
    udpLayout->setContentsMargins(10, 10, 10, 10);
    udpLayout->setSpacing(10);

    _localPortLabel = new ElaText(tr("Local Port:"), this);
    _localPortLabel->setTextPixelSize(13);
    _udpLocalPortSpin = new ElaSpinBox(this);
    _udpLocalPortSpin->setRange(1, 65535);
    _udpLocalPortSpin->setValue(5000);
    udpLayout->addWidget(_localPortLabel, 0, 0);
    udpLayout->addWidget(_udpLocalPortSpin, 0, 1);

    _targetIpLabel = new ElaText(tr("Target IP:"), this);
    _targetIpLabel->setTextPixelSize(13);
    _udpTargetIpEdit = new ElaLineEdit(this);
    _udpTargetIpEdit->setText("127.0.0.1");
    udpLayout->addWidget(_targetIpLabel, 0, 2);
    udpLayout->addWidget(_udpTargetIpEdit, 0, 3);

    _udpBindButton = new ElaPushButton(tr("Bind Port"), this);
    _udpBindButton->setFixedWidth(100);
    udpLayout->addWidget(_udpBindButton, 0, 4);

    _udpBroadcastBox = new ElaCheckBox(tr("Allow Broadcast"), this);
    _udpMulticastBox = new ElaCheckBox(tr("Allow Multicast"), this);
    udpLayout->addWidget(_udpBroadcastBox, 1, 0);
    udpLayout->addWidget(_udpMulticastBox, 1, 1);

    _commConfigTabWidget->addTab(_udpConfigWidget, tr("UDP"));
}

void MonitorPage::initSendArea()
{
}

void MonitorPage::initConnections()
{
    connect(_hexDisplaySwitch, &Widgets::ToggleSwitch::toggled, this, &MonitorPage::onHexDisplaySwitchToggled);

    connect(_sendButton, &ElaPushButton::clicked, this, [this]() {
        QString data = _sendLineEdit->text();
        bool hexMode = _hexSendSwitch->getIsToggled();
        bool appendNewLine = _appendNewLineSwitch->getIsToggled();
        emit sendData(data, hexMode, appendNewLine);
    });

    connect(_sendLineEdit, &ElaLineEdit::returnPressed, this, [this]() {
        QString data = _sendLineEdit->text();
        bool hexMode = _hexSendSwitch->getIsToggled();
        bool appendNewLine = _appendNewLineSwitch->getIsToggled();
        emit sendData(data, hexMode, appendNewLine);
    });

    connect(_clearReceiveButton, &ElaPushButton::clicked, this, [this]() {
        _receiveTextEdit->clear();
    });

    connect(_clearSendButton, &ElaPushButton::clicked, this, [this]() {
        _sendLineEdit->clear();
    });

    connect(_serialConnectButton, &ElaPushButton::clicked, this, [this]() {
        if (_isConnected) {
            emit disconnectButtonClicked();
        } else {
            emit connectButtonClicked(Services::Comm::CommProtocol::Serial);
        }
    });

    connect(_tcpClientConnectButton, &ElaPushButton::clicked, this, [this]() {
        if (_isConnected) {
            emit disconnectButtonClicked();
        } else {
            emit connectButtonClicked(Services::Comm::CommProtocol::TcpClient);
        }
    });

    connect(_tcpServerStartButton, &ElaPushButton::clicked, this, [this]() {
        if (_isConnected) {
            emit disconnectButtonClicked();
        } else {
            emit connectButtonClicked(Services::Comm::CommProtocol::TcpServer);
        }
    });

    connect(_udpBindButton, &ElaPushButton::clicked, this, [this]() {
        if (_isConnected) {
            emit disconnectButtonClicked();
        } else {
            emit connectButtonClicked(Services::Comm::CommProtocol::Udp);
        }
    });

    connect(_commConfigTabWidget, &ElaTabWidget::currentChanged, this, [this](int index) {
        if (_isConnected) {
            emit disconnectButtonClicked();
        }
        switch (index) {
        case 0:
            emit protocolChanged(Services::Comm::CommProtocol::Serial);
            break;
        case 1:
            emit protocolChanged(Services::Comm::CommProtocol::TcpClient);
            break;
        case 2:
            emit protocolChanged(Services::Comm::CommProtocol::TcpServer);
            break;
        case 3:
            emit protocolChanged(Services::Comm::CommProtocol::Udp);
            break;
        }
    });
}

void MonitorPage::onHexDisplaySwitchToggled(bool checked)
{
    qDebug() << "Hex display toggled:" << checked;
    if (_presenter) {
        _presenter->onHexDisplayModeChanged(checked);
    }
    emit hexDisplayModeChanged(checked);
}

void MonitorPage::appendReceivedData(const QString& data)
{
    QString displayData = data;
    if (_timestampSwitch->getIsToggled()) {
        displayData = formatDataWithTimestamp(data);
    }

    if (_autoNewLineEnabled) {
        _receiveTextEdit->appendPlainText(displayData);
    } else {
        QTextCursor cursor = _receiveTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(displayData);
        _receiveTextEdit->setTextCursor(cursor);
    }

    if (_autoScrollSwitch->getIsToggled()) {
        _receiveTextEdit->moveCursor(QTextCursor::End);
    }
}

void MonitorPage::updateConnectionStatus(bool connected, const QString& protocolName)
{
    _isConnected = connected;
    updateConnectButtonState(connected);
}

void MonitorPage::updateSerialPorts(const QStringList& ports)
{
    _serialPortCombo->clear();
    if (ports.isEmpty()) {
        _serialPortCombo->addItem(tr("No serial ports detected"));
        _serialPortCombo->setEnabled(false);
    } else {
        _serialPortCombo->addItems(ports);
        _serialPortCombo->setEnabled(true);
    }
}

void MonitorPage::showError(const QString& error)
{
    QMessageBox::warning(this, tr("Communication Error"), error);
}

void MonitorPage::updateConnectButtonState(bool connected)
{
    QString connectStyle = "QPushButton { background-color: #4CAF50; color: white; }";
    QString disconnectStyle = "QPushButton { background-color: #f44336; color: white; }";

    if (connected) {
        _serialConnectButton->setText(tr("Close Serial"));
        _serialConnectButton->setStyleSheet(disconnectStyle);

        _tcpClientConnectButton->setText(tr("Disconnect"));
        _tcpClientConnectButton->setStyleSheet(disconnectStyle);

        _tcpServerStartButton->setText(tr("Stop Listening"));
        _tcpServerStartButton->setStyleSheet(disconnectStyle);

        _udpBindButton->setText(tr("Unbind Port"));
        _udpBindButton->setStyleSheet(disconnectStyle);

        _serialPortCombo->setEnabled(false);
        _baudRateCombo->setEnabled(false);
        _dataBitsCombo->setEnabled(false);
        _parityCombo->setEnabled(false);
        _stopBitsCombo->setEnabled(false);

        _tcpServerIpEdit->setEnabled(false);
        _tcpServerPortSpin->setEnabled(false);

        _tcpListenPortSpin->setEnabled(false);

        _udpLocalPortSpin->setEnabled(false);
        _udpTargetIpEdit->setEnabled(false);

        _sendButton->setEnabled(true);
    } else {
        _serialConnectButton->setText(tr("Open Serial"));
        _serialConnectButton->setStyleSheet(connectStyle);

        _tcpClientConnectButton->setText(tr("Connect"));
        _tcpClientConnectButton->setStyleSheet(connectStyle);

        _tcpServerStartButton->setText(tr("Start Listening"));
        _tcpServerStartButton->setStyleSheet(connectStyle);

        _udpBindButton->setText(tr("Bind Port"));
        _udpBindButton->setStyleSheet(connectStyle);

        _serialPortCombo->setEnabled(true);
        _baudRateCombo->setEnabled(true);
        _dataBitsCombo->setEnabled(true);
        _parityCombo->setEnabled(true);
        _stopBitsCombo->setEnabled(true);

        _tcpServerIpEdit->setEnabled(true);
        _tcpServerPortSpin->setEnabled(true);

        _tcpListenPortSpin->setEnabled(true);

        _udpLocalPortSpin->setEnabled(true);
        _udpTargetIpEdit->setEnabled(true);

        _sendButton->setEnabled(false);
    }
}

QString MonitorPage::formatDataWithTimestamp(const QString& data)
{
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss.zzz] ");
    return timestamp + data;
}

void MonitorPage::setAutoNewLineEnabled(bool enabled)
{
    _autoNewLineEnabled = enabled;
}

void MonitorPage::appendDataWithoutAutoNewLine(const QString& data)
{
    QTextCursor cursor = _receiveTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(data);
    _receiveTextEdit->setTextCursor(cursor);

    if (_autoScrollSwitch->getIsToggled()) {
        _receiveTextEdit->moveCursor(QTextCursor::End);
    }
}

void MonitorPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    ElaScrollPage::changeEvent(event);
}

void MonitorPage::retranslateUi()
{
    setWindowTitle(tr("Monitor"));
    
    if (_hexDisplayLabel) {
        _hexDisplayLabel->setText(tr("HEX Display"));
    }
    if (_timestampLabel) {
        _timestampLabel->setText(tr("Timestamp"));
    }
    if (_autoScrollLabel) {
        _autoScrollLabel->setText(tr("Auto Scroll"));
    }
    if (_clearReceiveButton) {
        _clearReceiveButton->setText(tr("Clear Receive"));
    }
    if (_receiveTextEdit) {
        _receiveTextEdit->setPlaceholderText(tr("Received data will be displayed here..."));
    }
    if (_hexSendLabel) {
        _hexSendLabel->setText(tr("HEX Send"));
    }
    if (_newLineLabel) {
        _newLineLabel->setText(tr("Append \\r\\n"));
    }
    if (_clearSendButton) {
        _clearSendButton->setText(tr("Clear Input"));
    }
    if (_sendLineEdit) {
        _sendLineEdit->setPlaceholderText(tr("Enter data to send..."));
    }
    if (_sendButton) {
        _sendButton->setText(tr("Send"));
    }
    
    if (_portLabel) {
        _portLabel->setText(tr("Port:"));
    }
    if (_baudLabel) {
        _baudLabel->setText(tr("Baud Rate:"));
    }
    if (_dataLabel) {
        _dataLabel->setText(tr("Data Bits:"));
    }
    if (_parityLabel) {
        _parityLabel->setText(tr("Parity:"));
    }
    if (_stopLabel) {
        _stopLabel->setText(tr("Stop Bits:"));
    }
    if (_parityCombo) {
        _parityCombo->clear();
        _parityCombo->addItems({tr("None"), tr("Odd"), tr("Even"), tr("Mark"), tr("Space")});
    }
    
    if (_ipLabel) {
        _ipLabel->setText(tr("Server IP:"));
    }
    if (_portLabel2) {
        _portLabel2->setText(tr("Port:"));
    }
    if (_tcpAutoReconnectBox) {
        _tcpAutoReconnectBox->setText(tr("Auto Reconnect"));
    }
    
    if (_listenPortLabel) {
        _listenPortLabel->setText(tr("Listen Port:"));
    }
    if (_clientListLabel) {
        _clientListLabel->setText(tr("Online Clients:"));
    }
    if (_tcpClientListEdit) {
        _tcpClientListEdit->setText(tr("No clients"));
    }
    
    if (_localPortLabel) {
        _localPortLabel->setText(tr("Local Port:"));
    }
    if (_targetIpLabel) {
        _targetIpLabel->setText(tr("Target IP:"));
    }
    if (_udpBroadcastBox) {
        _udpBroadcastBox->setText(tr("Allow Broadcast"));
    }
    if(_udpMulticastBox) {
        _udpMulticastBox->setText(tr("Allow Multicast"));
    }

    if (_commConfigTabWidget) {
        _commConfigTabWidget->setTabText(0, tr("Serial Port"));
        _commConfigTabWidget->setTabText(1, tr("TCP Client"));
        _commConfigTabWidget->setTabText(2, tr("TCP Server"));
        _commConfigTabWidget->setTabText(3, tr("UDP"));
    }
    
    updateConnectButtonState(_isConnected);
}

}
}
