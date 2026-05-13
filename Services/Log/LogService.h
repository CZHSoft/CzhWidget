#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#include <QObject>
#include <QString>
#include <QMutex>

#include "log4qt/include/log4qt/logger.h"
#include "log4qt/include/log4qt/loggingevent.h"

namespace Services {
namespace Log {

class LogService : public QObject
{
    Q_OBJECT

public:
    static LogService* instance();

    ~LogService() override;

    void debug(const QString& message, const QString& category = "");
    void info(const QString& message, const QString& category = "");
    void warn(const QString& message, const QString& category = "");
    void error(const QString& message, const QString& category = "");
    void fatal(const QString& message, const QString& category = "");

    void setLogLevel(Log4Qt::Level level);
    Log4Qt::Level logLevel() const;

signals:
    void logReceived(const QString& message, int level);

private:
    explicit LogService(QObject* parent = nullptr);
    LogService(const LogService&) = delete;
    LogService& operator=(const LogService&) = delete;

    ///
    /// \brief 初始化目录和配置文件
    ///
    void initLog4Qt();

    static LogService* m_instance;
    static QMutex m_mutex;

    Log4Qt::Logger* m_logger;
};

}
}

#endif // LOGSERVICE_H