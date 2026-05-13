#include "Services/Log/LogService.h"
#include "log4qt/include/log4qt/propertyconfigurator.h"
#include "log4qt/include/log4qt/basicconfigurator.h"
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>

namespace Services {
namespace Log {

LogService* LogService::m_instance = nullptr;
QMutex LogService::m_mutex;

LogService::LogService(QObject* parent)
    : QObject(parent),
      m_logger(nullptr)
{
    initLog4Qt();
}

LogService::~LogService()
{
}

LogService* LogService::instance()
{
    if (!m_instance) {
        QMutexLocker locker(&m_mutex);
        if (!m_instance) {
            m_instance = new LogService();
        }
    }
    return m_instance;
}

void LogService::initLog4Qt()
{
    QDir logsDir("logs");
    if (!logsDir.exists()) {
        logsDir.mkpath(".");
    }

    try {
        Log4Qt::PropertyConfigurator::configure("log4qt.properties");
    } catch (const Log4Qt::LogError&) {
        Log4Qt::BasicConfigurator::configure();
    }

    m_logger = Log4Qt::Logger::rootLogger();
}

void LogService::debug(const QString& message, const QString& category)
{
    QString logMessage = category.isEmpty() ? message : QString("[%1] %2").arg(category,message);
    m_logger->debug(logMessage);
    QString formattedMessage = QString("[%1] [%2] %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.SSS"),"DEBUG",logMessage);
    Q_EMIT logReceived(formattedMessage, Log4Qt::Level::DEBUG_INT);
}

void LogService::info(const QString& message, const QString& category)
{
    QString logMessage = category.isEmpty() ? message : QString("[%1] %2").arg(category,message);
    m_logger->info(logMessage);
    QString formattedMessage = QString("[%1] [%2] %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.SSS"),"INFO",logMessage);
    Q_EMIT logReceived(formattedMessage, Log4Qt::Level::INFO_INT);
}

void LogService::warn(const QString& message, const QString& category)
{
    QString logMessage = category.isEmpty() ? message : QString("[%1] %2").arg(category,message);
    m_logger->warn(logMessage);
    QString formattedMessage = QString("[%1] [%2] %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.SSS"),"WARN",logMessage);
    Q_EMIT logReceived(formattedMessage, Log4Qt::Level::WARN_INT);
}

void LogService::error(const QString& message, const QString& category)
{
    QString logMessage = category.isEmpty() ? message : QString("[%1] %2").arg(category,message);
    m_logger->error(logMessage);
    QString formattedMessage = QString("[%1] [%2] %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.SSS"),"ERROR",logMessage);
    Q_EMIT logReceived(formattedMessage, Log4Qt::Level::ERROR_INT);
}

void LogService::fatal(const QString& message, const QString& category)
{
    QString logMessage = category.isEmpty() ? message : QString("[%1] %2").arg(category,message);
    m_logger->fatal(logMessage);
    QString formattedMessage = QString("[%1] [%2] %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.SSS"),"FATAL",logMessage);
    Q_EMIT logReceived(formattedMessage, Log4Qt::Level::FATAL_INT);
}

void LogService::setLogLevel(Log4Qt::Level level)
{
    if (m_logger) {
        m_logger->setLevel(level);
    }
}

Log4Qt::Level LogService::logLevel() const
{
    return m_logger ? m_logger->level() : Log4Qt::Level::DEBUG_INT;
}

}
}