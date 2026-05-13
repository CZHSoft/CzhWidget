#ifndef WAVEFORMPLOTTER_H
#define WAVEFORMPLOTTER_H

#include <QWidget>
#include <QVector>
#include <QColor>

class WaveformPlotter : public QWidget
{
    Q_OBJECT

public:
    explicit WaveformPlotter(QWidget* parent = nullptr);

    void setData(const QVector<double>& dataX, const QVector<double>& dataY);
    void addDataPoint(double x, double y);
    void clearData();

    void setXRange(double min, double max);
    void setYRange(double min, double max);

    void setWaveformColor(const QColor& color);
    void setBackgroundColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawBackground(QPainter& p);
    void drawGrid(QPainter& p);
    void drawAxis(QPainter& p);
    void drawCurve(QPainter& p);

    QPointF dataToScreen(double x, double y) const;

private:
    QVector<double> m_timeData;
    QVector<double> m_data;

    double m_xMin = 0.0;
    double m_xMax = 100.0;
    double m_yMin = -2.0;
    double m_yMax = 2.0;

    QColor m_waveformColor = QColor("#00BFFF");
    QColor m_backgroundColor = QColor("#1E1E1E");

    int m_marginLeft = 50;
    int m_marginRight = 15;
    int m_marginTop = 10;
    int m_marginBottom = 45;
};

#endif
