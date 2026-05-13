#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

#include <QWidget>
#include <QColor>
#include "ElaDef.h"

namespace Widgets {

class ToggleSwitch : public QWidget
{
    Q_OBJECT

public:
    explicit ToggleSwitch(QWidget* parent = nullptr);
    ~ToggleSwitch() override;

    bool isToggled() const;
    bool getIsToggled() const;
    void setIsToggled(bool toggled);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void toggled(bool checked);

protected:
    bool event(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onThemeModeChanged(ElaThemeType::ThemeMode mode) {
        _themeMode = mode;
        update();
    }

private:
    void startPosAnimation(qreal startX, qreal endX, bool isToggle);
    void startRadiusAnimation(qreal startRadius, qreal endRadius);
    void adjustCircleCenterX();

    bool _isToggled;
    int _margin;
    qreal _circleCenterX;
    qreal _circleRadius;
    bool _isLeftButtonPress;
    bool _isMousePressMove;
    int _lastMouseX;
    qreal _targetCircleCenterX;
    qreal _targetCircleRadius;
    ElaThemeType::ThemeMode _themeMode;
};

} // namespace Widgets

#endif // TOGGLESWITCH_H
