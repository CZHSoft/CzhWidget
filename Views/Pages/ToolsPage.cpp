#include "Views/Pages/ToolsPage.h"
#include "ElaText.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStandardItemModel>

namespace Views {
namespace Pages {

ToolsPage::ToolsPage(QWidget* parent)
    : ElaScrollPage(parent)
    , _tabWidget(nullptr)
    , _themeComboBox(nullptr)
    , _fontSizeComboBox(nullptr)
    , _logPathEdit(nullptr)
    , _autoSaveSwitch(nullptr)
    , _commandListView(nullptr)
    , _commandNameEdit(nullptr)
    , _commandDataEdit(nullptr)
    , _addCommandButton(nullptr)
    , _deleteCommandButton(nullptr)
    , _importCommandButton(nullptr)
    , _exportCommandButton(nullptr)
    , _portScanEdit(nullptr)
    , _scanButton(nullptr)
    , _scanResultEdit(nullptr)
    , _crcInputEdit(nullptr)
    , _crcTypeComboBox(nullptr)
    , _calculateButton(nullptr)
    , _crcResultEdit(nullptr)
    , _convertInputEdit(nullptr)
    , _convertFromComboBox(nullptr)
    , _convertToComboBox(nullptr)
    , _convertButton(nullptr)
    , _convertResultEdit(nullptr)
    , _themeLabel(nullptr)
    , _fontSizeLabel(nullptr)
    , _logPathLabel(nullptr)
    , _autoSaveLabel(nullptr)
    , _listLabel(nullptr)
    , _editLabel(nullptr)
    , _nameLabel(nullptr)
    , _dataLabel(nullptr)
    , _portLabel(nullptr)
    , _crcInputLabel(nullptr)
    , _crcResultLabel(nullptr)
    , _convertInputLabel(nullptr)
    , _fromLabel(nullptr)
    , _toLabel(nullptr)
    , _convertResultLabel(nullptr)
{
    setWindowTitle(tr("Tools"));
    setTitleVisible(false);

    initUI();
}

ToolsPage::~ToolsPage()
{
}

void ToolsPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    ElaScrollPage::changeEvent(event);
}

void ToolsPage::retranslateUi()
{
    setWindowTitle(tr("Tools"));

    if (_themeLabel) {
        _themeLabel->setText(tr("Theme:"));
    }
    if (_themeComboBox) {
        _themeComboBox->clear();
        _themeComboBox->addItems({tr("Light Theme"), tr("Dark Theme"), tr("System Theme")});
    }
    if (_fontSizeLabel) {
        _fontSizeLabel->setText(tr("Font Size:"));
    }
    if (_logPathLabel) {
        _logPathLabel->setText(tr("Log Path:"));
    }
    if (_autoSaveLabel) {
        _autoSaveLabel->setText(tr("Auto Save:"));
    }

    if (_listLabel) {
        _listLabel->setText(tr("Command List"));
    }
    if (_editLabel) {
        _editLabel->setText(tr("Command Edit"));
    }
    if (_nameLabel) {
        _nameLabel->setText(tr("Command Name:"));
    }
    if (_dataLabel) {
        _dataLabel->setText(tr("Command Data:"));
    }
    if (_commandNameEdit) {
        _commandNameEdit->setPlaceholderText(tr("Enter command name..."));
    }
    if (_commandDataEdit) {
        _commandDataEdit->setPlaceholderText(tr("Enter HEX data..."));
    }
    if (_addCommandButton) {
        _addCommandButton->setText(tr("Add"));
    }
    if (_deleteCommandButton) {
        _deleteCommandButton->setText(tr("Delete"));
    }
    if (_importCommandButton) {
        _importCommandButton->setText(tr("Import"));
    }
    if (_exportCommandButton) {
        _exportCommandButton->setText(tr("Export"));
    }

    if (_portScanGroupBox) {
        _portScanGroupBox->setTitle(tr("Port Scanner"));
    }
    if (_portLabel) {
        _portLabel->setText(tr("Target IP:"));
    }
    if (_scanButton) {
        _scanButton->setText(tr("Scan"));
    }
    if (_scanResultEdit) {
        _scanResultEdit->setPlaceholderText(tr("Scan results will appear here..."));
    }

    if (_crcGroupBox) {
        _crcGroupBox->setTitle(tr("CRC Calculator"));
    }
    if (_crcInputLabel) {
        _crcInputLabel->setText(tr("Input Data:"));
    }
    if (_crcInputEdit) {
        _crcInputEdit->setPlaceholderText(tr("Enter HEX data..."));
    }
    if (_calculateButton) {
        _calculateButton->setText(tr("Calculate"));
    }
    if (_crcResultLabel) {
        _crcResultLabel->setText(tr("Result:"));
    }

    if (_convertGroupBox) {
        _convertGroupBox->setTitle(tr("Number Converter"));
    }
    if (_convertInputLabel) {
        _convertInputLabel->setText(tr("Input:"));
    }
    if (_convertInputEdit) {
        _convertInputEdit->setPlaceholderText(tr("Enter data..."));
    }
    if (_fromLabel) {
        _fromLabel->setText(tr("From:"));
    }
    if (_convertFromComboBox) {
        _convertFromComboBox->clear();
        _convertFromComboBox->addItems({tr("Decimal"), tr("Hexadecimal"), tr("Binary"), tr("ASCII")});
    }
    if (_toLabel) {
        _toLabel->setText(tr("To:"));
    }
    if (_convertToComboBox) {
        _convertToComboBox->clear();
        _convertToComboBox->addItems({tr("Decimal"), tr("Hexadecimal"), tr("Binary"), tr("ASCII")});
    }
    if (_convertButton) {
        _convertButton->setText(tr("Convert"));
    }
    if (_convertResultLabel) {
        _convertResultLabel->setText(tr("Result:"));
    }

    if (_tabWidget) {
        _tabWidget->setTabText(0, tr("General"));
        _tabWidget->setTabText(1, tr("Commands"));
        _tabWidget->setTabText(2, tr("Tools"));
    }
}

void ToolsPage::initUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    _tabWidget = new ElaTabWidget(this);
    _tabWidget->setIsTabTransparent(true);
    _tabWidget->tabBar()->setTabsClosable(false);
    _tabWidget->setTabSize(QSize(120, 35));

