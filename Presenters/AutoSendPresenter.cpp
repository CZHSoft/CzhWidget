#include "AutoSendPresenter.h"
#include "Views/Panels/AutoSendPanel.h"

namespace Presenters {

AutoSendPresenter::AutoSendPresenter(Views::Panels::AutoSendPanel* panel, QObject* parent)
    : QObject(parent)
    , _panel(panel)
    , _model(new Models::AutoSendModel(this))
{
}

AutoSendPresenter::~AutoSendPresenter()
{
}

void AutoSendPresenter::init()
{
    if (_panel) {
        _panel->setPresenter(this);

        connect(_panel, &Views::Panels::AutoSendPanel::autoSendToggled, this, &AutoSendPresenter::onAutoSendToggled);
        connect(_panel, &Views::Panels::AutoSendPanel::addCommandRequested, this, &AutoSendPresenter::onAddCommandRequested);
        connect(_panel, &Views::Panels::AutoSendPanel::removeCommandRequested, this, &AutoSendPresenter::onRemoveCommandRequested);
        connect(_panel, &Views::Panels::AutoSendPanel::clearCommandsRequested, this, &AutoSendPresenter::onClearCommandsRequested);
        connect(_panel, &Views::Panels::AutoSendPanel::moveCommandUpRequested, this, &AutoSendPresenter::onMoveCommandUpRequested);
        connect(_panel, &Views::Panels::AutoSendPanel::moveCommandDownRequested, this, &AutoSendPresenter::onMoveCommandDownRequested);
        connect(_panel, &Views::Panels::AutoSendPanel::sendOptionsChanged, this, &AutoSendPresenter::onSendOptionsChanged);
        connect(_panel, &Views::Panels::AutoSendPanel::delayChanged, this, &AutoSendPresenter::onDelayChanged);
        connect(_panel, &Views::Panels::AutoSendPanel::intervalChanged, this, &AutoSendPresenter::onIntervalChanged);
        connect(_panel, &Views::Panels::AutoSendPanel::loopOptionsChanged, this, &AutoSendPresenter::onLoopOptionsChanged);
        connect(_panel, &Views::Panels::AutoSendPanel::executeRequested, this, &AutoSendPresenter::onExecuteRequested);
    }

    connect(_model, &Models::AutoSendModel::autoSendStarted, this, &AutoSendPresenter::onModelAutoSendStarted);
    connect(_model, &Models::AutoSendModel::autoSendStopped, this, &AutoSendPresenter::onModelAutoSendStopped);
    connect(_model, &Models::AutoSendModel::commandSent, this, &AutoSendPresenter::onModelCommandSent);
    connect(_model, &Models::AutoSendModel::loopProgress, this, &AutoSendPresenter::onModelLoopProgress);
    connect(_model, &Models::AutoSendModel::errorOccurred, this, &AutoSendPresenter::onModelErrorOccurred);
    connect(_model, &Models::AutoSendModel::sendFinished, this, &AutoSendPresenter::onModelSendFinished);
    connect(_model, &Models::AutoSendModel::executionStateChanged, this, &AutoSendPresenter::onModelExecutionStateChanged);
}

void AutoSendPresenter::setConnectionStatus(bool connected)
{
    _model->setConnectionStatus(connected);
    if (_panel) {
        _panel->updateConnectionStatus(connected);
    }
}

void AutoSendPresenter::onAutoSendToggled(bool enabled)
{
    if (enabled) {
        syncFromView();
        _model->startAutoSend();
    } else {
        _model->stopAutoSend();
    }
}

void AutoSendPresenter::onAddCommandRequested(const QString& command)
{
    _model->addCommand(command);
    if (_panel) {
        _panel->addCommandItem(command);
    }
}

void AutoSendPresenter::onRemoveCommandRequested(int index)
{
    _model->removeCommand(index);
    if (_panel) {
        _panel->removeCommandItem(index);
    }
}

void AutoSendPresenter::onClearCommandsRequested()
{
    _model->clearCommands();
    if (_panel) {
        _panel->clearCommandList();
    }
}

void AutoSendPresenter::onMoveCommandUpRequested(int index)
{
    Q_UNUSED(index);
}

void AutoSendPresenter::onMoveCommandDownRequested(int index)
{
    Q_UNUSED(index);
}

void AutoSendPresenter::onSendOptionsChanged()
{
    syncFromView();
}

void AutoSendPresenter::onDelayChanged(int delayMs)
{
    _model->setDelay(delayMs);
}

void AutoSendPresenter::onIntervalChanged(int intervalMs)
{
    _model->setInterval(intervalMs);
}

void AutoSendPresenter::onLoopOptionsChanged(bool enabled, int count)
{
    _model->setLoopEnabled(enabled);
    _model->setLoopCount(count);
}

void AutoSendPresenter::onExecuteRequested()
{
    if (_model->isAutoSending()) {
        _model->stopAutoSend();
    } else {
        syncFromView();
        _model->startAutoSend();
    }
}

void AutoSendPresenter::onModelAutoSendStarted()
{
    if (_panel) {
        _panel->updateAutoSendStatus(true);
    }
}

void AutoSendPresenter::onModelAutoSendStopped()
{
    if (_panel) {
        _panel->updateAutoSendStatus(false);
    }
}

void AutoSendPresenter::onModelCommandSent(const QString& command, int index, int total)
{
    Q_UNUSED(command);
    if (_panel) {
        _panel->updateSendProgress(index, total);
    }
}

void AutoSendPresenter::onModelLoopProgress(int currentLoop, int totalLoops)
{
    if (_panel) {
        _panel->updateLoopProgress(currentLoop, totalLoops);
    }
}

void AutoSendPresenter::onModelErrorOccurred(const QString& error)
{
    if (_panel) {
        _panel->showError(error);
    }
}

void AutoSendPresenter::onModelSendFinished()
{
    emit sendFinished();
}

void AutoSendPresenter::onModelExecutionStateChanged(int state, const QString& message)
{
    if (_panel) {
        _panel->updateExecutionState(static_cast<Views::Panels::AutoSendPanel::ExecutionState>(state), message);
    }
}

void AutoSendPresenter::syncFromView()
{
    if (_panel) {
        QList<Models::CommandItem> items;
        for (int i = 0; i < _panel->getCommandCount(); ++i) {
            Models::CommandItem item = _panel->getCommandItem(i);
            items.append(item);
        }
        _model->setCommandItems(items);
        _model->setLoopEnabled(_panel->isLoopEnabled());
        _model->setLoopCount(_panel->getLoopCount());
    }
}

}
