#include "AutoSendPanel.h"
#include "Presenters/AutoSendPresenter.h"
#include "Widgets/GroupBox.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QSizePolicy>
#include <QMessageBox>

namespace Views {
namespace Panels {

AutoSendPanel::AutoSendPanel(QWidget* parent)
    : QWidget(parent)
    , _presenter(nullptr)
    , _titleText(nullptr)
    , _executeButton(nullptr)
    , _statusLabel(nullptr)
    , _progressLabel(nullptr)
    , _executionStatusText(nullptr)
    , _statusGroupBox(nullptr)
    , _commandGroupBox(nullptr)
    , _loopGroupBox(nullptr)
    , _commandTableView(nullptr)
    , _commandModel(nullptr)
    , _commandInputEdit(nullptr)
    , _addCommandButton(nullptr)
    , _removeCommandButton(nullptr)
    , _clearCommandsButton(nullptr)
    , _moveUpButton(nullptr)
    , _moveDownButton(nullptr)
    , _loopSendCheckBox(nullptr)
    , _loopCountSpinBox(nullptr)
    , _currentState(Idle)
{
    initUI();
}

AutoSendPanel::~AutoSendPanel()
{
}

void AutoSendPanel::setPresenter(Presenters::AutoSendPresenter* presenter)
{
    _presenter = presenter;
}

Presenters::AutoSendPresenter* AutoSendPanel::getPresenter() const
{
    return _presenter;
}

void AutoSendPanel::initUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    _titleText = new ElaText(tr("Auto Command Send"), this);
    _titleText->setTextPixelSize(15);

    _executeButton = new ElaPushButton(tr("Execute"), this);
    _executeButton->setEnabled(false);

    titleLayout->addWidget(_titleText);
    titleLayout->addStretch();
    titleLayout->addWidget(_executeButton);
    mainLayout->addLayout(titleLayout);