    initGeneralSettingsTab();
    initCommandManagementTab();
    initExtendedToolsTab();

    _tabWidget->removeTab(0);
    _tabWidget->removeTab(0);

    mainLayout->addWidget(_tabWidget);

    addCentralWidget(centralWidget, true, false, 0);
}

void ToolsPage::initGeneralSettingsTab()
{
    QWidget* generalWidget = new QWidget(this);
    QVBoxLayout* generalLayout = new QVBoxLayout(generalWidget);
    generalLayout->setContentsMargins(10, 10, 10, 10);
    generalLayout->setSpacing(15);

    QWidget* themeWidget = new QWidget(this);
    QHBoxLayout* themeLayout = new QHBoxLayout(themeWidget);
    themeLayout->setContentsMargins(0, 0, 0, 0);
    themeLayout->setSpacing(10);

    _themeLabel = new ElaText(tr("Theme:"), this);
    _themeLabel->setFixedWidth(80);
    _themeComboBox = new Widgets::ComboBox(this);
    _themeComboBox->addItems({tr("Light Theme"), tr("Dark Theme"), tr("System Theme")});
    _themeComboBox->setFixedWidth(150);
    themeLayout->addWidget(_themeLabel);
    themeLayout->addWidget(_themeComboBox);
    generalLayout->addWidget(themeWidget);

    QWidget* fontSizeWidget = new QWidget(this);
    QHBoxLayout* fontSizeLayout = new QHBoxLayout(fontSizeWidget);
    fontSizeLayout->setContentsMargins(0, 0, 0, 0);
    fontSizeLayout->setSpacing(10);

    _fontSizeLabel = new ElaText(tr("Font Size:"), this);
    _fontSizeLabel->setFixedWidth(80);
    _fontSizeComboBox = new Widgets::ComboBox(this);
    _fontSizeComboBox->addItems({"12px", "13px", "14px", "15px", "16px", "18px", "20px"});
    _fontSizeComboBox->setFixedWidth(100);
    fontSizeLayout->addWidget(_fontSizeLabel);
    fontSizeLayout->addWidget(_fontSizeComboBox);
    generalLayout->addWidget(fontSizeWidget);

    QWidget* logPathWidget = new QWidget(this);
    QHBoxLayout* logPathLayout = new QHBoxLayout(logPathWidget);
    logPathLayout->setContentsMargins(0, 0, 0, 0);
    logPathLayout->setSpacing(10);

    _logPathLabel = new ElaText(tr("Log Path:"), this);
    _logPathLabel->setFixedWidth(80);
    _logPathEdit = new ElaLineEdit(this);
    _logPathEdit->setReadOnly(true);
    _logPathEdit->setText("./logs/");
    ElaPushButton* browseButton = new ElaPushButton(tr("Browse..."), this);
    browseButton->setFixedWidth(80);
    logPathLayout->addWidget(_logPathLabel);
    logPathLayout->addWidget(_logPathEdit);
    logPathLayout->addWidget(browseButton);
    generalLayout->addWidget(logPathWidget);

    QWidget* autoSaveWidget = new QWidget(this);
    QHBoxLayout* autoSaveLayout = new QHBoxLayout(autoSaveWidget);
    autoSaveLayout->setContentsMargins(0, 0, 0, 0);
    autoSaveLayout->setSpacing(10);

    _autoSaveLabel = new ElaText(tr("Auto Save:"), this);
    _autoSaveLabel->setFixedWidth(100);
    _autoSaveSwitch = new ElaToggleSwitch(this);
    _autoSaveSwitch->setIsToggled(true);
    autoSaveLayout->addWidget(_autoSaveLabel);
    autoSaveLayout->addWidget(_autoSaveSwitch);
    generalLayout->addWidget(autoSaveWidget);

    generalLayout->addStretch();

    _tabWidget->addTab(generalWidget, tr("General"));
}

