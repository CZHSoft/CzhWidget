#include "AutoSendModel.h"
#include <QDebug>

namespace Models {

AutoSendModel::AutoSendModel(QObject* parent)
    : QObject(parent)
    , _commManager(Services::Comm::CommManager::getInstance())
    , _delayMs(0)
    , _intervalMs(1000)
    , _loopEnabled(false)
    , _loopCount(10)
    , _hexSend(false)
    , _newLineAppend(false)
    , _isConnected(false)
    , _isAutoSending(false)
    , _currentCommandIndex(0)
    , _currentLoopCount(0)
    , _currentExecuteCount(0)
    , _sendTimer(new QTimer(this))
{
    _sendTimer->setSingleShot(true);
    connect(_sendTimer, &QTimer::timeout, this, &AutoSendModel::onTimerTimeout);
}

AutoSendModel::~AutoSendModel()
{
    _sendTimer->stop();
}

void AutoSendModel::setCommands(const QStringList& commands)
{
    _commands = commands;
}

QStringList AutoSendModel::getCommands() const
{
    return _commands;
}

void AutoSendModel::addCommand(const QString& command)
{
    if (!command.trimmed().isEmpty()) {
        _commands.append(command);
    }
}

void AutoSendModel::removeCommand(int index)
{
    if (index >= 0 && index < _commands.size()) {
        _commands.removeAt(index);
    }
    if (index >= 0 && index < _commandItems.size()) {
        _commandItems.removeAt(index);
    }
}

void AutoSendModel::clearCommands()
{
    _commands.clear();
    _commandItems.clear();
}

void AutoSendModel::setDelay(int delayMs)
{
    _delayMs = delayMs;
}

int AutoSendModel::getDelay() const
{
    return _delayMs;
}

void AutoSendModel::setInterval(int intervalMs)
{
    _intervalMs = intervalMs;
}

int AutoSendModel::getInterval() const
{
    return _intervalMs;
}

void AutoSendModel::setLoopEnabled(bool enabled)
{
    _loopEnabled = enabled;
}

bool AutoSendModel::isLoopEnabled() const
{
    return _loopEnabled;
}

void AutoSendModel::setLoopCount(int count)
{
    _loopCount = count;
}

int AutoSendModel::getLoopCount() const
{
    return _loopCount;
}

void AutoSendModel::setHexSend(bool enabled)
{
    _hexSend = enabled;
}

bool AutoSendModel::isHexSend() const
{
    return _hexSend;
}

void AutoSendModel::setNewLineAppend(bool enabled)
{
    _newLineAppend = enabled;
}

bool AutoSendModel::isNewLineAppend() const
{
    return _newLineAppend;
}

void AutoSendModel::setCommandItems(const QList<CommandItem>& items)
{
    _commandItems = items;
    _commands.clear();
    for (const CommandItem& item : items) {
        _commands.append(item.command);
    }
}

QList<CommandItem> AutoSendModel::getCommandItems() const
{
    return _commandItems;
}

void AutoSendModel::setConnectionStatus(bool connected)
{
    _isConnected = connected;
    if (!connected && _isAutoSending) {
        stopAutoSend();
    }
}

bool AutoSendModel::isConnected() const
{
    return _isConnected;
}

void AutoSendModel::startAutoSend()
{
    if (!_isConnected) {
        emit errorOccurred("串口未连接，无法开始自动发送");
        emit executionStateChanged(4, "串口未连接");
        return;
    }

    if (_commandItems.isEmpty() && _commands.isEmpty()) {
        emit errorOccurred("指令列表为空，请先添加指令");
        emit executionStateChanged(4, "指令列表为空");
        return;
    }

    _isAutoSending = true;
    _currentCommandIndex = 0;
    _currentLoopCount = 0;
    _currentExecuteCount = 0;

    emit autoSendStarted();
    emit executionStateChanged(1, "开始执行...");

    if (_delayMs > 0) {
        _sendTimer->start(_delayMs);
    } else {
        sendNextCommand();
    }
}

void AutoSendModel::stopAutoSend()
{
    if (_isAutoSending) {
        _sendTimer->stop();
        _isAutoSending = false;
        emit autoSendStopped();
        emit executionStateChanged(0, "已停止");
    }
}

bool AutoSendModel::isAutoSending() const
{
    return _isAutoSending;
}

void AutoSendModel::onTimerTimeout()
{
    sendNextCommand();
}

void AutoSendModel::sendNextCommand()
{
    if (!_isAutoSending || !_isConnected) {
        return;
    }

    int totalCommands = _commandItems.isEmpty() ? _commands.size() : _commandItems.size();
    
    if (_currentCommandIndex >= totalCommands) {
        _currentCommandIndex = 0;

        if (_loopEnabled) {
            _currentLoopCount++;
            if (_currentLoopCount >= _loopCount) {
                stopAutoSend();
                emit sendFinished();
                emit executionStateChanged(3, "执行完成");
                return;
            }
        } else {
            stopAutoSend();
            emit sendFinished();
            emit executionStateChanged(3, "执行完成");
            return;
        }
    }

    CommandItem currentItem;
    QString command;
    bool isHex = false;
    bool appendNewLine = false;
    int preDelay = 0;
    int postDelay = _intervalMs;
    int executeCount = 1;

    if (!_commandItems.isEmpty()) {
        currentItem = _commandItems.at(_currentCommandIndex);
        if (!currentItem.enabled) {
            _currentCommandIndex++;
            sendNextCommand();
            return;
        }
        command = currentItem.command;
        isHex = currentItem.isHexMode;
        appendNewLine = currentItem.appendNewLine;
        preDelay = currentItem.preDelayMs;
        postDelay = currentItem.postDelayMs;
        executeCount = currentItem.executeCount;
    } else {
        command = _commands.at(_currentCommandIndex);
        isHex = _hexSend;
        appendNewLine = _newLineAppend;
        preDelay = 0;
        postDelay = _intervalMs;
        executeCount = 1;
    }

    emit executionStateChanged(1, QString("发送指令 [%1/%2]: %3").arg(_currentCommandIndex + 1).arg(totalCommands).arg(command));

    QByteArray data = formatCommand(command, isHex, appendNewLine);

    if (_commManager->sendData(data) < 0) {
        QString errorMsg = "发送指令失败: " + _commManager->getErrorString();
        emit errorOccurred(errorMsg);
        stopAutoSend();
        emit executionStateChanged(4, errorMsg);
        return;
    }

    emit commandSent(command, _currentCommandIndex, totalCommands);
    if (_loopEnabled) {
        emit loopProgress(_currentLoopCount, _loopCount);
    }

    _currentExecuteCount++;
    if (_currentExecuteCount >= executeCount) {
        _currentExecuteCount = 0;
        _currentCommandIndex++;
    }

    if (_isAutoSending) {
        _sendTimer->start(postDelay);
    }
}

QByteArray AutoSendModel::formatCommand(const QString& command, bool isHex, bool appendNewLine) const
{
    QByteArray data;

    if (isHex) {
        data = QByteArray::fromHex(command.toUtf8());
    } else {
        data = command.toUtf8();
    }

    if (appendNewLine) {
        data.append("\r\n");
    }

    return data;
}

}
