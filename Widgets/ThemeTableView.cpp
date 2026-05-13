#include "ThemeTableView.h"
#include <QPainter>
#include <QHeaderView>

namespace Widgets {

ThemeTableView::ThemeTableView(QWidget* parent)
    : QTableView(parent)
    , _themeMode(ElaThemeType::Light)
{
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    
    applyThemeStyle();
}

ThemeTableView::~ThemeTableView()
{
}

void ThemeTableView::paintEvent(QPaintEvent* event)
{
    QTableView::paintEvent(event);
}

bool ThemeTableView::event(QEvent* event)
{
    if (event->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent* propEvent = static_cast<QDynamicPropertyChangeEvent*>(event);
        if (propEvent->propertyName() == "themeMode") {
            applyThemeStyle();
        }
    }
    return QTableView::event(event);
}

void ThemeTableView::onThemeModeChanged(ElaThemeType::ThemeMode mode)
{
    _themeMode = mode;
    applyThemeStyle();
}

void ThemeTableView::applyThemeStyle()
{
    QString styleSheet;

    if (_themeMode == ElaThemeType::Dark) {
        styleSheet = R"(
            QTableView {
                background-color: #2d2d30;
                color: #ffffff;
                border: 1px solid #464647;
                gridline-color: #3f3f46;
                selection-background-color: #007acc;
                selection-color: #ffffff;
            }
            QTableView::item {
                padding: 5px;
                border-bottom: 1px solid #3f3f46;
            }
            QTableView::item:alternate {
                background-color: #252526;
            }
            QTableView::item:selected {
                background-color: #007acc;
            }
            QHeaderView::section {
                background-color: #333337;
                color: #ffffff;
                padding: 5px;
                border: none;
                border-bottom: 2px solid #007acc;
                border-right: 1px solid #3f3f46;
            }
        )";
    } else {
        styleSheet = R"(
            QTableView {
                background-color: #ffffff;
                color: #000000;
                border: 1px solid #dcdcdc;
                gridline-color: #dcdcdc;
                selection-background-color: #e5f3ff;
                selection-color: #000000;
            }
            QTableView::item {
                padding: 5px;
                border-bottom: 1px solid #f0f0f0;
            }
            QTableView::item:alternate {
                background-color: #fafafa;
            }
            QTableView::item:selected {
                background-color: #e5f3ff;
            }
            QHeaderView::section {
                background-color: #f5f5f5;
                color: #000000;
                padding: 5px;
                border: none;
                border-bottom: 2px solid #4a90e2;
                border-right: 1px solid #dcdcdc;
            }
        )";
    }

    setStyleSheet(styleSheet);
}

}
