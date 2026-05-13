#include "Widgets/ToggleSwitch.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QMetaMethod>
#include <QTimer>

#include "ElaTheme.h"
#include "ElaDef.h"

namespace Widgets {

ToggleSwitch::ToggleSwitch(QWidget* parent)
    : QWidget(parent)
    , _isToggled(false)
    , _margin(1)
    , _circleCenterX(-1)
    , _circleRadius(0)
    , _isLeftButtonPress(false)
    , _isMousePressMove(false)
    , _lastMouseX(0)
    , _themeMode(eTheme->getThemeMode())
{

    setObjectName("ToggleSwitch");
    setMouseTracking(true);
    setFixedSize(44, 22);
    
    connect(eTheme,
            SIGNAL(themeModeChanged(ElaThemeType::ThemeMode)),
            this,
            SLOT(onThemeModeChanged(ElaThemeType::ThemeMode)));

}

ToggleSwitch::~ToggleSwitch()
{
}

bool ToggleSwitch::isToggled() const
{
    return _isToggled;
}

bool ToggleSwitch::getIsToggled() const
{
    return _isToggled;
}

void ToggleSwitch::setIsToggled(bool toggled)
{
    if (_isToggled == toggled)
    {
        return;
    }
    if (_isToggled)
    {
        startPosAnimation(width() - height() / 2 - _margin * 2, height() / 2, toggled);
    }
    else
    {
        startPosAnimation(height() / 2, width() - height() / 2 - _margin * 2, toggled);
    }
}

bool ToggleSwitch::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Enter:
    {
        if (isEnabled())
        {
            startRadiusAnimation(_circleRadius, height() * 0.35);
        }
        break;
    }
    case QEvent::Leave:
    {
        if (isEnabled())
        {
            startRadiusAnimation(_circleRadius, height() * 0.3);
        }
        break;
    }
    case QEvent::MouseMove:
    {
        update();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ToggleSwitch::mousePressEvent(QMouseEvent* event)
{
    adjustCircleCenterX();
    _isLeftButtonPress = true;
    _lastMouseX = event->pos().x();
    startRadiusAnimation(_circleRadius, height() * 0.25);
    QWidget::mousePressEvent(event);
}

void ToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    _isLeftButtonPress = false;
    QWidget::mouseReleaseEvent(event);
    if (_isMousePressMove)
    {
        _isMousePressMove = false;
        if (_circleCenterX > width() / 2)
        {
            startPosAnimation(_circleCenterX, width() - height() / 2 - _margin * 2, true);
        }
        else
        {
            startPosAnimation(_circleCenterX, height() / 2, false);
        }
    }
    else
    {
        if (_isToggled)
        {
            startPosAnimation(_circleCenterX, height() / 2, false);
        }
        else
        {
            startPosAnimation(_circleCenterX, width() - height() / 2 - _margin * 2, true);
        }
    }
    startRadiusAnimation(height() * 0.25, height() * 0.35);
}

void ToggleSwitch::mouseMoveEvent(QMouseEvent* event)
{
    if (_isLeftButtonPress)
    {
        _isMousePressMove = true;
        int moveX = event->pos().x() - _lastMouseX;
        _lastMouseX = event->pos().x();
        _circleCenterX += moveX;
        adjustCircleCenterX();
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void ToggleSwitch::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QColor bgColor;
    QColor borderCol;
    QColor circleColor;

    if (!isEnabled()) {
        bgColor = ElaThemeColor(_themeMode, BasicDisable);
        borderCol = ElaThemeColor(_themeMode, BasicBorderDeep);
        circleColor = ElaThemeColor(_themeMode, BasicTextDisable);
    } else if (_isToggled) {
        bgColor = ElaThemeColor(_themeMode, PrimaryNormal);
        borderCol = ElaThemeColor(_themeMode, BasicBorder);
        circleColor = ElaThemeColor(_themeMode, BasicTextInvert);
    } else {
        bgColor = underMouse() ? ElaThemeColor(_themeMode, BasicHover) : ElaThemeColor(_themeMode, BasicBase);
        borderCol = ElaThemeColor(_themeMode, BasicBorderDeep);
        circleColor = ElaThemeColor(_themeMode, ToggleSwitchNoToggledCenter);
    }

    painter.setPen(QPen(borderCol, 1.5));
    painter.setBrush(bgColor);
    QPainterPath path;
    path.moveTo(width() - height() - _margin, height() - _margin);
    path.arcTo(QRectF(QPointF(width() - height() - _margin, _margin), QSize(height() - _margin * 2, height() - _margin * 2)), -90, 180);
    path.lineTo(height() / 2 + _margin, _margin);
    path.arcTo(QRectF(QPointF(_margin, _margin), QSize(height() - _margin * 2, height() - _margin * 2)), 90, 180);
    path.lineTo(width() - height() - _margin, height() - _margin);
    path.closeSubpath();
    painter.drawPath(path);

    painter.setPen(Qt::NoPen);
    painter.setBrush(circleColor);
    if (_circleRadius == 0)
    {
        _circleRadius = isEnabled() ? (underMouse() ? height() * 0.35 : height() * 0.3) : height() * 0.3;
    }
    if (_isLeftButtonPress)
    {
        painter.drawEllipse(QPointF(_circleCenterX, height() / 2), _circleRadius, _circleRadius);
    }
    else
    {
        if (_circleCenterX == -1)
        {
            _circleCenterX = _isToggled ? width() - height() / 2 - _margin * 2 : height() / 2;
        }
        painter.drawEllipse(QPointF(_circleCenterX, height() / 2), _circleRadius, _circleRadius);
    }
    painter.restore();
}

void ToggleSwitch::startPosAnimation(qreal startX, qreal endX, bool isToggle)
{
    _isToggled = isToggle;
    emit toggled(isToggle);
    
    QVariantAnimation* circleAnimation = new QVariantAnimation(this);
    circleAnimation->setStartValue(startX);
    circleAnimation->setEndValue(endX);
    circleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(circleAnimation, &QVariantAnimation::valueChanged, this, [=](const QVariant& value) {
        _circleCenterX = value.toReal();
        update();
    });
    connect(circleAnimation, &QVariantAnimation::finished, circleAnimation, &QVariantAnimation::deleteLater);
    circleAnimation->start();
}

void ToggleSwitch::startRadiusAnimation(qreal startRadius, qreal endRadius)
{
    QVariantAnimation* circleRadiusAnimation = new QVariantAnimation(this);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(circleRadiusAnimation, &QVariantAnimation::valueChanged, this, [=](const QVariant& value) {
        _circleRadius = value.toReal();
        update();
    });
    connect(circleRadiusAnimation, &QVariantAnimation::finished, circleRadiusAnimation, &QVariantAnimation::deleteLater);
    circleRadiusAnimation->start();
}

void ToggleSwitch::adjustCircleCenterX()
{
    if (_circleCenterX > width() - height() / 2 - _margin * 2)
    {
        _circleCenterX = width() - height() / 2 - _margin * 2;
    }
    if (_circleCenterX < height() / 2)
    {
        _circleCenterX = height() / 2;
    }
}

QSize ToggleSwitch::sizeHint() const
{
    return QSize(44, 22);
}

QSize ToggleSwitch::minimumSizeHint() const
{
    return QSize(44, 22);
}

} // namespace Widgets
