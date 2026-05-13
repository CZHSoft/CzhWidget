#include "WaveformPlotter.h"
#include <QPainter>
#include <QPalette>
#include <QPainterPath>

WaveformPlotter::WaveformPlotter(QWidget* parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, m_backgroundColor);
    setPalette(pal);
}

void WaveformPlotter::setData(const QVector<double>& dataX, const QVector<double>& dataY)
{
    if (dataX.size() != dataY.size())
        return;

    m_timeData = dataX;
    m_data = dataY;

    if (!m_timeData.isEmpty()) {
        double lastX = m_timeData.last();
        if (lastX > m_xMax) {
            m_timeData.clear();
            m_data.clear();
        }
    }

    update();
}

void WaveformPlotter::addDataPoint(double x, double y)
{
    m_timeData.append(x);
    m_data.append(y);

    if (x > m_xMax) {
        m_timeData.clear();
        m_data.clear();
    }

    update();
}

void WaveformPlotter::clearData()
{
    m_timeData.clear();
    m_data.clear();
    update();
}

void WaveformPlotter::setXRange(double min, double max)
{
    m_xMin = min;
    m_xMax = max;
    update();
}

void WaveformPlotter::setYRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;
    update();
}

void WaveformPlotter::setWaveformColor(const QColor& color)
{
    m_waveformColor = color;
    update();
}

void WaveformPlotter::setBackgroundColor(const QColor& color)
{
    m_backgroundColor = color;
    QPalette pal = palette();
    pal.setColor(QPalette::Window, color);
    setPalette(pal);
    update();
}

void WaveformPlotter::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    drawBackground(p);
    drawGrid(p);
    drawAxis(p);
    drawCurve(p);
}

void WaveformPlotter::drawBackground(QPainter& p)
{
    p.fillRect(rect(), m_backgroundColor);
}

void WaveformPlotter::drawGrid(QPainter& p)
{
    int w = width() - m_marginLeft - m_marginRight;
    int h = height() - m_marginTop - m_marginBottom;

    p.setPen(QPen(QColor(80, 80, 80), 1, Qt::DashLine));

    for (int i = 0; i <= 10; ++i) {
        int x = m_marginLeft + (w * i / 10);
        p.drawLine(x, m_marginTop, x, height() - m_marginBottom);
    }

    for (int i = 0; i <= 8; ++i) {
        int y = m_marginTop + (h * i / 8);
        p.drawLine(m_marginLeft, y, width() - m_marginRight, y);
    }
}

void WaveformPlotter::drawAxis(QPainter& p)
{
    int w = width() - m_marginLeft - m_marginRight;
    int h = height() - m_marginTop - m_marginBottom;

    QColor textColor = m_backgroundColor.lightness() > 128 ? Qt::black : Qt::white;
    p.setPen(textColor);

    p.drawLine(m_marginLeft, height() - m_marginBottom, width() - m_marginRight, height() - m_marginBottom);
    p.drawLine(m_marginLeft, m_marginTop, m_marginLeft, height() - m_marginBottom);

    QFont font = p.font();
    font.setPixelSize(10);
    p.setFont(font);

    for (int i = 0; i <= 5; ++i) {
        double xVal = m_xMin + (m_xMax - m_xMin) * i / 5;
        double yVal = m_yMin + (m_yMax - m_yMin) * i / 5;

        int x = m_marginLeft + (w * i / 5);
        int y = height() - m_marginBottom - (h * i / 5);

        p.drawText(x - 30, height() - m_marginBottom + 20, 60, 15, Qt::AlignHCenter, QString::number(xVal, 'f', 0));
        p.drawText(0, y - 8, m_marginLeft - 8, 16, Qt::AlignRight, QString::number(yVal, 'f', 1));
    }

    p.drawText(width() / 2 - 40, height() - 20, 80, 15, Qt::AlignHCenter, "Time");

    QTransform t = p.transform();
    p.translate(15, height() / 2);
    p.rotate(-90);
    p.drawText(-40, 0, 80, 15, Qt::AlignHCenter, "Value");
    p.setTransform(t);
}

void WaveformPlotter::drawCurve(QPainter& p)
{
    if (m_data.isEmpty())
        return;

    int w = width() - m_marginLeft - m_marginRight;
    int h = height() - m_marginTop - m_marginBottom;

    QPainterPath path;
    bool started = false;

    for (int i = 0; i < m_data.size(); ++i) {
        double x = m_timeData.isEmpty() ? i : m_timeData[i];
        double y = m_data[i];

        double normX = (x - m_xMin) / (m_xMax - m_xMin);
        double normY = (y - m_yMin) / (m_yMax - m_yMin);

        if (normX < 0 || normX > 1)
            continue;

        int screenX = m_marginLeft + normX * w;
        int screenY = height() - m_marginBottom - normY * h;

        if (!started) {
            path.moveTo(screenX, screenY);
            started = true;
        } else {
            path.lineTo(screenX, screenY);
        }
    }

    p.setPen(QPen(m_waveformColor, 2));
    p.drawPath(path);
}

QPointF WaveformPlotter::dataToScreen(double x, double y) const
{
    int w = width() - m_marginLeft - m_marginRight;
    int h = height() - m_marginTop - m_marginBottom;

    double normX = (x - m_xMin) / (m_xMax - m_xMin);
    double normY = (y - m_yMin) / (m_yMax - m_yMin);

    return QPointF(m_marginLeft + normX * w, height() - m_marginBottom - normY * h);
}
