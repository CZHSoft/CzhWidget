/******************************************************************************
 *
 * This file is part of Log4Qt library.
 *
 * Copyright (C) 2007 - 2026 Log4Qt contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef LOG4QT_CRONTRIGGERINGPOLICY_H
#define LOG4QT_CRONTRIGGERINGPOLICY_H

#include "triggeringpolicy.h"
#include "log4qt/helpers/cronexpression.h"

#include <QDateTime>

namespace Log4Qt
{

/*!
 * \brief The class CronTriggeringPolicy triggers a rollover based on
 *        a cron expression schedule, similar to log4j2's CronTriggeringPolicy.
 *
 * The cron expression uses the Quartz format with 6 fields:
 *   seconds minutes hours day-of-month month day-of-week
 *
 * The default schedule is "0 0 0 * * ?" (midnight every day).
 *
 * Example configurations:
 * - \c "0 0 0 * * ?"       -- every day at midnight
 * - \c "0 0 0/4 * * ?"     -- every 4 hours starting at midnight
 * - \c "0 0 0 1 * ?"       -- first day of every month at midnight
 * - \c "0 0 8 ? * MON-FRI" -- weekdays at 8:00 AM
 *
 * \note All the functions declared in this class are thread-safe.
 */
class LOG4QT_EXPORT CronTriggeringPolicy : public TriggeringPolicy
{
    Q_OBJECT

    /*!
     * The cron expression that determines the rollover schedule.
     * Default: "0 0 0 * * ?" (midnight every day).
     */
    Q_PROPERTY(QString schedule READ schedule WRITE setSchedule)

public:
    explicit CronTriggeringPolicy(QObject *parent = nullptr);

    QString schedule() const { return mSchedule; }
    void setSchedule(const QString &schedule) { mSchedule = schedule; }

    void activateOptions() override;

    bool isTriggeringEvent(QIODevice *activeDevice,
                           const LoggingEvent &event) override;

private:
    Q_DISABLE_COPY_MOVE(CronTriggeringPolicy)

    void computeNextFireTime();

    QString mSchedule;
    CronExpression mCronExpression;
    QDateTime mNextFireTime;
};

} // namespace Log4Qt

#endif // LOG4QT_CRONTRIGGERINGPOLICY_H
