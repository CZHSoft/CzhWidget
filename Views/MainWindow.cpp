#include "Views/MainWindow.h"
#include "Views/Pages/DashboardPage.h"
#include "Views/Pages/SettingPage.h"
#include "Views/Pages/AboutPage.h"
#include "Views/Pages/LogsPage.h"
#include "Views/Pages/MonitorPage.h"
#include "Views/Pages/ProtocolPage.h"
#include "Views/Pages/ToolsPage.h"
#include "Views/Panels/AutoSendPanel.h"
#include "Views/Panels/WaveformPanel.h"
#include "Services/Comm/CommManager.h"
#include "Presenters/MonitorPresenter.h"
#include "Services/Log/LogService.h"

#include "ElaToolBar.h"
#include "ElaToolButton.h"

#include <QMouseEvent>
#include <QDebug>
#include <QDateTime>
#include <QNetworkInterface>
#include <QSysInfo>
#include <QWidget>
#include <QHBoxLayout>
#include <QMap>

namespace Views {

MainWindow::MainWindow(QWidget* parent)
    : ElaWindow(parent)
    , m_observer(nullptr)
    , _statusBar(nullptr)
    , _systemStatusText(nullptr)
    , _timeText(nullptr)
    , _timer(nullptr)
    , _menuBar(nullptr)
    , _viewMenu(nullptr)
    , _autoSendPanelAction(nullptr)
    , _autoSendDockWidget(nullptr)
    , _waveformPanelAction(nullptr)
    , _waveformDockWidget(nullptr)
{
    initWindow();
    initEdgeLayout();
    initContent();
    initStatusBar();
    initToolBar();
}

MainWindow::~MainWindow()
{
    if (_timer)
    {
        _timer->stop();
        delete _timer;

        Services::Log::LogService::instance()->info("_timer destoryed", "MainWindow");
    }
}

void MainWindow::setObserver(IMainWindowObserver* observer)
{
    m_observer = observer;
}

void MainWindow::initWindow()
{
    setFocusPolicy(Qt::StrongFocus);

    setMinimumSize(800, 400);
    resize(1024, 768);

    setUserInfoCardPixmap(QPixmap(":/Resources/pic/logo.png"));
    setUserInfoCardTitle(tr("CZHsoft"));
    setUserInfoCardSubTitle(tr("chenandczh@163.com"));
    setUserInfoCardVisible(true);

    setWindowTitle(tr("CzhWidget"));

    setIsDefaultClosed(true);
    setIsNavigationBarEnable(true);
}

void MainWindow::initEdgeLayout()
{
    _menuBar = new ElaMenuBar(this);
    _menuBar->setFixedHeight(30);

    QWidget* customWidget = new QWidget(this);
    customWidget->setFixedWidth(500);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(_menuBar);
    customLayout->addStretch();
    this->setCustomWidget(ElaAppBarType::MiddleArea, customWidget);

    _viewMenu = _menuBar->addMenu(ElaIconType::WindowRestore ,tr("View"));
    _viewMenu->setMenuItemHeight(27);
    
    _autoSendPanelAction = _viewMenu->addAction(tr("Auto Command Send Panel"));
    _autoSendPanelAction->setCheckable(true);
    _autoSendPanelAction->setChecked(false);

    _waveformPanelAction = _viewMenu->addAction(tr("Data Waveform Visualization Panel"));
    _waveformPanelAction->setCheckable(true);
    _waveformPanelAction->setChecked(false);

    Views::Panels::AutoSendPanel* autoSendPanel = new Views::Panels::AutoSendPanel(this);
    _autoSendDockWidget = new ElaDockWidget(tr("Auto Command Send"), this);
    _autoSendDockWidget->setWidget(autoSendPanel);
    this->addDockWidget(Qt::RightDockWidgetArea, _autoSendDockWidget);
    resizeDocks({_autoSendDockWidget}, {350}, Qt::Horizontal);
    _autoSendDockWidget->hide();

    Views::Panels::WaveformPanel* waveformPanel = new Views::Panels::WaveformPanel(this);
    _waveformDockWidget = new ElaDockWidget(tr("Data Waveform Visualization"), this);
    _waveformDockWidget->setWidget(waveformPanel);
    this->addDockWidget(Qt::LeftDockWidgetArea, _waveformDockWidget);
    resizeDocks({_waveformDockWidget}, {350}, Qt::Horizontal);
    _waveformDockWidget->hide();

    connect(_autoSendPanelAction, &QAction::toggled, this, [this](bool checked) {
        if (checked) {
            _autoSendDockWidget->show();
        } else {
            _autoSendDockWidget->hide();
        }
    });

    connect(_waveformPanelAction, &QAction::toggled, this, [this](bool checked) {
        if (checked) {
            _waveformDockWidget->show();
        } else {
            _waveformDockWidget->hide();
        }
    });

    connect(_autoSendDockWidget, &ElaDockWidget::visibilityChanged, this, [this](bool visible) {
        QSignalBlocker blocker(_autoSendPanelAction);
        Q_UNUSED(blocker);
        _autoSendPanelAction->setChecked(visible);
    });

    connect(_waveformDockWidget, &ElaDockWidget::visibilityChanged, this, [this](bool visible) {
        QSignalBlocker blocker(_waveformPanelAction);
        Q_UNUSED(blocker);
        _waveformPanelAction->setChecked(visible);
    });
}

void MainWindow::initContent()
{
    Pages::DashboardPage* dashboardPage = new Pages::DashboardPage(this);
    Pages::MonitorPage* monitorPage = new Pages::MonitorPage(this);
    Pages::ProtocolPage* protocolPage = new Pages::ProtocolPage(this);
    Pages::ToolsPage* toolsPage = new Pages::ToolsPage(this);
    Pages::SettingPage* settingPage = new Pages::SettingPage(this);
    Pages::AboutPage* aboutPage = new Pages::AboutPage(this);
    Pages::LogsPage* logsPage = new Pages::LogsPage(this);

    Presenters::MonitorPresenter* monitorPresenter = new Presenters::MonitorPresenter(monitorPage, this);
    monitorPresenter->init();
    monitorPage->setPresenter(monitorPresenter);
    
    // 将 Dock 中的 AutoSendPanel 传递给 MonitorPresenter
    if (_autoSendDockWidget) {
        auto autoSendPanel = qobject_cast<Views::Panels::AutoSendPanel*>(_autoSendDockWidget->widget());
        if (autoSendPanel) {
            monitorPresenter->setAutoSendPanel(autoSendPanel);
        }
    }

    connect(monitorPage, &Pages::MonitorPage::connectButtonClicked, monitorPresenter,
            &Presenters::MonitorPresenter::onConnectButtonClicked);
    connect(monitorPage, &Pages::MonitorPage::disconnectButtonClicked, monitorPresenter,
            &Presenters::MonitorPresenter::onDisconnectButtonClicked);
    connect(monitorPage, &Pages::MonitorPage::sendData, monitorPresenter,
            &Presenters::MonitorPresenter::onSendData);
    connect(monitorPage, &Pages::MonitorPage::protocolChanged, monitorPresenter,
            &Presenters::MonitorPresenter::onProtocolChanged);
    connect(monitorPage, &Pages::MonitorPage::hexDisplayModeChanged, monitorPresenter,
            &Presenters::MonitorPresenter::onHexDisplayModeChanged);

    // 将 WaveformPanel 与 CommManager 建立连接，实现数据可视化独立运行
    if (_waveformDockWidget) {
        auto waveformPanel = qobject_cast<Views::Panels::WaveformPanel*>(_waveformDockWidget->widget());
        if (waveformPanel) {
            waveformPanel->setCommManager(Services::Comm::CommManager::getInstance());
        }
    }

    connect(monitorPresenter, &Presenters::MonitorPresenter::connectionStatusChanged, monitorPage,
            &Pages::MonitorPage::updateConnectionStatus);
    connect(monitorPresenter, &Presenters::MonitorPresenter::dataReceived, monitorPage,
            &Pages::MonitorPage::appendReceivedData);
    connect(monitorPresenter, &Presenters::MonitorPresenter::errorOccurred, monitorPage,
            &Pages::MonitorPage::showError);
    connect(monitorPresenter, &Presenters::MonitorPresenter::serialPortsUpdated, monitorPage,
            &Pages::MonitorPage::updateSerialPorts);

    connect(monitorPage->getSerialPortCombo(), &ElaComboBox::currentTextChanged, this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateSerialConfig(
            monitorPage->getSerialPortCombo()->currentText(),
            monitorPage->getBaudRateCombo()->currentText().toInt(),
            monitorPage->getDataBitsCombo()->currentText().toInt(),
            monitorPage->getParityCombo()->currentText(),
            monitorPage->getStopBitsCombo()->currentText().toInt()
        );
    });

