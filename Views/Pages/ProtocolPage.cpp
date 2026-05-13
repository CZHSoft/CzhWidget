#include "Views/Pages/ProtocolPage.h"
#include "ElaText.h"
#include "Services/Protocol/ProtocolParserFactory.h"
#include "Services/Protocol/ProtocolParser.h"
#include "Services/Protocol/ProtocolType.h"
#include "Services/Protocol/ParseResult.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QClipboard>
#include <QDateTime>
#include <QApplication>

namespace Views {
namespace Pages {

ProtocolPage::ProtocolPage(QWidget* parent)
    : ElaScrollPage(parent)
    , _protocolComboBox(nullptr)
    , _autoParseSwitch(nullptr)
    , _rawDataTextEdit(nullptr)
    , _parsedResultTextEdit(nullptr)
    , _parseButton(nullptr)
    , _copyButton(nullptr)
    , _clearButton(nullptr)
    , _logTextEdit(nullptr)
    , _protocolLabel(nullptr)
    , _autoParseLabel(nullptr)
    , _rawDataLabel(nullptr)
    , _parsedLabel(nullptr)
    , _statusText(nullptr)
{
    setWindowTitle(tr("Protocol"));
    setTitleVisible(false);

    initUI();
    initConnections();
}

ProtocolPage::~ProtocolPage()
{
}

void ProtocolPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    ElaScrollPage::changeEvent(event);
}

void ProtocolPage::retranslateUi()
{
    setWindowTitle(tr("Protocol"));

    if (_protocolLabel) {
        _protocolLabel->setText(tr("Protocol:"));
    }
    if (_protocolComboBox) {
        int currentIndex = _protocolComboBox->currentIndex();
        _protocolComboBox->clear();
        _protocolComboBox->addItems({tr("Disable Parsing"), "Modbus-RTU", "Modbus-TCP", "CAN", "CANopen", tr("Custom Protocol")});
        _protocolComboBox->setCurrentIndex(currentIndex);
    }
    if (_autoParseLabel) {
        _autoParseLabel->setText(tr("Auto Parse:"));
    }
    if (_parseButton) {
        _parseButton->setText(tr("Parse"));
    }
    if (_copyButton) {
        _copyButton->setText(tr("Copy Result"));
    }
    if (_clearButton) {
        _clearButton->setText(tr("Clear"));
    }
    if (_rawDataLabel) {
        _rawDataLabel->setText(tr("Raw HEX Data"));
    }
    if (_rawDataTextEdit) {
        _rawDataTextEdit->setPlaceholderText(tr("Enter or paste HEX data...\n\nExamples:\nModbus RTU: 01 03 00 00 00 01 D4 0B\nModbus TCP: 00 01 00 00 00 06 01 03 00 00 00 01\nCAN: 123 02 41 5A\nCANopen: 018 01 02 03 04 05 06 07 08"));
    }
    if (_parsedLabel) {
        _parsedLabel->setText(tr("Parsed Result"));
    }
    if (_parsedResultTextEdit) {
        _parsedResultTextEdit->setPlaceholderText(tr("Parsing results will appear here..."));
    }
    if (_statusText) {
        _statusText->setText(tr("Parsing Log:"));
    }
    if (_logTextEdit) {
        _logTextEdit->setPlaceholderText(tr("Parsing log..."));
    }
}

void ProtocolPage::initUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    QWidget* topWidget = new QWidget(this);
    QHBoxLayout* topLayout = new QHBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(15);

    _protocolLabel = new ElaText(tr("Protocol:"), this);
    _protocolLabel->setTextPixelSize(13);
    _protocolComboBox = new Widgets::ComboBox(this);
    _protocolComboBox->addItems({tr("Disable Parsing"), "Modbus-RTU", "Modbus-TCP", "CAN", "CANopen", tr("Custom Protocol")});
    _protocolComboBox->setFixedWidth(180);
    topLayout->addWidget(_protocolLabel);
    topLayout->addWidget(_protocolComboBox);

    _autoParseLabel = new ElaText(tr("Auto Parse:"), this);
    _autoParseLabel->setTextPixelSize(13);
    _autoParseSwitch = new ElaToggleSwitch(this);
    topLayout->addWidget(_autoParseLabel);
    topLayout->addWidget(_autoParseSwitch);

    topLayout->addStretch();

    _parseButton = new ElaPushButton(tr("Parse"), this);
    _parseButton->setFixedWidth(100);
    topLayout->addWidget(_parseButton);

    _copyButton = new ElaPushButton(tr("Copy Result"), this);
    _copyButton->setFixedWidth(100);
    topLayout->addWidget(_copyButton);

    _clearButton = new ElaPushButton(tr("Clear"), this);
    _clearButton->setFixedWidth(80);
    topLayout->addWidget(_clearButton);

    mainLayout->addWidget(topWidget);


    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setStyleSheet(R"(
        QSplitter {
            background-color: transparent;
        }
    )");
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    _rawDataLabel = new ElaText(tr("Raw HEX Data"), this);
    _rawDataLabel->setTextPixelSize(13);
    leftLayout->addWidget(_rawDataLabel);

    _rawDataTextEdit = new ElaPlainTextEdit(this);
    _rawDataTextEdit->setPlaceholderText(tr("Enter or paste HEX data...\n\nExamples:\nModbus RTU: 01 03 00 00 00 01 D4 0B\nModbus TCP: 00 01 00 00 00 06 01 03 00 00 00 01\nCAN: 123 02 41 5A\nCANopen: 018 01 02 03 04 05 06 07 08"));
    _rawDataTextEdit->setMinimumHeight(400);
    leftLayout->addWidget(_rawDataTextEdit);

