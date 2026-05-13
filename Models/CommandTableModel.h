#ifndef COMMANDTABLEMODEL_H
#define COMMANDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

namespace Models {

class CommandItem
{
public:
    CommandItem() 
        : index(0), enabled(true), isHexMode(false), appendNewLine(true),
          preDelayMs(0), postDelayMs(1000), executeCount(1) {}
    
    CommandItem(int idx, const QString& cmd) 
        : index(idx), command(cmd), enabled(true), isHexMode(false), 
          appendNewLine(true), preDelayMs(0), postDelayMs(1000), executeCount(1) {}

    int index;
    QString command;
    bool enabled;
    bool isHexMode;
    bool appendNewLine;
    int preDelayMs;
    int postDelayMs;
    int executeCount;
};

class CommandTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnRole {
        IndexColumn = 0,
        CommandColumn = 1,
        PreDelayColumn = 2,
        PostDelayColumn = 3,
        HexModeColumn = 4,
        NewLineColumn = 5,
        ExecuteCountColumn = 6,
        EnabledColumn = 7
    };

    explicit CommandTableModel(QObject* parent = nullptr);
    ~CommandTableModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addCommand(const QString& command);
    void removeCommand(int row);
    void clearCommands();
    QStringList getAllCommands() const;
    QList<CommandItem> getAllCommandItems() const;
    CommandItem getCommand(int row) const;
    int commandCount() const { return _commands.size(); }
    
    bool moveUp(int row);
    bool moveDown(int row);
    
    void retranslateHeaders();

signals:
    void commandAdded(const QString& command);
    void commandRemoved(int index);
    void commandMoved(int fromRow, int toRow);

private:
    bool parseBoolString(const QString& str) const;
    QList<CommandItem> _commands;
};

}

#endif