    connect(monitorPage->getBaudRateCombo(), &ElaComboBox::currentTextChanged, this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateSerialConfig(
            monitorPage->getSerialPortCombo()->currentText(),
            monitorPage->getBaudRateCombo()->currentText().toInt(),
            monitorPage->getDataBitsCombo()->currentText().toInt(),
            monitorPage->getParityCombo()->currentText(),
            monitorPage->getStopBitsCombo()->currentText().toInt()
        );
    });

    connect(monitorPage->getDataBitsCombo(), &ElaComboBox::currentTextChanged, this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateSerialConfig(
            monitorPage->getSerialPortCombo()->currentText(),
            monitorPage->getBaudRateCombo()->currentText().toInt(),
            monitorPage->getDataBitsCombo()->currentText().toInt(),
            monitorPage->getParityCombo()->currentText(),
            monitorPage->getStopBitsCombo()->currentText().toInt()
        );
    });

    connect(monitorPage->getParityCombo(), &ElaComboBox::currentTextChanged, this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateSerialConfig(
            monitorPage->getSerialPortCombo()->currentText(),
            monitorPage->getBaudRateCombo()->currentText().toInt(),
            monitorPage->getDataBitsCombo()->currentText().toInt(),
            monitorPage->getParityCombo()->currentText(),
            monitorPage->getStopBitsCombo()->currentText().toInt()
        );
    });

    connect(monitorPage->getStopBitsCombo(), &ElaComboBox::currentTextChanged, this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateSerialConfig(
            monitorPage->getSerialPortCombo()->currentText(),
            monitorPage->getBaudRateCombo()->currentText().toInt(),
            monitorPage->getDataBitsCombo()->currentText().toInt(),
            monitorPage->getParityCombo()->currentText(),
            monitorPage->getStopBitsCombo()->currentText().toInt()
        );
    });

    connect(monitorPage->getTcpServerIpEdit(), &ElaLineEdit::textChanged, this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateTcpClientConfig(
            monitorPage->getTcpServerIpEdit()->text(),
            monitorPage->getTcpServerPortSpin()->value(),
            monitorPage->getTcpAutoReconnectBox()->isChecked()
        );
    });

    connect(monitorPage->getTcpServerPortSpin(), QOverload<int>::of(&ElaSpinBox::valueChanged), this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateTcpClientConfig(
            monitorPage->getTcpServerIpEdit()->text(),
            monitorPage->getTcpServerPortSpin()->value(),
            monitorPage->getTcpAutoReconnectBox()->isChecked()
        );
    });

    connect(monitorPage->getTcpListenPortSpin(), QOverload<int>::of(&ElaSpinBox::valueChanged), this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateTcpServerConfig(monitorPage->getTcpListenPortSpin()->value());
    });

    connect(monitorPage->getUdpLocalPortSpin(), QOverload<int>::of(&ElaSpinBox::valueChanged), this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateUdpConfig(
            monitorPage->getUdpLocalPortSpin()->value(),
            monitorPage->getUdpTargetIpEdit()->text(),
            monitorPage->getUdpBroadcastBox()->isChecked()
        );
    });

    connect(monitorPage->getUdpTargetIpEdit(), &ElaLineEdit::textChanged, this, [monitorPresenter, monitorPage]() {
        monitorPresenter->updateUdpConfig(
            monitorPage->getUdpLocalPortSpin()->value(),
            monitorPage->getUdpTargetIpEdit()->text(),
            monitorPage->getUdpBroadcastBox()->isChecked()
        );
    });

    monitorPresenter->updateSerialConfig(
        monitorPage->getSerialPortCombo()->currentText(),
        monitorPage->getBaudRateCombo()->currentText().toInt(),
        monitorPage->getDataBitsCombo()->currentText().toInt(),
        monitorPage->getParityCombo()->currentText(),
        monitorPage->getStopBitsCombo()->currentText().toInt()
    );

    monitorPresenter->updateTcpClientConfig(
        monitorPage->getTcpServerIpEdit()->text(),
        monitorPage->getTcpServerPortSpin()->value(),
        monitorPage->getTcpAutoReconnectBox()->isChecked()
    );

    monitorPresenter->updateTcpServerConfig(monitorPage->getTcpListenPortSpin()->value());

    monitorPresenter->updateUdpConfig(
        monitorPage->getUdpLocalPortSpin()->value(),
        monitorPage->getUdpTargetIpEdit()->text(),
        monitorPage->getUdpBroadcastBox()->isChecked()
    );

    monitorPresenter->onRefreshSerialPorts();

    addPageNode(tr("Dashboard"), dashboardPage, ElaIconType::House);
    addPageNode(tr("Monitor"), monitorPage, ElaIconType::MonitorWaveform);
    addPageNode(tr("Protocol"), protocolPage, ElaIconType::Message);
    addPageNode(tr("Tools"), toolsPage, ElaIconType::Wrench);
    addPageNode(tr("Setting"), settingPage, ElaIconType::GearComplex);
    addPageNode(tr("Logs"), logsPage, ElaIconType::FileLines);
    addPageNode(tr("About"), aboutPage, ElaIconType::User);

    QString dashKey = dashboardPage->property("ElaPageKey").toString();
    QString monitorKey = monitorPage->property("ElaPageKey").toString();
    QString protocolKey = protocolPage->property("ElaPageKey").toString();
    QString toolsKey = toolsPage->property("ElaPageKey").toString();
    QString settingKey = settingPage->property("ElaPageKey").toString();
    QString logsKey = logsPage->property("ElaPageKey").toString();
    QString aboutKey = aboutPage->property("ElaPageKey").toString();

    if (!dashKey.isEmpty()) _pageNameToKeyMap["Dashboard"] = dashKey;
    if (!monitorKey.isEmpty()) _pageNameToKeyMap["Monitor"] = monitorKey;
    if (!protocolKey.isEmpty()) _pageNameToKeyMap["Protocol"] = protocolKey;
    if (!toolsKey.isEmpty()) _pageNameToKeyMap["Tools"] = toolsKey;
    if (!settingKey.isEmpty()) _pageNameToKeyMap["Setting"] = settingKey;
    if (!logsKey.isEmpty()) _pageNameToKeyMap["Logs"] = logsKey;
    if (!aboutKey.isEmpty()) _pageNameToKeyMap["About"] = aboutKey;

    connect(dashboardPage, &Pages::DashboardPage::navigateToPage, this, [this](const QString& pageName) {
        QString pageKey = _pageNameToKeyMap.value(pageName);
        if (!pageKey.isEmpty()) {
            this->navigation(pageKey);
        }
    });
}

