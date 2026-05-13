#include "Views/Pages/LogsPage.h"
#include "Services/Log/LogService.h"
#include "log4qt/include/log4qt/level.h"
#include "ElaText.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QColor>
#include <QScrollBar>

namespace Views {
namespace Pages {

LogsPage::LogsPage(QWidget* parent)
    : ElaScrollPage(parent),
      m_logTextEdit(nullptr),
      m_mainLayout(nullptr),
      m_titleLabel(nullptr)
{
    setWindowTitle(tr("Logs"));
    setTitleVisible(false);

    initUI();

    connect(Services::Log::LogService::instance(),
            SIGNAL(logReceived(const QString&, int)),
            this,
            SLOT(onLogReceived(const QString&, int)));
}

LogsPage::~LogsPage()
{
}

void LogsPage::initUI()
{
    QWidget* centralWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(centralWidget);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);

    m_titleLabel = new ElaText(tr("System Logs"), this);
    m_titleLabel->setWordWrap(false);
    m_titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    m_mainLayout->addWidget(m_titleLabel);

    m_logTextEdit = new QTextEdit(this);
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    m_logTextEdit->setWordWrapMode(QTextOption::NoWrap);

    QPalette palette = m_logTextEdit->palette();
    palette.setColor(QPalette::Base, QColor("#1a1a1a"));
    palette.setColor(QPalette::Text, QColor("#ffffff"));
    m_logTextEdit->setPalette(palette);
    m_logTextEdit->setStyleSheet(R"(
        QTextEdit {
            background-color: #1a1a1a;
            color: #ffffff;
            border: 1px solid #333333;
            border-radius: 4px;
            padding: 10px;
            font-family: Consolas, "Courier New", monospace;
            font-size: 12px;
        }
        QTextEdit QScrollBar:vertical {
            background-color: #2a2a2a;
            width: 12px;
        }
        QTextEdit QScrollBar::handle:vertical {
            background-color: #4a4a4a;
            border-radius: 6px;
        }
        QTextEdit QScrollBar::handle:vertical:hover {
            background-color: #5a5a5a;
        }
        QTextEdit QScrollBar:horizontal {
            background-color: #2a2a2a;
            height: 12px;
        }
        QTextEdit QScrollBar::handle:horizontal {
            background-color: #4a4a4a;
            border-radius: 6px;
        }
    )");

    m_mainLayout->addWidget(m_logTextEdit);
    m_mainLayout->setStretchFactor(m_logTextEdit, 1);

    addCentralWidget(centralWidget, true, false, 0);
}

void LogsPage::onLogReceived(const QString& message, int level)
{
    QColor textColor = getLevelColor(level);

    QString htmlMessage = QString("<span style=\"color: %1;\">%2</span>")
        .arg(textColor.name())
        .arg(message.toHtmlEscaped());

    m_logTextEdit->append(htmlMessage);

    QScrollBar* scrollBar = m_logTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

QColor LogsPage::getLevelColor(int level)
{
    switch (level) {
    case Log4Qt::Level::DEBUG_INT:
        return QColor("#4DA6FF");
    case Log4Qt::Level::INFO_INT:
        return QColor("#00FF00");
    case Log4Qt::Level::WARN_INT:
        return QColor("#FFCC00");
    case Log4Qt::Level::ERROR_INT:
        return QColor("#FF4444");
    case Log4Qt::Level::FATAL_INT:
        return QColor("#FF0066");
    default:
        return QColor("#CCCCCC");
    }
}

QString LogsPage::getLevelName(int level)
{
    switch (level) {
    case Log4Qt::Level::DEBUG_INT:
        return tr("DEBUG");
    case Log4Qt::Level::INFO_INT:
        return tr("INFO");
    case Log4Qt::Level::WARN_INT:
        return tr("WARN");
    case Log4Qt::Level::ERROR_INT:
        return tr("ERROR");
    case Log4Qt::Level::FATAL_INT:
        return tr("FATAL");
    default:
        return tr("UNKNOWN");
    }
}

void LogsPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    ElaScrollPage::changeEvent(event);
}

void LogsPage::retranslateUi()
{
    setWindowTitle(tr("Logs"));
    if (m_titleLabel) {
        m_titleLabel->setText(tr("System Logs"));
    }
}

}
}