#ifndef AUTOSENDMODEL_H
#define AUTOSENDMODEL_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include "Services/Comm/CommManager.h"
#include "CommandTableModel.h"

namespace Models {

class CommandItem;

class AutoSendModel : public QObject
{
    Q_OBJECT

public:
    explicit AutoSendModel(QObject* parent = nullptr);
    ~AutoSendModel() override;

    void setCommands(const QStringList& commands);
    QStringList getCommands() const;
    void addCommand(const QString& command);
    void removeCommand(int index);
    void clearCommands();

    void setDelay(int delayMs);
    int getDelay() const;

    void setInterval(int intervalMs);
    int getInterval() const;

    void setLoopEnabled(bool enabled);
    bool isLoopEnabled() const;

    void setLoopCount(int count);
    int getLoopCount() const;

    void setHexSend(bool enabled);
    bool isHexSend() const;

    void setNewLineAppend(bool enabled);
    bool isNewLineAppend() const;

    void setCommandItems(const QList<CommandItem>& items);
    QList<CommandItem> getCommandItems() const;

    void setConnectionStatus(bool connected);
    bool isConnected() const;

    void startAutoSend();
    void stopAutoSend();
    bool isAutoSending() const;

signals:
    void autoSendStarted();
    void autoSendStopped();
    void commandSent(const QString& command, int index, int total);
    void loopProgress(int currentLoop, int totalLoops);
    void errorOccurred(const QString& error);
    void sendFinished();
    void executionStateChanged(int state, const QString& message);

private slots:
    void onTimerTimeout();

private:
    void sendNextCommand();
    QByteArray formatCommand(const QString& command, bool isHex, bool appendNewLine) const;
    void sendCommandWithDelay();

    Services::Comm::CommManager* _commManager;
    QStringList _commands;
    QList<CommandItem> _commandItems;
    int _delayMs;
    int _intervalMs;
    bool _loopEnabled;
    int _loopCount;
    bool _hexSend;
    bool _newLineAppend;

    bool _isConnected;
    bool _isAutoSending;
    int _currentCommandIndex;
    int _currentLoopCount;
    int _currentExecuteCount;
    QTimer* _sendTimer;
};

}

#endif // AUTOSENDMODEL_H
