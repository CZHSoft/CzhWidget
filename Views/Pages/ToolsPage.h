#ifndef TOOLSPAGE_H
#define TOOLSPAGE_H

#include "ElaScrollPage.h"
#include "ElaTabWidget.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaToggleSwitch.h"
#include "ElaPlainTextEdit.h"
#include "ElaListView.h"
#include "Widgets/GroupBox.h"
#include "Widgets/ComboBox.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QEvent>

namespace Views {
namespace Pages {

class ToolsPage : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit ToolsPage(QWidget* parent = nullptr);
    ~ToolsPage() override;

protected:
    virtual void changeEvent(QEvent* event) override;

private slots:
    void onScanButtonClicked();
    void onCalculateButtonClicked();
    void onConvertButtonClicked();
    void retranslateUi();

private:
    void initUI();
    void initGeneralSettingsTab();
    void initCommandManagementTab();
    void initExtendedToolsTab();

    quint16 calculateCRC16Modbus(const QByteArray& data);
    quint16 calculateCRC16CCITT(const QByteArray& data);
    quint32 calculateCRC32(const QByteArray& data);
    void scanPort(const QString& ip, int port);

    ElaTabWidget* _tabWidget;

    Widgets::ComboBox* _themeComboBox;
    Widgets::ComboBox* _fontSizeComboBox;
    ElaLineEdit* _logPathEdit;
    ElaToggleSwitch* _autoSaveSwitch;
    ElaText* _themeLabel;
    ElaText* _fontSizeLabel;
    ElaText* _logPathLabel;
    ElaText* _autoSaveLabel;

    ElaListView* _commandListView;
    ElaLineEdit* _commandNameEdit;
    ElaLineEdit* _commandDataEdit;
    ElaPushButton* _addCommandButton;
    ElaPushButton* _deleteCommandButton;
    ElaPushButton* _importCommandButton;
    ElaPushButton* _exportCommandButton;
    ElaText* _listLabel;
    ElaText* _editLabel;
    ElaText* _nameLabel;
    ElaText* _dataLabel;

    ElaLineEdit* _portScanEdit;
    ElaPushButton* _scanButton;
    ElaPlainTextEdit* _scanResultEdit;
    ElaText* _portLabel;
    
    Widgets::GroupBox* _portScanGroupBox;

    ElaLineEdit* _crcInputEdit;
    Widgets::ComboBox* _crcTypeComboBox;
    ElaPushButton* _calculateButton;
    ElaLineEdit* _crcResultEdit;
    ElaText* _crcInputLabel;
    ElaText* _crcResultLabel;
    
    Widgets::GroupBox* _crcGroupBox;

    ElaLineEdit* _convertInputEdit;
    Widgets::ComboBox* _convertFromComboBox;
    Widgets::ComboBox* _convertToComboBox;
    ElaPushButton* _convertButton;
    ElaLineEdit* _convertResultEdit;
    ElaText* _convertInputLabel;
    ElaText* _fromLabel;
    ElaText* _toLabel;
    ElaText* _convertResultLabel;
    
    Widgets::GroupBox* _convertGroupBox;
    
    int _scanningPort;
    QString _scanningIp;
};

}
}

#endif // TOOLSPAGE_H