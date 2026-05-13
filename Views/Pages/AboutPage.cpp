#include "Views/Pages/AboutPage.h"
#include "ElaText.h"
#include "ElaImageCard.h"
#include "ElaFlowLayout.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QUrl>
#include <QDesktopServices>
#include <QEvent>
#include <QMouseEvent>

namespace Views {
namespace Pages {

AboutPage::AboutPage(QWidget* parent)
    : ElaScrollPage(parent)
    , _titleLabel(nullptr)
    , _appNameLabel(nullptr)
    , _versionLabel(nullptr)
    , _authorNameLabel(nullptr)
    , _personalLinkLabel(nullptr)
    , _githubLinkLabel(nullptr)
    , _acknowledgementTitle(nullptr)
    , _qtLinkLabel(nullptr)
    , _elaWidgetToolsLinkLabel(nullptr)
    , _log4QtLinkLabel(nullptr)
    , _treaLinkLabel(nullptr)
    , _specialThanksLabel(nullptr)
{
    setWindowTitle(tr("About"));
    setTitleVisible(false);

    initUI();
}

void AboutPage::initUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(30);

    initProductSection(mainLayout);
    initAuthorSection(mainLayout);
    initAcknowledgementSection(mainLayout);

    addCentralWidget(centralWidget, true, false, 0);
}

void AboutPage::initProductSection(QVBoxLayout* parentLayout)
{
    QWidget* productCard = new QWidget(this);
    productCard->setStyleSheet(R"(
        QWidget {
            background-color: white;
            border-radius: 12px;
        }
    )");
    
    QVBoxLayout* cardLayout = new QVBoxLayout(productCard);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(15);

    _titleLabel = new ElaText(tr("Product Information"), this);
    _titleLabel->setWordWrap(false);
    _titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    cardLayout->addWidget(_titleLabel);

    _appNameLabel = new ElaText(tr("CzhWidget"), this);
    _appNameLabel->setAlignment(Qt::AlignCenter);
    _appNameLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a73e8;");
    cardLayout->addWidget(_appNameLabel);

    _versionLabel = new ElaText(tr("Version 1.0.0"), this);
    _versionLabel->setAlignment(Qt::AlignCenter);
    _versionLabel->setStyleSheet("font-size: 14px; color: #666;");
    cardLayout->addWidget(_versionLabel);

    _descriptionLabel = new ElaText(tr("Professional communication monitoring and protocol analysis tool"), this);
    _descriptionLabel->setAlignment(Qt::AlignCenter);
    _descriptionLabel->setStyleSheet("font-size: 13px; color: #888;");
    cardLayout->addWidget(_descriptionLabel);

    parentLayout->addWidget(productCard);
}

void AboutPage::initAuthorSection(QVBoxLayout* parentLayout)
{
    QWidget* authorCard = new QWidget(this);
    authorCard->setStyleSheet(R"(
        QWidget {
            background-color: white;
            border-radius: 12px;
        }
    )");
    
    QVBoxLayout* cardLayout = new QVBoxLayout(authorCard);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(20);

    _sectionTitle = new ElaText(tr("Author Information"), this);
    _sectionTitle->setWordWrap(false);
    _sectionTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    cardLayout->addWidget(_sectionTitle);

    _authorNameLabel = new ElaText(tr("Author: CZHsoft"), this);
    _authorNameLabel->setStyleSheet("font-size: 14px; color: #333;");
    cardLayout->addWidget(_authorNameLabel);

    QHBoxLayout* linkLayout = new QHBoxLayout();
    linkLayout->setSpacing(20);

    _personalLinkLabel = createLinkLabel(tr("Personal Homepage"), "http://my.tcpic.cn/");
    linkLayout->addWidget(_personalLinkLabel);

    _githubLinkLabel = createLinkLabel(tr("GitHub Homepage"), "https://github.com/CZHsoft/");
    linkLayout->addWidget(_githubLinkLabel);

    cardLayout->addLayout(linkLayout);

    _worksTitle = new ElaText(tr("My Works"), this);
    _worksTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    cardLayout->addWidget(_worksTitle);

    ElaFlowLayout* worksLayout = new ElaFlowLayout(0, 15, 15);
    worksLayout->setContentsMargins(0, 10, 0, 0);

    addWorkCard(worksLayout, ":/Resources/pic/battery_cover.png", tr("Battery Management"), "http://my.tcpic.cn/battery/");
    addWorkCard(worksLayout, ":/Resources/pic/tracking_cover.png", tr("Tracking System"), "http://my.tcpic.cn/tracking/");
    addWorkCard(worksLayout, ":/Resources/pic/daoren_cover.png", tr("DaoRen Project"), "http://my.tcpic.cn/daoren/");

    cardLayout->addLayout(worksLayout);

    parentLayout->addWidget(authorCard);

    parentLayout->addStretch();
}

QLabel* AboutPage::createLinkLabel(const QString& text, const QString& url)
{
    QLabel* label = new QLabel(this);
    label->setText(QString("<a href=\"%1\" style=\"color: #1a73e8; text-decoration: none; font-size: 14px;\">%2</a>").arg(url).arg(text));
    label->setOpenExternalLinks(true);
    label->setCursor(Qt::PointingHandCursor);
    return label;
}

void AboutPage::addWorkCard(ElaFlowLayout* parentLayout, const QString& imagePath, const QString& title, const QString& url)
{
    ElaImageCard* card = new ElaImageCard(this);
    card->setBorderRadius(8);
    card->setFixedSize(200, 150);
    card->setCardImage(QImage(imagePath));
    card->setCursor(Qt::PointingHandCursor);

    // ElaText* titleLabel = new ElaText(title, card);
    // titleLabel->setAlignment(Qt::AlignCenter);
    // titleLabel->setStyleSheet("font-size: 13px; color: white; background-color: rgba(0,0,0,0.5); padding: 5px;");
    // card->setTitle(titleLabel);

    card->installEventFilter(this);
    _workCards.insert(card, url);

    parentLayout->addWidget(card);
}

void AboutPage::initAcknowledgementSection(QVBoxLayout* parentLayout)
{
    QWidget* acknowledgementCard = new QWidget(this);
    acknowledgementCard->setStyleSheet(R"(
        QWidget {
            background-color: white;
            border-radius: 12px;
        }
    )");

    QVBoxLayout* cardLayout = new QVBoxLayout(acknowledgementCard);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(15);

    _acknowledgementTitle = new ElaText(tr("Acknowledgements"), this);
    _acknowledgementTitle->setWordWrap(false);
    _acknowledgementTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    cardLayout->addWidget(_acknowledgementTitle);

    QVBoxLayout* linksLayout = new QVBoxLayout();
    linksLayout->setSpacing(10);

    _qtLinkLabel = createLinkLabel(tr("Qt"), "https://www.qt.io/");
    linksLayout->addWidget(_qtLinkLabel);

    _elaWidgetToolsLinkLabel = createLinkLabel(tr("ElaWidgetTools"), "https://github.com/Liniyous/ElaWidgetTools");
    linksLayout->addWidget(_elaWidgetToolsLinkLabel);

    _log4QtLinkLabel = createLinkLabel(tr("Log4Qt"), "https://github.com/MEONMedical/Log4Qt");
    linksLayout->addWidget(_log4QtLinkLabel);

    _treaLinkLabel = createLinkLabel(tr("Trea"), "https://www.trae.ai/");
    linksLayout->addWidget(_treaLinkLabel);

    _specialThanksLabel = new ElaText(tr("Special thanks to my father"), this);
    _specialThanksLabel->setStyleSheet("font-size: 14px; color: #666; font-style: italic;");
    linksLayout->addWidget(_specialThanksLabel);

    cardLayout->addLayout(linksLayout);

    parentLayout->addWidget(acknowledgementCard);
}

bool AboutPage::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        ElaImageCard* card = qobject_cast<ElaImageCard*>(obj);
        if (card && _workCards.contains(card)) {
            QDesktopServices::openUrl(QUrl(_workCards[card]));
            return true;
        }
    }
    return ElaScrollPage::eventFilter(obj, event);
}

