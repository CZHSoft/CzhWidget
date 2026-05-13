#ifndef AUTOSENDPRESENTER_H
#define AUTOSENDPRESENTER_H

#include <QObject>
#include "Models/AutoSendModel.h"

namespace Views {
namespace Panels {
class AutoSendPanel;
}
}

namespace Presenters {

class AutoSendPresenter : public QObject
{
    Q_OBJECT

public:
    explicit AutoSendPresenter(Views::Panels::AutoSendPanel* panel, QObject* parent = nullptr);
    ~AutoSendPresenter() override;

    void init();
    void setConnectionStatus(bool connected);

public slots:
    void onAutoSendToggled(bool enabled);
    void onAddCommandRequested(const QString& command);
    void onRemoveCommandRequested(int index);
    void onClearCommandsRequested();
    void onMoveCommandUpRequested(int index);
    void onMoveCommandDownRequested(int index);
    void onSendOptionsChanged();
    void onDelayChanged(int delayMs);
    void onIntervalChanged(int intervalMs);
    void onLoopOptionsChanged(bool enabled, int count);
    void onExecuteRequested();

private slots:
    void onModelAutoSendStarted();
    void onModelAutoSendStopped();
    void onModelCommandSent(const QString& command, int index, int total);
    void onModelLoopProgress(int currentLoop, int totalLoops);
    void onModelErrorOccurred(const QString& error);
    void onModelSendFinished();
    void onModelExecutionStateChanged(int state, const QString& message);

signals:
    void sendFinished();

private:
    void syncFromView();

    Views::Panels::AutoSendPanel* _panel;
    Models::AutoSendModel* _model;
};

}

#endif
