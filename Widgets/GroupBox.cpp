#include "GroupBox.h"
#include <QPainter>
#include <QStyleOptionGroupBox>
#include <QApplication>
#include <QPalette>
#include <QPainterPath>
#include "ElaTheme.h"
#include "ElaDef.h"

namespace Widgets {

GroupBox::GroupBox(QWidget* parent)
    : QWidget(parent)
    , _title("")
    , _themeMode(ElaThemeType::Light)
{
    init();
}

GroupBox::GroupBox(const QString& title, QWidget* parent)
    : QWidget(parent)
    , _title(title)
    , _themeMode(ElaThemeType::Light)
{
    init();
}

GroupBox::~GroupBox()
{
}

void GroupBox::init()
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setBackgroundRole(QPalette::Window);
    setContentsMargins(15, 30, 15, 15);

    connect(eTheme,
            SIGNAL(themeModeChanged(ElaThemeType::ThemeMode)),
            this,
            SLOT(onThemeModeChanged(ElaThemeType::ThemeMode)));
}

QString GroupBox::getTitle() const
{
    return _title;
}

void GroupBox::setTitle(const QString& title)
{
    _title = title;
    update();
    updateGeometry();
}

QSize GroupBox::sizeHint() const
{
    return QSize(200, 150);
}

QSize GroupBox::minimumSizeHint() const
{
    return QSize(100, 100);
}

void GroupBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor backgroundColor;
    QColor borderColor;
    QColor textColor;

    if (_themeMode == ElaThemeType::Dark) {
        backgroundColor = QColor(45, 45, 48);
        borderColor = QColor(70, 70, 72);
        textColor = QColor(255, 255, 255);
    } else {
        backgroundColor = QColor(243, 243, 243);
        borderColor = QColor(200, 200, 200);
        textColor = QColor(0, 0, 0);
    }

    QRect rect = this->rect();
    int margin = 2;
    int titleHeight = 25;
    int titleWidth = _title.isEmpty() ? 0 : fontMetrics().horizontalAdvance(_title) + 20;

    painter.fillRect(rect.adjusted(margin, margin, -margin, -margin), backgroundColor);

    QPen borderPen(borderColor);
    borderPen.setWidthF(1);
    painter.setPen(borderPen);
    painter.drawRect(rect.adjusted(margin, margin, -margin, -margin));

    if (!_title.isEmpty()) {
        QRect titleBgRect(rect.left() + margin + 8, rect.top() + margin, 
                         titleWidth, titleHeight);
        
        QPainterPath titlePath;
        titlePath.addRect(titleBgRect);
        painter.fillPath(titlePath, backgroundColor);

        painter.setPen(textColor);
        painter.setFont(font());
        QRect textRect = titleBgRect.adjusted(10, 0, -10, 0);
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, _title);
    }
}

bool GroupBox::event(QEvent* event)
{
    if (event->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent* propEvent = static_cast<QDynamicPropertyChangeEvent*>(event);
        if (propEvent->propertyName() == "themeMode") {
            update();
        }
    }
    return QWidget::event(event);
}

}
