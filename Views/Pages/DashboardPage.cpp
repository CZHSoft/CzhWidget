#include "Views/Pages/DashboardPage.h"
#include "Widgets/FunctionNavCard.h"
#include "ElaText.h"
#include "ElaImageCard.h"
#include "ElaFlowLayout.h"
#include "ElaMessageBar.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QEvent>

namespace Views {
namespace Pages {

DashboardPage::DashboardPage(QWidget* parent)
    : ElaScrollPage(parent)
    , _backgroundCard(nullptr)
    , _welcomeText(nullptr)
    , _descriptionText(nullptr)
    , _sectionTitle(nullptr)
{
    setWindowTitle(tr("Dashboard"));
    setTitleVisible(false);

    initUI();
}

void DashboardPage::initUI()
{
    initWelcomeSection();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName("dashboardCentralWidget");
    centralWidget->setStyleSheet(R"(
        #dashboardCentralWidget {
            background-image: url(:/Resources/pic/background.jpg);
            background-repeat: no-repeat;
            background-position: center;
        }
    )");
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* contentWidget = new QWidget(this);
    //contentWidget->setStyleSheet("background-color: rgba(255, 255, 255, 0.2);");
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(_backgroundCard);
    contentLayout->addWidget(initNavigationCards());

    mainLayout->addWidget(contentWidget);

    addCentralWidget(centralWidget, true, false, 0);
}

void DashboardPage::initWelcomeSection()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timeGreeting;
    int hour = currentDateTime.time().hour();
    if (hour >= 5 && hour < 12) {
        timeGreeting = tr("Good Morning");
    } else if (hour >= 12 && hour < 14) {
        timeGreeting = tr("Good Noon");
    } else if (hour >= 14 && hour < 18) {
        timeGreeting = tr("Good Afternoon");
    } else if (hour >= 18 && hour < 22) {
        timeGreeting = tr("Good Evening");
    } else {
        timeGreeting = tr("Good Night");
    }

    _welcomeText = new ElaText(QString(tr("%1, welcome to CzhWidget")).arg(timeGreeting), this);
    _welcomeText->setTextPixelSize(28);
    _welcomeText->setStyleSheet("font-weight: bold;");

    _descriptionText = new ElaText(tr("Professional communication monitoring and protocol analysis tool"), this);
    _descriptionText->setTextPixelSize(16);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setContentsMargins(30, 20, 0, 0);
    titleLayout->setSpacing(8);
    titleLayout->addWidget(_welcomeText);
    titleLayout->addWidget(_descriptionText);

    _backgroundCard = new ElaImageCard(this);
    _backgroundCard->setBorderRadius(12);
    _backgroundCard->setFixedHeight(180);
    //_backgroundCard->setCardImage(QImage(":/Resources/pic/background.jpg"));

    QVBoxLayout* backgroundLayout = new QVBoxLayout(_backgroundCard);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);
    backgroundLayout->addLayout(titleLayout);
}

QWidget* DashboardPage::initNavigationCards()
{
    QWidget* cardsContainer = new QWidget(this);
    QVBoxLayout* cardsLayout = new QVBoxLayout(cardsContainer);
    cardsLayout->setContentsMargins(20, 20, 20, 20);
    cardsLayout->setSpacing(10);

    _sectionTitle = new ElaText(tr("Function Navigation"), this);
    _sectionTitle->setTextPixelSize(20);
    _sectionTitle->setStyleSheet("font-weight: bold;");
    cardsLayout->addWidget(_sectionTitle);

    ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 15, 15);
    flowLayout->setContentsMargins(0, 10, 0, 0);
    flowLayout->setIsAnimation(true);

    struct NavItemData {
        QString pageName;
        QString iconName;
        const char* titleKey;
        const char* descriptionKey;
    };
    
    QList<NavItemData> navItems = {
        {"Monitor", "MonitorWaveform", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Monitor"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Real-time monitoring of serial port, network and other communication status")},
        {"Protocol", "Message", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Protocol"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Supports multiple communication protocol parsing and configuration")},
        {"Tools", "Wrench", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Tools"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Provides practical tools for data conversion and calculation")},
        {"Logs", "FileLines", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Logs"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "View and manage system operation logs")},
        {"Setting", "GearComplex", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Setting"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Configure application parameters")},
        {"About", "User", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "About"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "View product information and version updates")}
    };

    for (const NavItemData& item : navItems) {
        Widgets::FunctionNavCard* card = new Widgets::FunctionNavCard(this);
        card->setPageName(item.pageName);

        QString description = tr(item.descriptionKey);

        card->setTitle(tr(item.titleKey));
        card->setDescription(description);

        connect(card, &Widgets::FunctionNavCard::cardClicked, this, [this, item]() {
            Q_EMIT navigateToPage(item.pageName);
            ElaMessageBar::success(ElaMessageBarType::BottomRight, tr("Navigation"), 
                QString(tr("Entering %1...")).arg(tr(item.titleKey)), 1500);
        });

        _navigationPageNames.append(item.pageName);
        _navCards.append(card);
        flowLayout->addWidget(card);
    }

    cardsLayout->addLayout(flowLayout);

    return cardsContainer;
}

DashboardPage::~DashboardPage()
{
}

void DashboardPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    ElaScrollPage::changeEvent(event);
}

void DashboardPage::retranslateUi()
{
    setWindowTitle(tr("Dashboard"));
    
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timeGreeting;
    int hour = currentDateTime.time().hour();
    if (hour >= 5 && hour < 12) {
        timeGreeting = tr("Good Morning");
    } else if (hour >= 12 && hour < 14) {
        timeGreeting = tr("Good Noon");
    } else if (hour >= 14 && hour < 18) {
        timeGreeting = tr("Good Afternoon");
    } else if (hour >= 18 && hour < 22) {
        timeGreeting = tr("Good Evening");
    } else {
        timeGreeting = tr("Good Night");
    }
    
    if (_welcomeText) {
        _welcomeText->setText(QString(tr("%1, welcome to CzhWidget")).arg(timeGreeting));
    }
    if (_descriptionText) {
        _descriptionText->setText(tr("Professional communication monitoring and protocol analysis tool"));
    }
    if (_sectionTitle) {
        _sectionTitle->setText(tr("Function Navigation"));
    }
    
    struct NavItemData {
        QString pageName;
        QString iconName;
        const char* titleKey;
        const char* descriptionKey;
    };
    
    QList<NavItemData> navItems = {
        {"Monitor", "MonitorWaveform", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Monitor"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Real-time monitoring of serial port, network and other communication status")},
        {"Protocol", "Message", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Protocol"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Supports multiple communication protocol parsing and configuration")},
        {"Tools", "Wrench", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Tools"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Provides practical tools for data conversion and calculation")},
        {"Logs", "FileLines", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Logs"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "View and manage system operation logs")},
        {"Setting", "GearComplex", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Setting"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "Configure application parameters")},
        {"About", "User", QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "About"), QT_TRANSLATE_NOOP("Views::Pages::DashboardPage", "View product information and version updates")}
    };
    
    for (int i = 0; i < _navCards.size() && i < navItems.size(); ++i) {
        Widgets::FunctionNavCard* card = _navCards[i];
        NavItemData item = navItems[i];
        card->setPageName(item.pageName);
        card->setTitle(tr(item.titleKey));
        card->setDescription(tr(item.descriptionKey));
    }
}

}
}
