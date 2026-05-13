#include <QApplication>
#include <QLocale>
#include <QDebug>
#include <QMessageLogContext>
#include <QDateTime>
#include <QTimer>

#include "ElaApplication.h"
#include "Views/MainWindow.h"
#include "Views/SplashWindow.h"
#include "Presenters/MainPresenter.h"
#include "Models/AppModel.h"
#include "Services/Log/LogService.h"
#include "Services/I18n/LanguageManager.h"

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_UNUSED(context);

    if (msg.contains("QFont::setPointSizeF") && msg.contains("Point size <= 0")) {
        return;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString logMessage = QString("[%1] %2").arg(timestamp).arg(msg);

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", logMessage.toLocal8Bit().data());
        fflush(stderr);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info:  %s\n", logMessage.toLocal8Bit().data());
        fflush(stderr);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warn:  %s\n", logMessage.toLocal8Bit().data());
        fflush(stderr);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Error: %s\n", logMessage.toLocal8Bit().data());
        fflush(stderr);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", logMessage.toLocal8Bit().data());
        fflush(stderr);
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);

    QApplication a(argc, argv);
    
    a.setApplicationName("CzhWidget");
    a.setApplicationVersion("1.0.0");
    a.setOrganizationName("CZHsoft");
    a.setWindowIcon(QIcon(":/Resources/pic/logo.ico"));

    Services::Log::LogService::instance()->info("Application starting...", "Main");

    eApp->init();
    Services::Log::LogService::instance()->info("ElaApplication initialized", "Main");

    Services::I18n::LanguageManager::instance()->init();
    Services::Log::LogService::instance()->info("LanguageManager initialized", "Main");

    Views::SplashWindow splash;
    splash.show();
    Services::Log::LogService::instance()->info("Splash window shown, starting initialization", "Main");

    // 先初始化数据，不要阻塞
    Models::AppModel model;

    // 启动动画，让Splash先播放
    splash.startAnimation();

    // 在Splash动画播放完成后，再创建主窗口
    QObject::connect(&splash, &Views::SplashWindow::loadingFinished, [&]() {
        splash.close();
        
        Services::Log::LogService::instance()->info("Creating main window", "Main");
        Views::MainWindow* mainWindow = new Views::MainWindow();
        
        Presenters::MainPresenter* presenter = new Presenters::MainPresenter();
        presenter->setView(mainWindow);
        presenter->initialize();
        
        mainWindow->show();
        Services::Log::LogService::instance()->info("Main window shown", "Main");
    });

    return QCoreApplication::exec();
}