    splitter->addWidget(leftWidget);

    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    _parsedLabel = new ElaText(tr("Parsed Result"), this);
    _parsedLabel->setTextPixelSize(13);
    rightLayout->addWidget(_parsedLabel);

    _parsedResultTextEdit = new ElaPlainTextEdit(this);
    _parsedResultTextEdit->setReadOnly(true);
    _parsedResultTextEdit->setPlaceholderText(tr("Parsing results will appear here..."));
    _parsedResultTextEdit->setMinimumHeight(400);
    rightLayout->addWidget(_parsedResultTextEdit);

    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter);

    QWidget* bottomWidget = new QWidget(this);
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    _statusText = new ElaText(tr("Parsing Log:"), this);
    _statusText->setTextPixelSize(13);
    bottomLayout->addWidget(_statusText);

    _logTextEdit = new ElaPlainTextEdit(this);
    _logTextEdit->setReadOnly(true);
    _logTextEdit->setFixedHeight(80);
    _logTextEdit->setPlaceholderText(tr("Parsing log..."));
    bottomLayout->addWidget(_logTextEdit);

    mainLayout->addWidget(bottomWidget);

    addCentralWidget(centralWidget, true, false, 0);
}

void ProtocolPage::initConnections()
{
    connect(_protocolComboBox, QOverload<int>::of(&Widgets::ComboBox::currentIndexChanged),
            this, &ProtocolPage::onProtocolChanged);
    connect(_autoParseSwitch, &ElaToggleSwitch::toggled,
            this, &ProtocolPage::onAutoParseStateChanged);
    connect(_parseButton, &ElaPushButton::clicked,
            this, &ProtocolPage::onParseClicked);
    connect(_copyButton, &ElaPushButton::clicked,
            this, &ProtocolPage::onCopyClicked);
    connect(_clearButton, &ElaPushButton::clicked,
            this, &ProtocolPage::onClearClicked);
    connect(_rawDataTextEdit, &ElaPlainTextEdit::textChanged,
            this, &ProtocolPage::onRawDataChanged);
}

void ProtocolPage::onParseClicked()
{
    QString hexData = _rawDataTextEdit->toPlainText().trimmed();

    if (hexData.isEmpty()) {
        updateLog(tr("Please enter HEX data to parse"));
        return;
    }

    if (!validateHexInput(hexData)) {
        _parsedResultTextEdit->setPlainText(tr("Input data is not valid HEX format"));
        updateLog(tr("Invalid input format"));
        return;
    }

    QString protocolStr = _protocolComboBox->currentText();
    Services::Protocol::ProtocolType type = Services::Protocol::stringToProtocolType(protocolStr);

    if (type == Services::Protocol::ProtocolType::None) {
        _parsedResultTextEdit->setPlainText(tr("No protocol selected, cannot parse"));
        updateLog(tr("Please select a protocol"));
        return;
    }

    if (type == Services::Protocol::ProtocolType::Custom) {
        _parsedResultTextEdit->setPlainText(tr("Custom protocol parsing not implemented"));
        updateLog(tr("Custom protocol not supported"));
        return;
    }

    QByteArray rawBytes = QByteArray::fromHex(hexData.toUtf8());

    auto parser = Services::Protocol::ProtocolParserFactory::createParser(type);
    if (!parser) {
        _parsedResultTextEdit->setPlainText("无法创建解析器");
        updateLog("解析器创建失败");
        return;
    }

    updateLog(QString("开始解析 [%1] - 数据长度: %2 字节").arg(protocolStr).arg(rawBytes.size()));

    Services::Protocol::ParseResult result = parser->parse(rawBytes);

    if (result.success) {
        _parsedResultTextEdit->setPlainText(result.formattedResult);
        updateLog(QString("解析成功 - %1").arg(protocolStr));
    } else {
        _parsedResultTextEdit->setPlainText("[解析失败]\n" + result.errorMessage);
        updateLog(QString("解析失败: %1").arg(result.errorMessage));
    }
}

void ProtocolPage::onCopyClicked()
{
    QString result = _parsedResultTextEdit->toPlainText();
    if (!result.isEmpty()) {
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(result);
        updateLog(tr("Parsing result copied to clipboard"));
    } else {
        updateLog(tr("No content to copy"));
    }
}

void ProtocolPage::onClearClicked()
{
    _rawDataTextEdit->clear();
    _parsedResultTextEdit->clear();
    _logTextEdit->clear();
    updateLog(tr("Cleared all content"));
}

void ProtocolPage::onProtocolChanged(int)
{
    _currentParser.reset();
    if (_autoParseSwitch->getIsToggled()) {
        onParseClicked();
    }
}

void ProtocolPage::onAutoParseStateChanged(bool enabled)
{
    _parseButton->setEnabled(!enabled);
    if (enabled) {
        updateLog(tr("Auto parsing enabled"));
        onParseClicked();
    } else {
        updateLog(tr("Auto parsing disabled"));
    }
}

void ProtocolPage::onRawDataChanged()
{
    if (_autoParseSwitch->getIsToggled()) {
        onParseClicked();
    }
}

void ProtocolPage::updateLog(const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    _logTextEdit->appendPlainText(QString("[%1] %2").arg(timestamp).arg(message));
}

bool ProtocolPage::validateHexInput(const QString& input)
{
    QString cleanInput = input;
    cleanInput.remove(QRegularExpression("[\\s\\n\\r]"));
    QRegularExpression hexRegex("^[0-9A-Fa-f]*$");
    return hexRegex.match(cleanInput).hasMatch() && cleanInput.size() % 2 == 0;
}

}
}