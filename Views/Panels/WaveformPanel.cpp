#include "WaveformPanel.h"
#include <QShowEvent>
#include <QHideEvent>
#include <QDebug>

namespace Views {
namespace Panels {

WaveformPanel::WaveformPanel(QWidget* parent)
    : QWidget(parent)
    , m_plotter(nullptr)
    , m_updateTimer(nullptr)
    , m_statusBarLayout(nullptr)
    , m_connectionIndicator(nullptr)
    , m_statusLabel(nullptr)
    , m_dataRateLabel(nullptr)
    , m_displayModeCombo(nullptr)
    , m_commManager(nullptr)
    , m_isConnected(false)
    , m_isPlotting(false)
    , m_hasReceivedData(false)
    , m_isReceivingData(false)
    , m_xOffset(0.0)
    , m_displayMode(DisplayMode::Marquee)
    , m_dataReceivedInSecond(0)
    , m_currentDataRate(0)
{
    initUI();
    initConnections();
}

WaveformPanel::~WaveformPanel()
{
    stopPlotting();
    delete m_updateTimer;
}

void WaveformPanel::initUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(2);

    m_statusBarLayout = new QFrame(this);
    m_statusBarLayout->setFrameShape(QFrame::Box);
    QHBoxLayout* statusLayout = new QHBoxLayout(m_statusBarLayout);
    statusLayout->setContentsMargins(8, 4, 8, 4);
    statusLayout->setSpacing(8);

    m_connectionIndicator = new QWidget(this);
    m_connectionIndicator->setFixedSize(12, 12);
    m_connectionIndicator->setStyleSheet("background-color: red; border-radius: 6px;");
    statusLayout->addWidget(m_connectionIndicator);

    m_statusLabel = new ElaText(tr("Serial Port Disconnected"), this);
    m_statusLabel->setTextPixelSize(13);
    statusLayout->addWidget(m_statusLabel);

    statusLayout->addStretch();

    m_dataRateLabel = new ElaText(tr("Data Rate: 0 pts/s"), this);
    m_dataRateLabel->setTextPixelSize(13);
    statusLayout->addWidget(m_dataRateLabel);

    mainLayout->addWidget(m_statusBarLayout);

    m_plotter = new WaveformPlotter(this);
    mainLayout->addWidget(m_plotter, 1);

    m_plotter->setYRange(-2.0, 2.0);
    m_plotter->setXRange(0.0, X_RANGE);

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(1000);

    updateConnectionIndicator();
}

void WaveformPanel::initConnections()
{
    connect(m_updateTimer, &QTimer::timeout, this, &WaveformPanel::onTimerTimeout);
}

void WaveformPanel::startPlotting()
{
    if (m_isPlotting)
        return;

    m_isPlotting = true;
    m_xOffset = 0.0;
    m_xData.clear();
    m_yData.clear();
    m_hasReceivedData = false;
    m_isReceivingData = false;
    m_dataReceivedInSecond = 0;
    m_currentDataRate = 0;
    m_plotter->clearData();

    m_updateTimer->start();
}

void WaveformPanel::stopPlotting()
{
    if (!m_isPlotting)
        return;

    m_isPlotting = false;
    m_updateTimer->stop();
}

void WaveformPanel::setCommManager(Services::Comm::CommManager* manager)
{
    if (m_commManager == manager)
        return;

    if (m_commManager) {
        disconnect(m_commManager, &Services::Comm::CommManager::connected,
                   this, &WaveformPanel::onSerialConnected);
        disconnect(m_commManager, &Services::Comm::CommManager::disconnected,
                   this, &WaveformPanel::onSerialDisconnected);
        disconnect(m_commManager, &Services::Comm::CommManager::dataReceived,
                   this, &WaveformPanel::onDataReceived);
    }

    m_commManager = manager;

    if (m_commManager) {
        connect(m_commManager, &Services::Comm::CommManager::connected,
                this, &WaveformPanel::onSerialConnected);
        connect(m_commManager, &Services::Comm::CommManager::disconnected,
                this, &WaveformPanel::onSerialDisconnected);
        connect(m_commManager, &Services::Comm::CommManager::dataReceived,
                this, &WaveformPanel::onDataReceived);

        m_isConnected = m_commManager->isConnected();
        updateConnectionIndicator();
    }
}

Services::Comm::CommManager* WaveformPanel::getCommManager() const
{
    return m_commManager;
}

void WaveformPanel::clearWaveform()
{
    m_xData.clear();
    m_yData.clear();
    m_xOffset = 0.0;
    m_hasReceivedData = false;
    m_isReceivingData = false;
    m_plotter->clearData();
}

void WaveformPanel::setDisplayMode(DisplayMode mode)
{
    m_displayMode = mode;
}

DisplayMode WaveformPanel::getDisplayMode() const
{
    return m_displayMode;
}

void WaveformPanel::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (m_isConnected) {
        startPlotting();
    }
}

