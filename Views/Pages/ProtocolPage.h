#ifndef PROTOCOLPAGE_H
#define PROTOCOLPAGE_H

#include "ElaScrollPage.h"
#include "ElaPlainTextEdit.h"
#include "ElaPushButton.h"
#include "ElaToggleSwitch.h"
#include "Widgets/ComboBox.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>

namespace Services {
namespace Protocol {
class ProtocolParser;
}
}

namespace Views {
namespace Pages {

class ProtocolPage : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit ProtocolPage(QWidget* parent = nullptr);
    ~ProtocolPage() override;

protected:
    virtual void changeEvent(QEvent* event) override;

public slots:
    void retranslateUi();

private:
    void initUI();
    void initConnections();
    void onParseClicked();
    void onCopyClicked();
    void onClearClicked();
    void onProtocolChanged(int index);
    void onAutoParseStateChanged(bool enabled);
    void onRawDataChanged();
    void updateLog(const QString& message);
    bool validateHexInput(const QString& input);

    Widgets::ComboBox* _protocolComboBox;
    ElaToggleSwitch* _autoParseSwitch;
    ElaPlainTextEdit* _rawDataTextEdit;
    ElaPlainTextEdit* _parsedResultTextEdit;
    ElaPushButton* _parseButton;
    ElaPushButton* _copyButton;
    ElaPushButton* _clearButton;
    ElaPlainTextEdit* _logTextEdit;
    ElaText* _protocolLabel;
    ElaText* _autoParseLabel;
    ElaText* _rawDataLabel;
    ElaText* _parsedLabel;
    ElaText* _statusText;

    std::unique_ptr<Services::Protocol::ProtocolParser> _currentParser;
};

}
}

#endif // PROTOCOLPAGE_H