void ToolsPage::initCommandManagementTab()
{
    QWidget* commandWidget = new QWidget(this);
    QHBoxLayout* commandLayout = new QHBoxLayout(commandWidget);
    commandLayout->setContentsMargins(10, 10, 10, 10);
    commandLayout->setSpacing(15);

    QWidget* listWidget = new QWidget(this);
    QVBoxLayout* listLayout = new QVBoxLayout(listWidget);
    listLayout->setContentsMargins(0, 0, 0, 0);
    listLayout->setSpacing(10);

    _listLabel = new ElaText(tr("Command List"), this);
    listLayout->addWidget(_listLabel);

    _commandListView = new ElaListView(this);
    _commandListView->setFixedWidth(200);
    _commandListView->setMinimumHeight(250);

    QStandardItemModel* model = new QStandardItemModel(this);
    model->appendRow(new QStandardItem(tr("Cmd1: Read Register")));
    model->appendRow(new QStandardItem(tr("Cmd2: Write Register")));
    model->appendRow(new QStandardItem(tr("Cmd3: Get Status")));
    _commandListView->setModel(model);

    listLayout->addWidget(_commandListView);

    QWidget* buttonWidget = new QWidget(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(10);

    _importCommandButton = new ElaPushButton(tr("Import"), this);
    _importCommandButton->setFixedWidth(70);
    _exportCommandButton = new ElaPushButton(tr("Export"), this);
    _exportCommandButton->setFixedWidth(70);
    buttonLayout->addWidget(_importCommandButton);
    buttonLayout->addWidget(_exportCommandButton);
    listLayout->addWidget(buttonWidget);

    commandLayout->addWidget(listWidget);

    QWidget* editWidget = new QWidget(this);
    QVBoxLayout* editLayout = new QVBoxLayout(editWidget);
    editLayout->setContentsMargins(0, 0, 0, 0);
    editLayout->setSpacing(15);

    _editLabel = new ElaText(tr("Command Edit"), this);
    editLayout->addWidget(_editLabel);

    QWidget* nameWidget = new QWidget(this);
    QHBoxLayout* nameLayout = new QHBoxLayout(nameWidget);
    nameLayout->setContentsMargins(0, 0, 0, 0);
    nameLayout->setSpacing(10);

    _nameLabel = new ElaText(tr("Command Name:"), this);
    _nameLabel->setFixedWidth(80);
    _commandNameEdit = new ElaLineEdit(this);
    _commandNameEdit->setPlaceholderText(tr("Enter command name..."));
    nameLayout->addWidget(_nameLabel);
    nameLayout->addWidget(_commandNameEdit);
    editLayout->addWidget(nameWidget);

    QWidget* dataWidget = new QWidget(this);
    QHBoxLayout* dataLayout = new QHBoxLayout(dataWidget);
    dataLayout->setContentsMargins(0, 0, 0, 0);
    dataLayout->setSpacing(10);

    _dataLabel = new ElaText(tr("Command Data:"), this);
    _dataLabel->setFixedWidth(80);
    _commandDataEdit = new ElaLineEdit(this);
    _commandDataEdit->setPlaceholderText(tr("Enter HEX data..."));
    dataLayout->addWidget(_dataLabel);
    dataLayout->addWidget(_commandDataEdit);
    editLayout->addWidget(dataWidget);

    QWidget* editButtonWidget = new QWidget(this);
    QHBoxLayout* editButtonLayout = new QHBoxLayout(editButtonWidget);
    editButtonLayout->setContentsMargins(0, 0, 0, 0);
    editButtonLayout->setSpacing(10);

    _addCommandButton = new ElaPushButton(tr("Add"), this);
    _addCommandButton->setFixedWidth(80);
    _deleteCommandButton = new ElaPushButton(tr("Delete"), this);
    _deleteCommandButton->setFixedWidth(80);
    editButtonLayout->addWidget(_addCommandButton);
    editButtonLayout->addWidget(_deleteCommandButton);
    editLayout->addWidget(editButtonWidget);

    editLayout->addStretch();

    commandLayout->addWidget(editWidget);

    _tabWidget->addTab(commandWidget, tr("Commands"));
}

void ToolsPage::initExtendedToolsTab()
{
    QFont font = this->font();
    font.setPointSize(13);
    this->setFont(font);

    QWidget* toolsWidget = new QWidget(this);
    QVBoxLayout* toolsLayout = new QVBoxLayout(toolsWidget);
    toolsLayout->setContentsMargins(15, 15, 15, 15);
    toolsLayout->setSpacing(20);

    _portScanGroupBox = new Widgets::GroupBox(tr("Port Scanner"), this);
    QVBoxLayout* portScanLayout = new QVBoxLayout(_portScanGroupBox);
    portScanLayout->setContentsMargins(10, 10, 10, 10);
    portScanLayout->setSpacing(10);

    QWidget* portInputWidget = new QWidget(this);
    QHBoxLayout* portInputLayout = new QHBoxLayout(portInputWidget);
    portInputLayout->setContentsMargins(0, 0, 0, 0);
    portInputLayout->setSpacing(10);

    _portLabel = new ElaText(tr("Target IP:"), this);
    _portLabel->setFixedWidth(80);
    _portLabel->setFont(font);
    _portScanEdit = new ElaLineEdit(this);
    _portScanEdit->setText("127.0.0.1");
    _portScanEdit->setFont(font);
    _scanButton = new ElaPushButton(tr("Scan"), this);
    _scanButton->setFixedWidth(80);
    _scanButton->setFont(font);
    portInputLayout->addWidget(_portLabel);
    portInputLayout->addWidget(_portScanEdit);
    portInputLayout->addWidget(_scanButton);
    portScanLayout->addWidget(portInputWidget);

    _scanResultEdit = new ElaPlainTextEdit(this);
    _scanResultEdit->setReadOnly(true);
    _scanResultEdit->setFixedHeight(120);
    _scanResultEdit->setPlaceholderText(tr("Scan results will appear here..."));
    _scanResultEdit->setFont(font);
    portScanLayout->addWidget(_scanResultEdit);

    toolsLayout->addWidget(_portScanGroupBox);

    _crcGroupBox = new Widgets::GroupBox(tr("CRC Calculator"), this);
    QVBoxLayout* crcLayout = new QVBoxLayout(_crcGroupBox);
    crcLayout->setContentsMargins(10, 10, 10, 10);
    crcLayout->setSpacing(10);

    QWidget* crcInputWidget = new QWidget(this);
    QHBoxLayout* crcInputLayout = new QHBoxLayout(crcInputWidget);
    crcInputLayout->setContentsMargins(0, 0, 0, 0);
    crcInputLayout->setSpacing(10);

    _crcInputLabel = new ElaText(tr("Input Data:"), this);
    _crcInputLabel->setFixedWidth(80);
    _crcInputLabel->setFont(font);
    _crcInputEdit = new ElaLineEdit(this);
    _crcInputEdit->setPlaceholderText(tr("Enter HEX data..."));
    _crcInputEdit->setFont(font);
    _crcTypeComboBox = new Widgets::ComboBox(this);
    _crcTypeComboBox->addItems({"CRC16-Modbus", "CRC16-CCITT", "CRC32"});
    _crcTypeComboBox->setFixedWidth(150);
    _crcTypeComboBox->setFont(font);
    _calculateButton = new ElaPushButton(tr("Calculate"), this);
    _calculateButton->setFixedWidth(80);
    _calculateButton->setFont(font);
    crcInputLayout->addWidget(_crcInputLabel);
    crcInputLayout->addWidget(_crcInputEdit);
    crcInputLayout->addWidget(_crcTypeComboBox);
    crcInputLayout->addWidget(_calculateButton);
    crcLayout->addWidget(crcInputWidget);

    QWidget* crcResultWidget = new QWidget(this);
    QHBoxLayout* crcResultLayout = new QHBoxLayout(crcResultWidget);
    crcResultLayout->setContentsMargins(0, 0, 0, 0);
    crcResultLayout->setSpacing(10);

    _crcResultLabel = new ElaText(tr("Result:"), this);
    _crcResultLabel->setFixedWidth(80);
    _crcResultLabel->setFont(font);
    _crcResultEdit = new ElaLineEdit(this);
    _crcResultEdit->setReadOnly(true);
    _crcResultEdit->setFont(font);
    crcResultLayout->addWidget(_crcResultLabel);
    crcResultLayout->addWidget(_crcResultEdit);
    crcLayout->addWidget(crcResultWidget);

    toolsLayout->addWidget(_crcGroupBox);

    _convertGroupBox = new Widgets::GroupBox(tr("Number Converter"), this);
    QVBoxLayout* convertLayout = new QVBoxLayout(_convertGroupBox);
    convertLayout->setContentsMargins(10, 10, 10, 10);
    convertLayout->setSpacing(10);

    QWidget* convertInputWidget = new QWidget(this);
    QHBoxLayout* convertInputLayout = new QHBoxLayout(convertInputWidget);
    convertInputLayout->setContentsMargins(0, 0, 0, 0);
    convertInputLayout->setSpacing(10);

    _convertInputLabel = new ElaText(tr("Input:"), this);
    _convertInputLabel->setFixedWidth(60);
    _convertInputLabel->setFont(font);
    _convertInputEdit = new ElaLineEdit(this);
    _convertInputEdit->setPlaceholderText(tr("Enter data..."));
    _convertInputEdit->setFont(font);

    _fromLabel = new ElaText(tr("From:"), this);
    _fromLabel->setFont(font);
    _convertFromComboBox = new Widgets::ComboBox(this);
    _convertFromComboBox->addItems({tr("Decimal"), tr("Hexadecimal"), tr("Binary"), tr("ASCII")});
    _convertFromComboBox->setFixedWidth(100);
    _convertFromComboBox->setFont(font);

    _toLabel = new ElaText(tr("To:"), this);
    _toLabel->setFont(font);
    _convertToComboBox = new Widgets::ComboBox(this);
    _convertToComboBox->addItems({tr("Decimal"), tr("Hexadecimal"), tr("Binary"), tr("ASCII")});
    _convertToComboBox->setFixedWidth(100);
    _convertToComboBox->setFont(font);

    _convertButton = new ElaPushButton(tr("Convert"), this);
    _convertButton->setFixedWidth(80);
    _convertButton->setFont(font);

    convertInputLayout->addWidget(_convertInputLabel);
    convertInputLayout->addWidget(_convertInputEdit);
    convertInputLayout->addWidget(_fromLabel);
    convertInputLayout->addWidget(_convertFromComboBox);
    convertInputLayout->addWidget(_toLabel);
    convertInputLayout->addWidget(_convertToComboBox);
    convertInputLayout->addWidget(_convertButton);
    convertLayout->addWidget(convertInputWidget);

    QWidget* convertResultWidget = new QWidget(this);
    QHBoxLayout* convertResultLayout = new QHBoxLayout(convertResultWidget);
    convertResultLayout->setContentsMargins(0, 0, 0, 0);
    convertResultLayout->setSpacing(10);

    _convertResultLabel = new ElaText(tr("Result:"), this);
    _convertResultLabel->setFixedWidth(60);
    _convertResultLabel->setFont(font);
    _convertResultEdit = new ElaLineEdit(this);
    _convertResultEdit->setReadOnly(true);
    _convertResultEdit->setFont(font);
    convertResultLayout->addWidget(_convertResultLabel);
    convertResultLayout->addWidget(_convertResultEdit);
    convertLayout->addWidget(convertResultWidget);

    toolsLayout->addWidget(_convertGroupBox);

    toolsLayout->addStretch();

    connect(_scanButton, &ElaPushButton::clicked, this, &ToolsPage::onScanButtonClicked);
    connect(_calculateButton, &ElaPushButton::clicked, this, &ToolsPage::onCalculateButtonClicked);
    connect(_convertButton, &ElaPushButton::clicked, this, &ToolsPage::onConvertButtonClicked);

    _tabWidget->addTab(toolsWidget, tr("Tools"));
}

void ToolsPage::onScanButtonClicked()
{
    QString ip = _portScanEdit->text().trimmed();
    if (ip.isEmpty()) {
        _scanResultEdit->appendPlainText(tr("Please enter target IP address"));
        return;
    }

    _scanResultEdit->clear();
    _scanResultEdit->appendPlainText(tr("Scanning common ports on %1...").arg(ip));
    
    _scanningIp = ip;
    _scanningPort = 1;
    
    scanPort(ip, _scanningPort);
}

void ToolsPage::scanPort(const QString& ip, int port)
{
    if (port > 1000) {
        _scanResultEdit->appendPlainText(tr("Scan complete"));
        return;
    }

    QTcpSocket* socket = new QTcpSocket(this);
    socket->setProperty("port", port);
    
    connect(socket, &QTcpSocket::connected, this, [this, socket]() {
        int port = socket->property("port").toInt();
        _scanResultEdit->appendPlainText(tr("Port %1: Open").arg(port));
        socket->close();
        socket->deleteLater();
        
        _scanningPort++;
        scanPort(_scanningIp, _scanningPort);
    });
    
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, [this, socket]() {
        socket->deleteLater();
        
        _scanningPort++;
        scanPort(_scanningIp, _scanningPort);
    });
    
    socket->connectToHost(ip, port);
}

