#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include "ElaScrollPage.h"
#include "ElaPushButton.h"
#include "Widgets/ComboBox.h"

class ElaComboBox;

namespace Views {
namespace Pages {

class SettingPage : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget* parent = nullptr);
    ~SettingPage() override;

protected:
    virtual void changeEvent(QEvent* event) override;

public slots:
    void retranslateUi();

private:
    void initUI();
    void onLanguageChanged();

private:
    Widgets::ComboBox* _languageCombo;
    ElaText* _titleLabel;
    ElaText* _languageLabel;
    ElaPushButton* _applyButton;
    ElaText* _infoLabel;
};

}
}

#endif // SETTINGPAGE_H