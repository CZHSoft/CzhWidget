#include "Widgets/FunctionNavCard.h"
#include "ElaText.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOption>

namespace Widgets {

FunctionNavCard::FunctionNavCard(QWidget* parent)
    : QWidget(parent)
    , _iconSize(48, 48)
    , _isHovered(false)
    , _isPressed(false)
{
    setFixedSize(220, 140);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);

    setStyleSheet(R"(
        FunctionNavCard {
            background-color: rgba(255, 255, 255, 0.95);
            border-radius: 10px;
        }
    )");
}

FunctionNavCard::~FunctionNavCard()
{
}

void FunctionNavCard::setIcon(const QPixmap& pixmap)
{
    _iconPixmap = pixmap;
    update();
}

void FunctionNavCard::setIconSize(const QSize& size)
{
    _iconSize = size;
    update();
}

void FunctionNavCard::setTitle(const QString& title)
{
    _title = title;
    update();
}

void FunctionNavCard::setDescription(const QString& description)
{
    _description = description;
    update();
}

void FunctionNavCard::setPageName(const QString& pageName)
{
    _pageName = pageName;
}

void FunctionNavCard::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        _isPressed = true;
        updateStyle();
    }
    QWidget::mousePressEvent(event);
}

void FunctionNavCard::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && _isPressed) {
        _isPressed = false;
        updateStyle();
        Q_EMIT cardClicked(_pageName);
    }
    QWidget::mouseReleaseEvent(event);
}

void FunctionNavCard::enterEvent(QEnterEvent* event)
{
    _isHovered = true;
    updateStyle();
    QWidget::enterEvent(event);
}

void FunctionNavCard::leaveEvent(QEvent* event)
{
    _isHovered = false;
    updateStyle();
    QWidget::leaveEvent(event);
}

void FunctionNavCard::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgColor(255, 255, 255);
    if (_isPressed) {
        bgColor = QColor(230, 230, 230);
    } else if (_isHovered) {
        bgColor = QColor(245, 245, 250);
    }

    painter.setBrush(bgColor);
    painter.setPen(QColor(200, 200, 210));
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 10, 10);

    int margin = 15;
    int iconY = margin + 10;
    int iconX = (width() - _iconSize.width()) / 2;

    if (!_iconPixmap.isNull()) {
        QPixmap scaledIcon = _iconPixmap.scaled(_iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(iconX, iconY, scaledIcon);
    }

    int textY = iconY + _iconSize.height() + 15;
    QFont titleFont = painter.font();
    titleFont.setPixelSize(16);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(QColor(30, 30, 30));
    painter.drawText(QPoint((width() - painter.fontMetrics().horizontalAdvance(_title)) / 2, textY), _title);

    int descY = textY + 20;
    QFont descFont = painter.font();
    descFont.setPixelSize(12);
    descFont.setBold(false);
    painter.setFont(descFont);
    painter.setPen(QColor(100, 100, 100));
    QString elidedText = painter.fontMetrics().elidedText(_description, Qt::ElideRight, width() - 30);
    painter.drawText(QRect(margin, descY, width() - 2 * margin, 30), Qt::AlignCenter | Qt::TextWordWrap, elidedText);
}

void FunctionNavCard::updateStyle()
{
    update();
}

}
