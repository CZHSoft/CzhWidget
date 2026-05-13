#include "Widgets/ComboBox.h"
#include <QShowEvent>
#include <QEvent>
#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionComboBox>
#include <QStyle>
#include <QPropertyAnimation>
#include "ElaTheme.h"
#include "ElaDef.h"

namespace Widgets {

ComboBox::ComboBox(QWidget* parent)
    : QComboBox(parent)
    , _themeMode(eTheme->getThemeMode())
    , _isInitialized(false)
    , _expandIconRotate(0)
    , _expandMarkWidth(0)
{
    setObjectName("ComboBox");
    setFixedHeight(35);

    connect(eTheme,
            SIGNAL(themeModeChanged(ElaThemeType::ThemeMode)),
            this,
            SLOT(onThemeModeChanged(ElaThemeType::ThemeMode)));
    
    setMouseTracking(true);
}

ComboBox::~ComboBox()
{
}

void ComboBox::setPlaceholderText(const QString& text)
{
    _placeholderText = text;
    update();
}

QString ComboBox::placeholderText() const
{
    return _placeholderText;
}

void ComboBox::showEvent(QShowEvent* event)
{
    QComboBox::showEvent(event);
    if (!_isInitialized) {
        _isInitialized = true;
        updateThemeColors();
    }
}

bool ComboBox::event(QEvent* event)
{
    if (event->type() == QEvent::EnabledChange) {
        updateThemeColors();
    }
    return QComboBox::event(event);
}

void ComboBox::onThemeModeChanged(ElaThemeType::ThemeMode mode)
{
    _themeMode = mode;
    updateThemeColors();
    update();
}

void ComboBox::updateThemeColors()
{
    applyThemeColors();
}

void ComboBox::applyThemeColors()
{
    QString styleSheet;

    if (!isEnabled()) {
        styleSheet = QString(
            "QComboBox {"
            "    background-color: %1;"
            "    color: %2;"
            "    border: 1px solid %3;"
            "    border-radius: 3px;"
            "    padding: 2px 8px 2px 16px;"
            "    selection-background-color: %4;"
            "}"
            "QComboBox::drop-down {"
            "    width: 0px;"
            "    border: none;"
            "}"
            "QComboBox::down-arrow {"
            "    width: 0px;"
            "    height: 0px;"
            "}"
            "QComboBox QAbstractItemView {"
            "    background-color: %5;"
            "    color: %2;"
            "    border: 1px solid %3;"
            "    selection-background-color: %4;"
            "    outline: none;"
            "}"
            "QComboBox QAbstractItemView::item {"
            "    min-height: 35px;"
            "    padding: 4px 8px;"
            "    border-radius: 2px;"
            "}"
            "QComboBox QAbstractItemView::item:hover {"
            "    background-color: %6;"
            "}"
            "QComboBox QAbstractItemView::item:selected {"
            "    background-color: %4;"
            "    color: white;"
            "}"
        ).arg(
            ElaThemeColor(_themeMode, BasicDisable).name(),
            ElaThemeColor(_themeMode, BasicTextDisable).name(),
            ElaThemeColor(_themeMode, BasicBorderDeep).name(),
            ElaThemeColor(_themeMode, PrimaryNormal).name(),
            ElaThemeColor(_themeMode, PopupBase).name(),
            ElaThemeColor(_themeMode, BasicHover).name()
        );
    } else {
        styleSheet = QString(
            "QComboBox {"
            "    background-color: %1;"
            "    color: %2;"
            "    border: 1px solid %3;"
            "    border-radius: 3px;"
            "    padding: 2px 8px 2px 16px;"
            "    selection-background-color: %4;"
            "}"
            "QComboBox::drop-down {"
            "    width: 0px;"
            "    border: none;"
            "}"
            "QComboBox::down-arrow {"
            "    width: 0px;"
            "    height: 0px;"
            "}"
            "QComboBox:hover {"
            "    background-color: %5;"
            "}"
            "QComboBox:focus {"
            "    border: 1px solid %4;"
            "}"
            "QComboBox QAbstractItemView {"
            "    background-color: %6;"
            "    color: %2;"
            "    border: 1px solid %3;"
            "    selection-background-color: %4;"
            "    outline: none;"
            "}"
            "QComboBox QAbstractItemView::item {"
            "    min-height: 35px;"
            "    padding: 4px 8px;"
            "    border-radius: 2px;"
            "}"
            "QComboBox QAbstractItemView::item:hover {"
            "    background-color: %5;"
            "}"
            "QComboBox QAbstractItemView::item:selected {"
            "    background-color: %4;"
            "    color: white;"
            "}"
        ).arg(
            ElaThemeColor(_themeMode, BasicBase).name(),
            ElaThemeColor(_themeMode, BasicText).name(),
            ElaThemeColor(_themeMode, BasicBorder).name(),
            ElaThemeColor(_themeMode, PrimaryNormal).name(),
            ElaThemeColor(_themeMode, BasicHover).name(),
            ElaThemeColor(_themeMode, PopupBase).name()
        );
    }

    setStyleSheet(styleSheet);
}

void ComboBox::paintEvent(QPaintEvent* event)
{
    // 先让 Qt 绘制标准内容
    QComboBox::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect comboBoxRect = rect();
    comboBoxRect.adjust(6, 1, -6, -1);
    
    // 绘制底边线
    painter.setPen(ElaThemeColor(_themeMode, BasicBaseLine));
    painter.drawLine(comboBoxRect.x() + 3, comboBoxRect.y() + comboBoxRect.height(),
                     comboBoxRect.x() + comboBoxRect.width() - 3, comboBoxRect.y() + comboBoxRect.height());

    // 绘制展开指示器标记
    if (_expandMarkWidth > 0) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
        int markWidth = qMin(_expandMarkWidth, 20.0);
        painter.drawRoundedRect(QRectF(comboBoxRect.center().x() - markWidth,
                                        comboBoxRect.y() + comboBoxRect.height() - 3,
                                        markWidth * 2, 3), 2, 2);
    }

    int arrowSize = 5;  // 箭头大小
    int iconAreaWidth = 30;
    int iconAreaLeft = comboBoxRect.right() - 18;
    QPointF arrowCenter(iconAreaLeft + iconAreaWidth / 2.0, comboBoxRect.center().y());

    painter.save();
    painter.translate(arrowCenter);
    painter.rotate(_expandIconRotate);

    // 颜色
    QColor textColor;
    if (!isEnabled()) {
        textColor = ElaThemeColor(_themeMode, BasicTextDisable);
    } else {
        textColor = ElaThemeColor(_themeMode, BasicText);
    }

    painter.setPen(QPen(textColor, 1));  // 线条颜色 + 粗细
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(-arrowSize, 2, 0, -2);    // 左斜线
    painter.drawLine(0, -2, arrowSize, 2);     // 右斜线

    painter.restore();

}

void ComboBox::showPopup()
{
    // 禁用默认动画
    bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo);
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
    
    QComboBox::showPopup();
    
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);
    
    if (count() > 0) {
        // 箭头旋转动画
        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "expandIconRotate");
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(_expandIconRotate);
        rotateAnimation->setEndValue(-180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        
        // 展开标记动画
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "expandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(_expandMarkWidth);
        markAnimation->setEndValue(width() / 2 - 3 - 6);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void ComboBox::hidePopup()
{
    QComboBox::hidePopup();
    
    if (count() > 0) {
        // 箭头旋转回原始位置
        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "expandIconRotate");
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(_expandIconRotate);
        rotateAnimation->setEndValue(0);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        
        // 展开标记动画
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "expandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(_expandMarkWidth);
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

} // namespace Widgets
