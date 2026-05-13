#ifndef WAVEFORMPANEL_H
#define WAVEFORMPANEL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QVector>
#include "Widgets/WaveformPlotter.h"
#include "Services/Comm/CommManager.h"
#include "ElaText.h"
#include "ElaComboBox.h"

namespace Views {
namespace Panels {

enum class DisplayMode {
    Marquee,
    Heartbeat
};

class WaveformPanel : public QWidget
{
    Q_OBJECT

protected:
    static constexpr size_t MAX_DATA_POINTS = 500;
    static constexpr size_t CLEAR_AFTER_FLAG = 10;
    static constexpr double X_RANGE = 200.0;

    WaveformPlotter* m_plotter;
    QTimer* m_updateTimer;

    QFrame* m_statusBarLayout;
    QWidget* m_connectionIndicator;
    ElaText* m_statusLabel;
    ElaText* m_dataRateLabel;
    ElaComboBox* m_displayModeCombo;

    Services::Comm::CommManager* m_commManager;
    bool m_isConnected;
    bool m_isPlotting;
    bool m_hasReceivedData;
    bool m_isReceivingData;

    QVector<double> m_xData;
    QVector<double> m_yData;
    double m_xOffset;
    DisplayMode m_displayMode;

    QList<double> m_calcTimes;
    size_t m_dataReceivedInSecond;
    size_t m_currentDataRate;

    void initUI();
    void initConnections();
    void startPlotting();
    void stopPlotting();
    void updateConnectionIndicator();
    void updateDataRate();

    void addDataMarqueeMode(double x, double y);
    void addDataHeartbeatMode(double x, double y);

public:
    explicit WaveformPanel(QWidget* parent = nullptr);
    ~WaveformPanel() override;

    void setCommManager(Services::Comm::CommManager* manager);
    Services::Comm::CommManager* getCommManager() const;

    void clearWaveform();
    void setDisplayMode(DisplayMode mode);
    DisplayMode getDisplayMode() const;

protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    void retranslateUi();

private Q_SLOTS:
    void plotNewData();
    void onSerialConnected();
    void onSerialDisconnected();
    void onDataReceived(const QByteArray& data);
    void onDisplayModeChanged(int index);
    void onTimerTimeout();
};

}
}

#endif
