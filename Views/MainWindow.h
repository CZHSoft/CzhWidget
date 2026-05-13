#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ElaWindow.h"
#include "ElaStatusBar.h"
#include "ElaText.h"
#include "ElaMenuBar.h"
#include "ElaMenu.h"
#include "ElaDockWidget.h"

#include <QMouseEvent>
#include <QTimer>
#include <QAction>

namespace Views {
namespace Pages {
class DashboardPage;
class SettingPage;
class AboutPage;
class LogsPage;
class MonitorPage;
class ProtocolPage;
class ToolsPage;
}
namespace Panels {
class WaveformPanel;
}

class IMainWindowObserver
{
public:
    virtual ~IMainWindowObserver() = default;
};

class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void setObserver(IMainWindowObserver* observer);
    IMainWindowObserver* getObserver() const { return m_observer; }

    void initWindow();
    void initEdgeLayout();
    void initContent();
    void initStatusBar();
    void initToolBar();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void changeEvent(QEvent* event) override;

private slots:
    void updateTime();
    void updateSystemStatus();

public slots:
    void retranslateUi();

private:
    IMainWindowObserver* m_observer;
    ElaStatusBar* _statusBar;
    ElaText* _systemStatusText;
    ElaText* _timeText;
    QTimer* _timer;
    
    ElaMenuBar* _menuBar;
    ElaMenu* _viewMenu;
    QAction* _autoSendPanelAction;
    ElaDockWidget* _autoSendDockWidget;
    
    QAction* _waveformPanelAction;
    ElaDockWidget* _waveformDockWidget;
    
    QMap<QString, QString> _pageNameToKeyMap;
};

}

#endif // MAINWINDOW_H