void WaveformPanel::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    stopPlotting();
}

void WaveformPanel::onSerialConnected()
{
    m_isConnected = true;
    updateConnectionIndicator();

    if (isVisible()) {
        startPlotting();
    }
}

void WaveformPanel::onSerialDisconnected()
{
    m_isConnected = false;
    updateConnectionIndicator();
    stopPlotting();
}

void WaveformPanel::onDataReceived(const QByteArray& data)
{
    if (!m_isPlotting || data.isEmpty())
        return;

    m_hasReceivedData = true;
    m_isReceivingData = true;
    m_dataReceivedInSecond += data.size();

    for (int i = 0; i < data.size(); ++i) {
        m_xOffset += 1.0;
        double value = static_cast<unsigned char>(data[i]) / 255.0 * 4.0 - 2.0;

        if (m_xOffset > X_RANGE) {
            m_xOffset = 1.0;
            m_xData.clear();
            m_yData.clear();
        }

        m_xData.append(m_xOffset);
        m_yData.append(value);

        while (m_xData.size() > MAX_DATA_POINTS) {
            m_xData.removeFirst();
            m_yData.removeFirst();
        }
    }

    m_plotter->setData(m_xData, m_yData);
}

void WaveformPanel::onTimerTimeout()
{
    m_currentDataRate = m_dataReceivedInSecond;
    m_dataRateLabel->setText(tr("Data Rate: %1 pts/s").arg(m_currentDataRate));

    m_dataReceivedInSecond = 0;

    if (m_isPlotting && m_hasReceivedData && !m_isReceivingData) {
        m_xOffset += 1.0;

        if (m_xOffset > X_RANGE) {
            m_xOffset = 1.0;
            m_xData.clear();
            m_yData.clear();
        }

        m_xData.append(m_xOffset);
        m_yData.append(0.0);

        while (m_xData.size() > MAX_DATA_POINTS) {
            m_xData.removeFirst();
            m_yData.removeFirst();
        }

        m_plotter->setData(m_xData, m_yData);
    }

    m_isReceivingData = false;
}

void WaveformPanel::addDataMarqueeMode(double x, double y)
{
    m_xData.append(x);
    m_yData.append(y);

    while (m_xData.size() > MAX_DATA_POINTS) {
        m_xData.removeFirst();
        m_yData.removeFirst();
    }
}

void WaveformPanel::addDataHeartbeatMode(double x, double y)
{
    m_xData.append(x);
    m_yData.append(y);

    while (m_xData.size() > MAX_DATA_POINTS) {
        m_xData.removeFirst();
        m_yData.removeFirst();
    }
}

void WaveformPanel::onDisplayModeChanged(int index)
{
    m_displayMode = static_cast<DisplayMode>(index);
}

void WaveformPanel::updateConnectionIndicator()
{
    QString status;
    if (m_isConnected) {
        status = tr("Serial Port Connected");
        m_connectionIndicator->setStyleSheet("background-color: #00C853; border-radius: 6px;");
    } else {
        status = tr("Serial Port Disconnected");
        m_connectionIndicator->setStyleSheet("background-color: #FF5252; border-radius: 6px;");
    }

    m_statusLabel->setText(status);
}

void WaveformPanel::updateDataRate()
{
}

void WaveformPanel::plotNewData()
{
}

void WaveformPanel::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QWidget::changeEvent(event);
}

void WaveformPanel::retranslateUi()
{
    updateConnectionIndicator();
    m_dataRateLabel->setText(tr("Data Rate: %1 pts/s").arg(m_currentDataRate));
}

}
}
