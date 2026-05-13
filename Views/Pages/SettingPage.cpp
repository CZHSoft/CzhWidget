#include "Views/Pages/SettingPage.h"
#include "Services/I18n/LanguageManager.h"
#include "ElaText.h"
#include "ElaComboBox.h"
#include "ElaPushButton.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMessageBox>

namespace Views {
namespace Pages {

SettingPage::SettingPage(QWidget* parent)
    : ElaScrollPage(parent)
    , _languageCombo(nullptr)
    , _titleLabel(nullptr)
    , _languageLabel(nullptr)
    , _applyButton(nullptr)
    , _infoLabel(nullptr)
{
    setWindowTitle(tr("Setting"));
    setTitleVisible(false);

    initUI();
}

void SettingPage::initUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    _titleLabel = new ElaText(tr("Settings"), this);
    _titleLabel->setWordWrap(false);
    _titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(_titleLabel);

    QWidget* languageWidget = new QWidget(this);
    QHBoxLayout* languageLayout = new QHBoxLayout(languageWidget);
    languageLayout->setContentsMargins(10, 10, 10, 10);
    languageLayout->setSpacing(15);

    _languageLabel = new ElaText(tr("Language:"), this);
    _languageLabel->setTextPixelSize(14);
    languageLayout->addWidget(_languageLabel);

    _languageCombo = new Widgets::ComboBox(this);
    _languageCombo->addItems({
        tr("Chinese"),
        tr("English"),
        tr("Korean"),
        tr("Japanese")
    });

    QString currentLang = Services::I18n::LanguageManager::instance()->currentLanguage();
    if (currentLang == "zh_CN") {
        _languageCombo->setCurrentIndex(0);
    } else if (currentLang == "ko_KR") {
        _languageCombo->setCurrentIndex(2);
    } else if (currentLang == "ja_JP") {
        _languageCombo->setCurrentIndex(3);
    } else {
        _languageCombo->setCurrentIndex(1);
    }
    languageLayout->addWidget(_languageCombo);

    _applyButton = new ElaPushButton(tr("Apply"), this);
    _applyButton->setFixedWidth(80);
    languageLayout->addWidget(_applyButton);

    mainLayout->addWidget(languageWidget);

    _infoLabel = new ElaText(tr("Full functionality will be added in future versions"), this);
    _infoLabel->setTextPixelSize(12);
    _infoLabel->setStyleSheet("color: #666;");
    mainLayout->addWidget(_infoLabel);

    mainLayout->addStretch();

    addCentralWidget(centralWidget, true, false, 0);

    connect(_applyButton, &ElaPushButton::clicked, this, &SettingPage::onLanguageChanged);
}

void SettingPage::onLanguageChanged()
{
    QString locale;
    switch (_languageCombo->currentIndex()) {
    case 0:
        locale = "zh_CN";
        break;
    case 1:
        locale = "en";
        break;
    case 2:
        locale = "ko_KR";
        break;
    case 3:
        locale = "ja_JP";
        break;
    default:
        locale = "zh_CN";
    }

    int ret = QMessageBox::information(this, tr("Confirm Language Change"),
        tr("Are you sure you want to change the language?"),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);

    if (ret == QMessageBox::Ok) {
        Services::I18n::LanguageManager::instance()->switchLanguage(locale);
    }
}

SettingPage::~SettingPage()
{
}

void SettingPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    ElaScrollPage::changeEvent(event);
}

void SettingPage::retranslateUi()
{
    setWindowTitle(tr("Setting"));
    
    if (_titleLabel) {
        _titleLabel->setText(tr("Settings"));
    }
    if (_languageLabel) {
        _languageLabel->setText(tr("Language:"));
    }
    if (_languageCombo) {
        int currentIndex = _languageCombo->currentIndex();
        _languageCombo->clear();
        _languageCombo->addItems({
            tr("Chinese"),
            tr("English"),
            tr("Korean"),
            tr("Japanese")
        });
        _languageCombo->setCurrentIndex(currentIndex);
    }
    if (_applyButton) {
        _applyButton->setText(tr("Apply"));
    }
    if (_infoLabel) {
        _infoLabel->setText(tr("Full functionality will be added in future versions"));
    }
}

}
}
