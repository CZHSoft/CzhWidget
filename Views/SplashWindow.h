#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QWidget>
#include <QTimer>
#include "ElaText.h"
#include "ElaProgressBar.h"
#include "ElaPromotionView.h"
#include "ElaPromotionCard.h"

namespace Views {

class SplashWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SplashWindow(QWidget* parent = nullptr);
    ~SplashWindow() override;

    ///
    /// \brief 开启定时器，模拟更新进度条
    ///
    void startAnimation();

    ///
    /// \brief 设置加载进度（0-100）
    /// \param progress 进度值
    /// \param message 进度消息
    ///
    void setProgress(int progress, const QString& message = QString());
    
    ///
    /// \brief 强制完成加载
    ///
    void finishLoading();

signals:
    void loadingFinished();

private slots:
    void updateProgress();
    void updateLoadingMessage(int progress);

private:
    ///
    /// \brief 初始化SplashUI
    ///
    void initUI();
    ///
    /// \brief 初始化轮播图
    ///
    void initPromotionImages();

    ///
    /// \brief 左上角 标题名
    ///
    ElaText* _appNameText;
    ///
    /// \brief 进度条
    ///
    ElaProgressBar* _progressBar;
    ///
    /// \brief 进度消息文本
    ///
    ElaText* _loadingMessage;
    ///
    /// \brief 轮播图控件
    ///
    ElaPromotionView* _promotionView;

    ///
    /// \brief 控制进度定时器
    ///
    QTimer* _progressTimer;
    ///
    /// \brief 进度条值
    ///
    int _currentProgress;
    ///
    /// \brief Splash Flag,控制只允许运行一次
    ///
    bool _isAnimating;
};

}

#endif // SPLASHWINDOW_H