    QHBoxLayout* statusLayout = new QHBoxLayout();
    _statusLabel = new ElaText(tr("Status: Waiting for connection"), this);
    _statusLabel->setTextPixelSize(12);
    _progressLabel = new ElaText("", this);
    _progressLabel->setTextPixelSize(12);
    statusLayout->addWidget(_statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(_progressLabel);
    mainLayout->addLayout(statusLayout);

    _statusGroupBox = new Widgets::GroupBox(tr("Execution Status"), this);
    QVBoxLayout* statusBoxLayout = new QVBoxLayout(_statusGroupBox);
    statusBoxLayout->setContentsMargins(15, 15, 15, 15);
    
    _executionStatusText = new ElaText(tr("Ready"), this);
    _executionStatusText->setTextPixelSize(14);
    _executionStatusText->setStyleSheet("color: #22c55e; font-weight: bold;");
    statusBoxLayout->addWidget(_executionStatusText);
    mainLayout->addWidget(_statusGroupBox, 0);

    _commandGroupBox = new Widgets::GroupBox(tr("Command List"), this);
    QVBoxLayout* commandLayout = new QVBoxLayout(_commandGroupBox);
    commandLayout->setContentsMargins(15, 15, 15, 15);
    commandLayout->setSpacing(8);

    _commandModel = new Models::CommandTableModel(this);
    _commandTableView = new ElaTableView(this);
    _commandTableView->setModel(_commandModel);
    _commandTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _commandTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _commandTableView->setAlternatingRowColors(true);
    _commandTableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _commandTableView->setMinimumHeight(150);
    _commandTableView->setMaximumHeight(350);
    _commandTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _commandTableView->verticalHeader()->setVisible(false);
    _commandTableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
    _commandTableView->setColumnWidth(0, 60);
    _commandTableView->setColumnWidth(1, 100);
    _commandTableView->setColumnWidth(2, 80);
    _commandTableView->setColumnWidth(3, 80);
    _commandTableView->setColumnWidth(4, 50);
    _commandTableView->setColumnWidth(5, 50);
    _commandTableView->setColumnWidth(6, 80);
    _commandTableView->setColumnWidth(7, 50);
    commandLayout->addWidget(_commandTableView, 1);

    _commandInputEdit = new ElaLineEdit(this);
    _commandInputEdit->setPlaceholderText(tr("Enter command or text..."));
    commandLayout->addWidget(_commandInputEdit, 0);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    _addCommandButton = new ElaPushButton(tr("Add"), this);
    _removeCommandButton = new ElaPushButton(tr("Delete"), this);
    _clearCommandsButton = new ElaPushButton(tr("Clear"), this);
    _moveUpButton = new ElaPushButton(tr("Up"), this);
    _moveDownButton = new ElaPushButton(tr("Down"), this);
    
    buttonLayout->addWidget(_addCommandButton);
    buttonLayout->addWidget(_removeCommandButton);
    buttonLayout->addWidget(_clearCommandsButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(_moveUpButton);
    buttonLayout->addWidget(_moveDownButton);
    
    commandLayout->addLayout(buttonLayout, 0);

    mainLayout->addWidget(_commandGroupBox, 1);

    _loopGroupBox = new Widgets::GroupBox(tr("Global Settings"), this);
    QHBoxLayout* loopLayout = new QHBoxLayout(_loopGroupBox);
    loopLayout->setContentsMargins(15, 15, 15, 15);
    loopLayout->setSpacing(10);

    _loopSendCheckBox = new ElaCheckBox(tr("Enable Loop"), this);
    _loopCountSpinBox = new ElaSpinBox(this);
    _loopCountSpinBox->setRange(1, 9999);
    _loopCountSpinBox->setValue(10);
    _loopCountSpinBox->setEnabled(false);
    _countLabel = new ElaText(tr("Count:"), this);
    _countLabel->setTextPixelSize(13);

    connect(_loopSendCheckBox, &ElaCheckBox::toggled, _loopCountSpinBox, &ElaSpinBox::setEnabled);

    loopLayout->addWidget(_loopSendCheckBox);
    loopLayout->addWidget(_countLabel);
    loopLayout->addWidget(_loopCountSpinBox);
    loopLayout->addStretch();

    mainLayout->addWidget(_loopGroupBox, 0);

    mainLayout->addStretch(0);

    initConnections();
}

void AutoSendPanel::initConnections()
{
    connect(_executeButton, &ElaPushButton::clicked, this, &AutoSendPanel::onExecuteButtonClicked);
    connect(_addCommandButton, &ElaPushButton::clicked, this, &AutoSendPanel::onAddButtonClicked);
    connect(_removeCommandButton, &ElaPushButton::clicked, this, &AutoSendPanel::onRemoveButtonClicked);
    connect(_clearCommandsButton, &ElaPushButton::clicked, this, &AutoSendPanel::onClearButtonClicked);
    connect(_moveUpButton, &ElaPushButton::clicked, this, &AutoSendPanel::onMoveUpButtonClicked);
    connect(_moveDownButton, &ElaPushButton::clicked, this, &AutoSendPanel::onMoveDownButtonClicked);
    connect(_loopSendCheckBox, &ElaCheckBox::toggled, this, &AutoSendPanel::onLoopCheckBoxToggled);
    connect(_loopCountSpinBox, QOverload<int>::of(&ElaSpinBox::valueChanged), this, &AutoSendPanel::onLoopCountChanged);
    connect(_commandInputEdit, &ElaLineEdit::returnPressed, this, &AutoSendPanel::onAddButtonClicked);
}

void AutoSendPanel::updateConnectionStatus(bool connected)
{
    _executeButton->setEnabled(connected);
    if (connected) {
        _statusLabel->setText(tr("Status: Connected"));
        if (_currentState == Idle) {
            updateExecutionState(Idle, tr("Ready - Click Execute to start"));
        }
    } else {
        _statusLabel->setText(tr("Status: Waiting for connection"));
        updateExecutionState(Idle, tr("Waiting for connection..."));
        if (_currentState == Running) {
            updateExecutionState(Error, tr("Connection lost"));
        }
    }
}

void AutoSendPanel::updateAutoSendStatus(bool enabled)
{
    if (enabled) {
        _executeButton->setText(tr("Stop"));
        _commandTableView->setEnabled(false);
        _commandInputEdit->setEnabled(false);
        _addCommandButton->setEnabled(false);
        _removeCommandButton->setEnabled(false);
        _clearCommandsButton->setEnabled(false);
        _moveUpButton->setEnabled(false);
        _moveDownButton->setEnabled(false);
        _loopSendCheckBox->setEnabled(false);
        _loopCountSpinBox->setEnabled(false);
    } else {
        _executeButton->setText(tr("Execute"));
        _commandTableView->setEnabled(true);
        _commandInputEdit->setEnabled(true);
        _addCommandButton->setEnabled(true);
        _removeCommandButton->setEnabled(true);
        _clearCommandsButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
        _moveDownButton->setEnabled(true);
        _loopSendCheckBox->setEnabled(true);
        if (_loopSendCheckBox->isChecked()) {
            _loopCountSpinBox->setEnabled(true);
        }
    }
}

void AutoSendPanel::updateSendProgress(int currentIndex, int totalCount)
{
    _progressLabel->setText(QString(tr("Cmd: %1/%2")).arg(currentIndex + 1).arg(totalCount));
}

void AutoSendPanel::updateLoopProgress(int currentLoop, int totalLoops)
{
    if (totalLoops > 0) {
        _progressLabel->setText(_progressLabel->text() + QString(tr(" (Loop: %1/%2)")).arg(currentLoop + 1).arg(totalLoops));
    }
}

void AutoSendPanel::showError(const QString& error)
{
    QMessageBox::warning(this, tr("Auto Send Error"), error);
}

void AutoSendPanel::updateExecutionState(ExecutionState state, const QString& message)
{
    _currentState = state;
    updateStateVisual(state);
    
    QString statusText = message;
    if (statusText.isEmpty()) {
        switch (state) {
        case Idle:
            statusText = tr("Ready");
            break;
        case Running:
            statusText = tr("Running...");
            break;
        case Paused:
            statusText = tr("Paused");
            break;
        case Completed:
            statusText = tr("Completed");
            break;
        case Error:
            statusText = tr("Error");
            break;
        }
    }
    
    _executionStatusText->setText(statusText);
}

void AutoSendPanel::updateStateVisual(ExecutionState state)
{
    QString color;
    switch (state) {
    case Idle:
        color = "#22c55e";
        break;
    case Running:
        color = "#3b82f6";
        break;
    case Paused:
        color = "#f59e0b";
        break;
    case Completed:
        color = "#22c55e";
        break;
    case Error:
        color = "#ef4444";
        break;
    }
    
    _executionStatusText->setStyleSheet(QString("color: %1; font-weight: bold;").arg(color));
}

void AutoSendPanel::clearCommandList()
{
    _commandModel->clearCommands();
}

void AutoSendPanel::addCommandItem(const QString& command)
{
    if (!command.trimmed().isEmpty()) {
        _commandModel->addCommand(command);
    }
}

void AutoSendPanel::removeCommandItem(int index)
{
    _commandModel->removeCommand(index);
}

QStringList AutoSendPanel::getCommandList() const
{
    return _commandModel->getAllCommands();
}

int AutoSendPanel::getCurrentCommandIndex() const
{
    return _commandTableView->currentIndex().row();
}

bool AutoSendPanel::isHexSend(int commandIndex) const
{
    Models::CommandItem item = _commandModel->getCommand(commandIndex);
    return item.isHexMode;
}

bool AutoSendPanel::isNewLineAppend(int commandIndex) const
{
    Models::CommandItem item = _commandModel->getCommand(commandIndex);
    return item.appendNewLine;
}

int AutoSendPanel::getPreDelay(int commandIndex) const
{
    Models::CommandItem item = _commandModel->getCommand(commandIndex);
    return item.preDelayMs;
}

int AutoSendPanel::getPostDelay(int commandIndex) const
{
    Models::CommandItem item = _commandModel->getCommand(commandIndex);
    return item.postDelayMs;
}

int AutoSendPanel::getExecuteCount(int commandIndex) const
{
    Models::CommandItem item = _commandModel->getCommand(commandIndex);
    return item.executeCount;
}

bool AutoSendPanel::isCommandEnabled(int commandIndex) const
{
    Models::CommandItem item = _commandModel->getCommand(commandIndex);
    return item.enabled;
}

int AutoSendPanel::getCommandCount() const
{
    return _commandModel->commandCount();
}

Models::CommandItem AutoSendPanel::getCommandItem(int index) const
{
    return _commandModel->getCommand(index);
}

bool AutoSendPanel::isLoopEnabled() const
{
    return _loopSendCheckBox->isChecked();
}

int AutoSendPanel::getLoopCount() const
{
    return _loopCountSpinBox->value();
}

void AutoSendPanel::onExecuteButtonClicked()
{
    emit executeRequested();
}

void AutoSendPanel::onAddButtonClicked()
{
    QString command = _commandInputEdit->text();
    if (!command.trimmed().isEmpty()) {
        emit addCommandRequested(command);
        _commandInputEdit->clear();
    }
}

void AutoSendPanel::onRemoveButtonClicked()
{
    QModelIndexList selected = _commandTableView->selectionModel()->selectedRows();
    if (!selected.isEmpty()) {
        int row = selected.first().row();
        emit removeCommandRequested(row);
    }
}

void AutoSendPanel::onClearButtonClicked()
{
    emit clearCommandsRequested();
}

void AutoSendPanel::onMoveUpButtonClicked()
{
    QModelIndexList selected = _commandTableView->selectionModel()->selectedRows();
    if (!selected.isEmpty()) {
        int row = selected.first().row();
        _commandModel->moveUp(row);
    }
}

void AutoSendPanel::onMoveDownButtonClicked()
{
    QModelIndexList selected = _commandTableView->selectionModel()->selectedRows();
    if (!selected.isEmpty()) {
        int row = selected.first().row();
        _commandModel->moveDown(row);
    }
}

void AutoSendPanel::onLoopCheckBoxToggled(bool checked)
{
    emit loopOptionsChanged(checked, _loopCountSpinBox->value());
}

void AutoSendPanel::onLoopCountChanged(int value)
{
    emit loopOptionsChanged(_loopSendCheckBox->isChecked(), value);
}

void AutoSendPanel::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QWidget::changeEvent(event);
}

void AutoSendPanel::retranslateUi()
{
    _titleText->setText(tr("Auto Command Send"));
    _executeButton->setText(_currentState == Running ? tr("Stop") : tr("Execute"));
    _statusLabel->setText(tr("Status: Waiting for connection"));
    _executionStatusText->setText(tr("Ready"));
    
    if (_statusGroupBox) {
        _statusGroupBox->setTitle(tr("Execution Status"));
    }
    if (_commandGroupBox) {
        _commandGroupBox->setTitle(tr("Command List"));
    }
    if (_loopGroupBox) {
        _loopGroupBox->setTitle(tr("Global Settings"));
    }
    
    _commandInputEdit->setPlaceholderText(tr("Enter command or text..."));
    _addCommandButton->setText(tr("Add"));
    _removeCommandButton->setText(tr("Delete"));
    _clearCommandsButton->setText(tr("Clear"));
    _moveUpButton->setText(tr("Up"));
    _moveDownButton->setText(tr("Down"));
    _loopSendCheckBox->setText(tr("Enable Loop"));
    _countLabel->setText(tr("Count:"));
    
    if (_commandModel) {
        _commandModel->retranslateHeaders();
    }
}

}
}
