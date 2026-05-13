#include "Views/SplashWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>

namespace Views {

SplashWindow::SplashWindow(QWidget* parent)
    : QWidget(parent)
    , _appNameText(nullptr)
    , _progressBar(nullptr)
    , _loadingMessage(nullptr)
    , _promotionView(nullptr)
    , _progressTimer(nullptr)
    , _currentProgress(0)
    , _isAnimating(false)
{
    initUI();
}

SplashWindow::~SplashWindow()
{
    if (_progressTimer) {
        _progressTimer->stop();
        delete _progressTimer;
    }
}

void SplashWindow::initUI()
{
    setFixedSize(800, 500);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 30);
    mainLayout->setSpacing(20);

    _appNameText = new ElaText("CZHsoft", this);
    _appNameText->setTextPixelSize(25);
    _appNameText->setFixedHeight(40);

    QFont font = _appNameText->font();
    font.setItalic(true);
    _appNameText->setFont(font);
    mainLayout->addWidget(_appNameText, 0, Qt::AlignLeft | Qt::AlignTop);

    _promotionView = new ElaPromotionView(this);
    _promotionView->setFixedSize(600, 300);
    _promotionView->setIsAutoScroll(true);
    _promotionView->setAutoScrollInterval(1000);

    initPromotionImages();

    QWidget* promotionContainer = new QWidget(this);
    QHBoxLayout* promotionLayout = new QHBoxLayout(promotionContainer);
    promotionLayout->setContentsMargins(0, 0, 0, 0);
    promotionLayout->addStretch();
    promotionLayout->addWidget(_promotionView);
    promotionLayout->addStretch();
    mainLayout->addWidget(promotionContainer);

    _loadingMessage = new ElaText("正在初始化系统...", this);
    _loadingMessage->setTextPixelSize(14);
    _loadingMessage->setWordWrap(false);
    mainLayout->addWidget(_loadingMessage, 0, Qt::AlignCenter);

    _progressBar = new ElaProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(100);
    _progressBar->setFixedHeight(20);
    _progressBar->setValue(0);
    mainLayout->addWidget(_progressBar);

    mainLayout->addStretch();
}

///
/// \brief 初始化轮播组件
///
void SplashWindow::initPromotionImages()
{
    // QStringList imagePaths;
    // imagePaths << ":/Resources/pic/battery_cover.png"
    //            << ":/Resources/pic/daoren_cover.png"
    //            << ":/Resources/pic/tracking_cover.png";

    // for (int i = 0; i < imagePaths.size() && i < 3; i++) {
    //     ElaPromotionCard* card = new ElaPromotionCard(_promotionView);
    //     card->setCardPixmap(QPixmap(imagePaths[i]));
    //     card->setCardTitle("CZH Widget");
    //     card->setTitle("CZHsoft");
    //     card->setSubTitle("Powered by ElaWidgetTools");
    //     _promotionView->appendPromotionCard(card);
    // }

        ElaPromotionCard* card1 = new ElaPromotionCard(_promotionView);
        card1->setCardPixmap(QPixmap(":/Resources/pic/battery_cover.png"));
        // card1->setTitle("battery2025");
        card1->setCardTitle("电池充放电控制系统");
        // card1->setSubTitle("Powered by CZHsoft");
        _promotionView->appendPromotionCard(card1);

        ElaPromotionCard* card2 = new ElaPromotionCard(_promotionView);
        card2->setCardPixmap(QPixmap(":/Resources/pic/tracking_cover.png"));
        // card2->setTitle("objecttracking");
        card2->setCardTitle("物体识别追踪系统");
        // card2->setSubTitle("Powered by CZHsoft");
        _promotionView->appendPromotionCard(card2);

        ElaPromotionCard* card3 = new ElaPromotionCard(_promotionView);
        card3->setCardPixmap(QPixmap(":/Resources/pic/daoren_cover.png"));
        // card3->setTitle("daoren");
        card3->setCardTitle("数据采集监控系统");
        // card3->setSubTitle("Powered by CZHsoft");
        _promotionView->appendPromotionCard(card3);
}

void SplashWindow::startAnimation()
{
    if (_isAnimating) {
        return;
    }

    _isAnimating = true;
    _currentProgress = 0;
    _progressBar->setValue(0);
    updateLoadingMessage(0);

    _progressTimer = new QTimer(this);
    connect(_progressTimer, &QTimer::timeout, this, &SplashWindow::updateProgress);
    _progressTimer->start(80);
}

///
/// \brief SplashWindow::updateProgress
///
void SplashWindow::updateProgress()
{
    _currentProgress++;
    _progressBar->setValue(_currentProgress);
    updateLoadingMessage(_currentProgress);

    if (_currentProgress >= 100) {
        finishLoading();
    }
}

void SplashWindow::finishLoading()
{
    if (_progressTimer) {
        _progressTimer->stop();
    }
    _isAnimating = false;
    _currentProgress = 100;
    _progressBar->setValue(100);
    updateLoadingMessage(100);
    QTimer::singleShot(500, this, &SplashWindow::loadingFinished);
}

void SplashWindow::updateLoadingMessage(int progress)
{
    QString message;
    if (progress < 20) {
        message = "正在初始化系统组件...";
    } else if (progress < 40) {
        message = "正在加载图形界面...";
    } else if (progress < 60) {
        message = "正在初始化网络模块...";
    } else if (progress < 80) {
        message = "正在配置应用程序...";
    } else if (progress < 100) {
        message = "准备就绪，即将启动...";
    } else {
        message = "启动完成！";
    }
    _loadingMessage->setText(message);
}

void SplashWindow::setProgress(int progress, const QString& message)
{
    if (_progressTimer) {
        _progressTimer->stop();
    }

    _currentProgress = qBound(0, progress, 100);
    _progressBar->setValue(_currentProgress);

    if (!message.isEmpty()) {
        _loadingMessage->setText(message);
    } else {
        updateLoadingMessage(_currentProgress);
    }

    if (_currentProgress >= 100) {
        _isAnimating = false;
        QTimer::singleShot(500, this, &SplashWindow::loadingFinished);
    } else {
        _isAnimating = true;
    }
}

}