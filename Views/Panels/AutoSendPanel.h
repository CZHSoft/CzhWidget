#ifndef AUTOSENDPANEL_H
#define AUTOSENDPANEL_H

#include <QWidget>
#include <QVariant>
#include <QEvent>
#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaTableView.h"
#include "ElaLineEdit.h"
#include "ElaCheckBox.h"
#include "ElaSpinBox.h"
#include "Widgets/ComboBox.h"
#include "Widgets/ToggleSwitch.h"
#include "Widgets/GroupBox.h"
#include "Models/CommandTableModel.h"

namespace  Presenters{
class AutoSendPresenter;
}

namespace Views {
namespace Panels {

class AutoSendPanel : public QWidget
{
    Q_OBJECT

public:
    enum ExecutionState {
        Idle,
        Running,
        Paused,
        Completed,
        Error
    };

    explicit AutoSendPanel(QWidget* parent = nullptr);
    ~AutoSendPanel() override;

    void setPresenter(Presenters::AutoSendPresenter* presenter);
    Presenters::AutoSendPresenter* getPresenter() const;

    void updateConnectionStatus(bool connected);
    void updateAutoSendStatus(bool enabled);
    void updateSendProgress(int currentIndex, int totalCount);
    void updateLoopProgress(int currentLoop, int totalLoops);
    void showError(const QString& error);
    void updateExecutionState(ExecutionState state, const QString& message = "");

    void clearCommandList();
    void addCommandItem(const QString& command);
    void removeCommandItem(int index);
    QStringList getCommandList() const;
    int getCurrentCommandIndex() const;

    bool isHexSend(int commandIndex) const;
    bool isNewLineAppend(int commandIndex) const;
    int getPreDelay(int commandIndex) const;
    int getPostDelay(int commandIndex) const;
    int getExecuteCount(int commandIndex) const;
    bool isCommandEnabled(int commandIndex) const;
    int getCommandCount() const;
    Models::CommandItem getCommandItem(int index) const;

    bool isLoopEnabled() const;
    int getLoopCount() const;

signals:
    void autoSendToggled(bool checked);
    void addCommandRequested(const QString& command);
    void removeCommandRequested(int index);
    void clearCommandsRequested();
    void moveCommandUpRequested(int index);
    void moveCommandDownRequested(int index);
    void delayChanged(int delayMs);
    void intervalChanged(int intervalMs);
    void loopOptionsChanged(bool enabled, int count);
    void sendOptionsChanged();
    void executeRequested();

protected:
    virtual void changeEvent(QEvent* event) override;

private slots:
    void onExecuteButtonClicked();
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onClearButtonClicked();
    void onMoveUpButtonClicked();
    void onMoveDownButtonClicked();
    void onLoopCheckBoxToggled(bool checked);
    void onLoopCountChanged(int value);

private:
    void initUI();
    void initConnections();
    void retranslateUi();
    void updateStateVisual(ExecutionState state);

private:
    Presenters::AutoSendPresenter* _presenter;

    ElaText* _titleText;
    ElaPushButton* _executeButton;
    ElaText* _statusLabel;
    ElaText* _progressLabel;
    ElaText* _executionStatusText;

    Widgets::GroupBox* _statusGroupBox;
    Widgets::GroupBox* _commandGroupBox;
    Widgets::GroupBox* _loopGroupBox;

    ElaTableView* _commandTableView;
    Models::CommandTableModel* _commandModel;
    ElaLineEdit* _commandInputEdit;
    ElaPushButton* _addCommandButton;
    ElaPushButton* _removeCommandButton;
    ElaPushButton* _clearCommandsButton;
    ElaPushButton* _moveUpButton;
    ElaPushButton* _moveDownButton;

    ElaCheckBox* _loopSendCheckBox;
    ElaSpinBox* _loopCountSpinBox;
    ElaText* _countLabel;

    ExecutionState _currentState;
};

}
}

#endif // AUTOSENDPANEL_H