AboutPage::~AboutPage()
{
}

void AboutPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    ElaScrollPage::changeEvent(event);
}

void AboutPage::retranslateUi()
{
    setWindowTitle(tr("About"));

    if (_titleLabel) {
        _titleLabel->setText(tr("Product Information"));
    }
    if (_appNameLabel) {
        _appNameLabel->setText(tr("CzhWidget"));
    }
    if (_versionLabel) {
        _versionLabel->setText(tr("Version 1.0.0"));
    }
    if (_descriptionLabel) {
        _descriptionLabel->setText(tr("Professional communication monitoring and protocol analysis tool"));
    }
    if (_sectionTitle) {
        _sectionTitle->setText(tr("Author Information"));
    }
    if (_authorNameLabel) {
        _authorNameLabel->setText(tr("Author: CZHsoft"));
    }
    if (_worksTitle) {
        _worksTitle->setText(tr("My Works"));
    }
    if (_acknowledgementTitle) {
        _acknowledgementTitle->setText(tr("Acknowledgements"));
    }
    if (_personalLinkLabel) {
        _personalLinkLabel->setText(QString("<a href=\"http://my.tcpic.cn/\" style=\"color: #1a73e8; text-decoration: none; font-size: 14px;\">%1</a>").arg(tr("Personal Homepage")));
    }
    if (_githubLinkLabel) {
        _githubLinkLabel->setText(QString("<a href=\"https://github.com/CZHsoft/\" style=\"color: #1a73e8; text-decoration: none; font-size: 14px;\">%1</a>").arg(tr("GitHub Homepage")));
    }
    if (_qtLinkLabel) {
        _qtLinkLabel->setText(QString("<a href=\"https://www.qt.io/\" style=\"color: #1a73e8; text-decoration: none; font-size: 14px;\">%1</a>").arg(tr("Qt")));
    }
    if (_elaWidgetToolsLinkLabel) {
        _elaWidgetToolsLinkLabel->setText(QString("<a href=\"https://github.com/Liniyous/ElaWidgetTools\" style=\"color: #1a73e8; text-decoration: none; font-size: 14px;\">%1</a>").arg(tr("ElaWidgetTools")));
    }
    if (_log4QtLinkLabel) {
        _log4QtLinkLabel->setText(QString("<a href=\"https://github.com/MEONMedical/Log4Qt\" style=\"color: #1a73e8; text-decoration: none; font-size: 14px;\">%1</a>").arg(tr("Log4Qt")));
    }
    if (_treaLinkLabel) {
        _treaLinkLabel->setText(QString("<a href=\"https://www.trae.ai/\" style=\"color: #1a73e8; text-decoration: none; font-size: 14px;\">%1</a>").arg(tr("Trea")));
    }
    if (_specialThanksLabel) {
        _specialThanksLabel->setText(tr("Special thanks to my father"));
    }
}

}
}