void MainWindow::initStatusBar()
{
    _statusBar = new ElaStatusBar(this);

    _systemStatusText = new ElaText(tr("System Ready"), this);
    _systemStatusText->setTextPixelSize(13);
    _systemStatusText->setWordWrap(false);

    _timeText = new ElaText("", this);
    _timeText->setTextPixelSize(13);

    _statusBar->addWidget(_systemStatusText);
    _statusBar->addPermanentWidget(_timeText, 0);

    setStatusBar(_statusBar);

    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &MainWindow::updateTime);
    connect(_timer, &QTimer::timeout, this, &MainWindow::updateSystemStatus);
    _timer->start(1000);
}

void MainWindow::initToolBar(){
    ElaToolBar* toolBar = new ElaToolBar("toolBar", this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setIconSize(QSize(25, 25));

    ElaToolButton* toolButton1 = new ElaToolButton(this);
    toolButton1->setElaIcon(ElaIconType::Blinds);
    toolButton1->setToolTip(tr("Open Auto Command Send Panel"));
    toolButton1->setCheckable(true);
    connect(toolButton1, &ElaToolButton::clicked, this, [this, toolButton1]() {
        bool checked = toolButton1->isChecked();
        _autoSendDockWidget->setVisible(checked);
        QSignalBlocker blocker(_autoSendPanelAction);
        Q_UNUSED(blocker);
        _autoSendPanelAction->setChecked(checked);
    });
    connect(_autoSendDockWidget, &ElaDockWidget::visibilityChanged, this, [this, toolButton1](bool visible) {
        QSignalBlocker blocker(toolButton1);
        Q_UNUSED(blocker);
        toolButton1->setChecked(visible);
    });
    toolBar->addWidget(toolButton1);

    ElaToolButton* toolButton2 = new ElaToolButton(this);
    toolButton2->setElaIcon(ElaIconType::ChartMixed);
    toolButton2->setToolTip(tr("Open Data Waveform Visualization Panel"));
    toolButton2->setCheckable(true);
    connect(toolButton2, &ElaToolButton::clicked, this, [this, toolButton2]() {
        bool checked = toolButton2->isChecked();
        _waveformDockWidget->setVisible(checked);
        QSignalBlocker blocker(_waveformPanelAction);
        Q_UNUSED(blocker);
        _waveformPanelAction->setChecked(checked);
    });
    connect(_waveformDockWidget, &ElaDockWidget::visibilityChanged, this, [this, toolButton2](bool visible) {
        QSignalBlocker blocker(toolButton2);
        Q_UNUSED(blocker);
        toolButton2->setChecked(visible);
    });
    toolBar->addWidget(toolButton2);

    toolBar->addSeparator();

    this->addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = QString("%1 ").arg(currentTime.toString("HH:mm:ss"));

    _timeText->setText(timeString);
}

void MainWindow::updateSystemStatus()
{
    QString statusInfo = tr("System Normal");

    bool networkConnected = false;
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces)
    {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            iface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            networkConnected = true;
            break;
        }
    }

    QString networkInfo = "";

    if (networkConnected)
    {
        networkInfo = tr("Network: Connected");
    }
    else
    {
        networkInfo = tr("Network: Disconnected");
    }

    QString archInfo = QString(tr("System: %1")).arg(QSysInfo::productType());

    statusInfo = QString("%1 | %2 | %3 ").arg(statusInfo, networkInfo, archInfo);

    _systemStatusText->setText(statusInfo);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::BackButton)
    {
        setCurrentStackIndex(0);
    }
    else if (event->button() == Qt::ForwardButton)
    {
        setCurrentStackIndex(1);
    }
    ElaWindow::mouseReleaseEvent(event);
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    ElaWindow::changeEvent(event);
}

