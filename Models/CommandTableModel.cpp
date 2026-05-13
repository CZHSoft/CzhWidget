#include "CommandTableModel.h"

namespace Models {

CommandTableModel::CommandTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

CommandTableModel::~CommandTableModel()
{
}

int CommandTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return _commands.size();
}

int CommandTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return 8;
}

QVariant CommandTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= _commands.size()) {
        return QVariant();
    }

    const CommandItem& item = _commands.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case IndexColumn:
            return item.index;
        case CommandColumn:
            return item.command;
        case PreDelayColumn:
            return item.preDelayMs;
        case PostDelayColumn:
            return item.postDelayMs;
        case HexModeColumn:
            return item.isHexMode ? "true" : "false";
        case NewLineColumn:
            return item.appendNewLine ? "true" : "false";
        case ExecuteCountColumn:
            return item.executeCount;
        case EnabledColumn:
            return item.enabled ? "true" : "false";
        }
    }

    if (role == Qt::CheckStateRole) {
        switch (index.column()) {
        case HexModeColumn:
            return item.isHexMode ? Qt::Checked : Qt::Unchecked;
        case NewLineColumn:
            return item.appendNewLine ? Qt::Checked : Qt::Unchecked;
        case EnabledColumn:
            return item.enabled ? Qt::Checked : Qt::Unchecked;
        }
    }

    return QVariant();
}

bool CommandTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= _commands.size()) {
        return false;
    }

    if (role == Qt::EditRole) {
        CommandItem& item = _commands[index.row()];
        switch (index.column()) {
        case IndexColumn:
            item.index = value.toInt();
            break;
        case CommandColumn:
            item.command = value.toString();
            break;
        case PreDelayColumn:
            item.preDelayMs = qMax(0, value.toInt());
            break;
        case PostDelayColumn:
            item.postDelayMs = qMax(0, value.toInt());
            break;
        case HexModeColumn:
            item.isHexMode = parseBoolString(value.toString());
            break;
        case NewLineColumn:
            item.appendNewLine = parseBoolString(value.toString());
            break;
        case ExecuteCountColumn:
            item.executeCount = qMax(1, value.toInt());
            break;
        case EnabledColumn:
            item.enabled = parseBoolString(value.toString());
            break;
        default:
            return false;
        }
        emit dataChanged(index, index, {role, Qt::CheckStateRole});
        return true;
    }

    if (role == Qt::CheckStateRole) {
        CommandItem& item = _commands[index.row()];
        Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
        bool checked = (state == Qt::Checked);
        
        switch (index.column()) {
        case HexModeColumn:
            item.isHexMode = checked;
            break;
        case NewLineColumn:
            item.appendNewLine = checked;
            break;
        case EnabledColumn:
            item.enabled = checked;
            break;
        default:
            return false;
        }
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

Qt::ItemFlags CommandTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == CommandColumn || 
        index.column() == PreDelayColumn || 
        index.column() == PostDelayColumn ||
        index.column() == ExecuteCountColumn) {
        flags |= Qt::ItemIsEditable;
    } else if (index.column() == HexModeColumn || 
               index.column() == NewLineColumn || 
               index.column() == EnabledColumn) {
        flags |= Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
    }

    return flags;
}

QVariant CommandTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case IndexColumn:
            return tr("Index");
        case CommandColumn:
            return tr("Command");
        case PreDelayColumn:
            return tr("Pre-delay(ms)");
        case PostDelayColumn:
            return tr("Post-delay(ms)");
        case HexModeColumn:
            return tr("HEX");
        case NewLineColumn:
            return tr("Newline");
        case ExecuteCountColumn:
            return tr("Execute Count");
        case EnabledColumn:
            return tr("Enabled");
        }
    }

    if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

void CommandTableModel::retranslateHeaders()
{
    Q_EMIT headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);
}

void CommandTableModel::addCommand(const QString& command)
{
    beginInsertRows(QModelIndex(), _commands.size(), _commands.size());
    CommandItem item;
    item.index = _commands.size() + 1;
    item.command = command;
    item.enabled = true;
    item.isHexMode = false;
    item.appendNewLine = true;
    item.preDelayMs = 0;
    item.postDelayMs = 1000;
    item.executeCount = 1;
    _commands.append(item);
    endInsertRows();
    emit commandAdded(command);
}

void CommandTableModel::removeCommand(int row)
{
    if (row >= 0 && row < _commands.size()) {
        beginRemoveRows(QModelIndex(), row, row);
        _commands.removeAt(row);
        for (int i = row; i < _commands.size(); ++i) {
            _commands[i].index = i + 1;
        }
        endRemoveRows();
        emit commandRemoved(row);
    }
}

void CommandTableModel::clearCommands()
{
    if (!_commands.isEmpty()) {
        beginResetModel();
        _commands.clear();
        endResetModel();
    }
}

QStringList CommandTableModel::getAllCommands() const
{
    QStringList commands;
    for (const CommandItem& item : _commands) {
        if (item.enabled) {
            commands.append(item.command);
        }
    }
    return commands;
}

QList<CommandItem> CommandTableModel::getAllCommandItems() const
{
    return _commands;
}

CommandItem CommandTableModel::getCommand(int row) const
{
    if (row >= 0 && row < _commands.size()) {
        return _commands.at(row);
    }
    return CommandItem();
}

bool CommandTableModel::moveUp(int row)
{
    if (row <= 0 || row >= _commands.size()) {
        return false;
    }

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
    _commands.swapItemsAt(row, row - 1);
    
    int temp = _commands[row].index;
    _commands[row].index = _commands[row - 1].index;
    _commands[row - 1].index = temp;
    
    endMoveRows();
    emit commandMoved(row, row - 1);
    return true;
}

bool CommandTableModel::moveDown(int row)
{
    if (row < 0 || row >= _commands.size() - 1) {
        return false;
    }

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2);
    _commands.swapItemsAt(row, row + 1);
    
    int temp = _commands[row].index;
    _commands[row].index = _commands[row + 1].index;
    _commands[row + 1].index = temp;
    
    endMoveRows();
    emit commandMoved(row, row + 1);
    return true;
}

bool CommandTableModel::parseBoolString(const QString& str) const
{
    QString lower = str.toLower().trimmed();
    return (lower == "true" || lower == "1" || lower == "yes" || lower == "是");
}

}
