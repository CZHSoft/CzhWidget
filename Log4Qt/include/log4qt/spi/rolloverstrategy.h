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

#ifndef LOG4QT_ROLLOVERSTRATEGY_H
#define LOG4QT_ROLLOVERSTRATEGY_H

#include "log4qt/log4qt.h"
#include "log4qt/log4qtsharedptr.h"

#include <QObject>

namespace Log4Qt
{

class RolloverStrategy;

using RolloverStrategySharedPtr = Log4QtSharedPtr<RolloverStrategy>;

/*!
 * \brief The class RolloverStrategy is the base class for all rollover
 *        strategies that determine how a rollover is performed.
 *
 * Inspired by log4j2's RolloverStrategy interface.
 *
 * \note The ownership and lifetime of objects of this class are managed.
 *       See \ref Ownership "Object ownership" for more details.
 */
class LOG4QT_EXPORT RolloverStrategy : public QObject
{
    Q_OBJECT

public:
    explicit RolloverStrategy(QObject *parent = nullptr);
    ~RolloverStrategy() override;

    virtual void activateOptions();

    /*!
     * Returns the initial active file path the appender should open on startup.
     * The default returns \a fileName unchanged. Strategies may override this
     * to provide a dated or indexed initial filename without needing a rollover.
     */
    virtual QString initialFileName(const QString &fileName) const;

    /*!
     * Performs the rollover for the given file.
     *
     * The strategy is responsible for rotating/renaming/deleting old
     * backup files. It must NOT close or open the active file (the
     * appender handles that).
     *
     * \param fileName The base log file path.
     * \return The file path the appender should open next
     *         (usually the same as \a fileName).
     */
    virtual QString rollover(const QString &fileName) = 0;

protected:
    static bool removeFile(const QString &fileName);
    static bool renameFile(const QString &source, const QString &target);

private:
    Q_DISABLE_COPY_MOVE(RolloverStrategy)
};

} // namespace Log4Qt

#endif // LOG4QT_ROLLOVERSTRATEGY_H