void MainWindow::retranslateUi()
{
    setUserInfoCardTitle(tr("CZHsoft"));
    setUserInfoCardSubTitle(tr("chenandczh@163.com"));
    setWindowTitle(tr("CzhWidget"));
    
    if (_viewMenu)
    {
        _viewMenu->setTitle(tr("View(&V)"));
    }
    if (_autoSendPanelAction)
    {
        _autoSendPanelAction->setText(tr("Auto Command Send Panel"));
    }
    if (_waveformPanelAction)
    {
        _waveformPanelAction->setText(tr("Data Waveform Visualization Panel"));
    }
    if (_autoSendDockWidget)
    {
        _autoSendDockWidget->setWindowTitle(tr("Auto Command Send"));
    }
    if (_waveformDockWidget)
    {
        _waveformDockWidget->setWindowTitle(tr("Data Waveform Visualization"));
    }
    
    QString dashKey = _pageNameToKeyMap.value("Dashboard");
    QString monitorKey = _pageNameToKeyMap.value("Monitor");
    QString protocolKey = _pageNameToKeyMap.value("Protocol");
    QString toolsKey = _pageNameToKeyMap.value("Tools");
    QString settingKey = _pageNameToKeyMap.value("Setting");
    QString logsKey = _pageNameToKeyMap.value("Logs");
    QString aboutKey = _pageNameToKeyMap.value("About");
    
    if (!dashKey.isEmpty()) setNavigationNodeTitle(dashKey, tr("Dashboard"));
    if (!monitorKey.isEmpty()) setNavigationNodeTitle(monitorKey, tr("Monitor"));
    if (!protocolKey.isEmpty()) setNavigationNodeTitle(protocolKey, tr("Protocol"));
    if (!toolsKey.isEmpty()) setNavigationNodeTitle(toolsKey, tr("Tools"));
    if (!settingKey.isEmpty()) setNavigationNodeTitle(settingKey, tr("Setting"));
    if (!logsKey.isEmpty()) setNavigationNodeTitle(logsKey, tr("Logs"));
    if (!aboutKey.isEmpty()) setNavigationNodeTitle(aboutKey, tr("About"));
    
    updateSystemStatus();
}

}