void ToolsPage::onCalculateButtonClicked()
{
    QString input = _crcInputEdit->text().trimmed();
    if (input.isEmpty()) {
        _crcResultEdit->setText(tr("Please enter data"));
        return;
    }

    QByteArray data;
    QStringList hexBytes = input.split(' ', Qt::SkipEmptyParts);
    if (hexBytes.isEmpty()) {
        hexBytes = QStringList(input);
    }
    
    bool valid = true;
    for (const QString& hex : hexBytes) {
        bool ok;
        uchar byte = hex.toUInt(&ok, 16);
        if (!ok) {
            valid = false;
            break;
        }
        data.append(byte);
    }
    
    if (!valid) {
        _crcResultEdit->setText(tr("Invalid HEX data"));
        return;
    }

    QString result;
    QString crcType = _crcTypeComboBox->currentText();
    
    if (crcType == "CRC16-Modbus") {
        quint16 crc = calculateCRC16Modbus(data);
        result = QString("%1").arg(crc, 4, 16, QChar('0')).toUpper();
    } else if (crcType == "CRC16-CCITT") {
        quint16 crc = calculateCRC16CCITT(data);
        result = QString("%1").arg(crc, 4, 16, QChar('0')).toUpper();
    } else if (crcType == "CRC32") {
        quint32 crc = calculateCRC32(data);
        result = QString("%1").arg(crc, 8, 16, QChar('0')).toUpper();
    }
    
    _crcResultEdit->setText(result);
}

