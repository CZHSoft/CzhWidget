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

#ifndef LOG4QT_TRIGGERINGPOLICY_H
#define LOG4QT_TRIGGERINGPOLICY_H

#include "log4qt/log4qt.h"
#include "log4qt/log4qtsharedptr.h"

#include <QObject>

class QIODevice;

namespace Log4Qt
{

class LoggingEvent;
class TriggeringPolicy;

using TriggeringPolicySharedPtr = Log4QtSharedPtr<TriggeringPolicy>;

/*!
 * \brief The class TriggeringPolicy is the base class for all triggering
 *        policies that determine when a rollover should occur.
 *
 * Inspired by log4j2's TriggeringPolicy interface.
 *
 * \note The ownership and lifetime of objects of this class are managed.
 *       See \ref Ownership "Object ownership" for more details.
 */
class LOG4QT_EXPORT TriggeringPolicy : public QObject
{
    Q_OBJECT

public:
    explicit TriggeringPolicy(QObject *parent = nullptr);
    ~TriggeringPolicy() override;

    virtual void activateOptions();

    /*!
     * Returns \c true if a rollover should be triggered.
     *
     * \param activeDevice The underlying QIODevice of the active log file.
     *        Policies that need the file size (e.g. SizeBasedTriggeringPolicy)
     *        call \c activeDevice->pos() which reads a cached value (no syscall).
     *        All other policies ignore this parameter.
     * \param event The logging event that was just written.
     */
    virtual bool isTriggeringEvent(QIODevice *activeDevice,
                                   const LoggingEvent &event) = 0;

    /*!
     * Returns \c true if a rollover should be triggered at startup.
     * The default implementation returns \c false.
     *
     * \param fileName The current active log file path.
     * \param fileSize The current file size in bytes.
     */
    virtual bool isStartupTrigger(const QString &fileName, qint64 fileSize);

private:
    Q_DISABLE_COPY_MOVE(TriggeringPolicy)
};

} // namespace Log4Qt

#endif // LOG4QT_TRIGGERINGPOLICY_H