quint16 ToolsPage::calculateCRC16Modbus(const QByteArray& data)
{
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); i++) {
        crc ^= static_cast<quint8>(data[i]);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

quint16 ToolsPage::calculateCRC16CCITT(const QByteArray& data)
{
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); i++) {
        crc ^= static_cast<quint8>(data[i]) << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

quint32 ToolsPage::calculateCRC32(const QByteArray& data)
{
    quint32 crc = 0xFFFFFFFF;
    for (int i = 0; i < data.size(); i++) {
        crc ^= static_cast<quint8>(data[i]);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x00000001) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}

void ToolsPage::onConvertButtonClicked()
{
    QString input = _convertInputEdit->text().trimmed();
    if (input.isEmpty()) {
        _convertResultEdit->setText(tr("Please enter data"));
        return;
    }

    QString fromType = _convertFromComboBox->currentText();
    QString toType = _convertToComboBox->currentText();
    
    QString result;
    
    if (fromType == toType) {
        result = input;
    } else if (fromType == tr("Decimal")) {
        bool ok;
        qlonglong value = input.toLongLong(&ok);
        if (!ok) {
            _convertResultEdit->setText(tr("Invalid decimal data"));
            return;
        }
        
        if (toType == tr("Hexadecimal")) {
            result = QString("%1").arg(value, 0, 16).toUpper();
        } else if (toType == tr("Binary")) {
            result = QString("%1").arg(value, 0, 2);
        } else if (toType == tr("ASCII")) {
            result = QChar(static_cast<char>(value));
        }
    } else if (fromType == tr("Hexadecimal")) {
        bool ok;
        qlonglong value = input.toLongLong(&ok, 16);
        if (!ok) {
            _convertResultEdit->setText(tr("Invalid hexadecimal data"));
            return;
        }
        
        if (toType == tr("Decimal")) {
            result = QString("%1").arg(value);
        } else if (toType == tr("Binary")) {
            result = QString("%1").arg(value, 0, 2);
        } else if (toType == tr("ASCII")) {
            result = QChar(static_cast<char>(value));
        }
    } else if (fromType == tr("Binary")) {
        bool ok;
        qlonglong value = input.toLongLong(&ok, 2);
        if (!ok) {
            _convertResultEdit->setText(tr("Invalid binary data"));
            return;
        }
        
        if (toType == tr("Decimal")) {
            result = QString("%1").arg(value);
        } else if (toType == tr("Hexadecimal")) {
            result = QString("%1").arg(value, 0, 16).toUpper();
        } else if (toType == tr("ASCII")) {
            result = QChar(static_cast<char>(value));
        }
    } else if (fromType == tr("ASCII")) {
        if (input.length() > 0) {
            QChar ch = input.at(0);
            qlonglong value = static_cast<qlonglong>(ch.toLatin1());
            
            if (toType == tr("Decimal")) {
                result = QString("%1").arg(value);
            } else if (toType == tr("Hexadecimal")) {
                result = QString("%1").arg(value, 0, 16).toUpper();
            } else if (toType == tr("Binary")) {
                result = QString("%1").arg(value, 0, 2);
            }
        }
    }
    
    _convertResultEdit->setText(result);
}

}
}
