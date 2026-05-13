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

#ifndef LOG4QT_DEFAULTROLLOVERSTRATEGY_H
#define LOG4QT_DEFAULTROLLOVERSTRATEGY_H

#include "rolloverstrategy.h"

namespace Log4Qt
{

/*!
 * \brief The class DefaultRolloverStrategy performs a fixed-window numbered
 *        rotation of backup files.
 *
 * When a rollover occurs the strategy:
 * 1. Deletes the oldest backup file (at maxIndex)
 * 2. Shifts numbered backups up: .N -> .N+1
 * 3. Renames the base file to .minIndex
 *
 * \note All the functions declared in this class are thread-safe.
 */
class LOG4QT_EXPORT DefaultRolloverStrategy : public RolloverStrategy
{
    Q_OBJECT

    /*!
     * The minimum index for backup files.
     * The default is 1.
     */
    Q_PROPERTY(int minIndex READ minIndex WRITE setMinIndex)

    /*!
     * The maximum index for backup files.
     * The default is 7.
     */
    Q_PROPERTY(int maxIndex READ maxIndex WRITE setMaxIndex)

public:
    static constexpr int defaultMinIndex = 1;
    static constexpr int defaultMaxIndex = 7;

    explicit DefaultRolloverStrategy(QObject *parent = nullptr);

    [[nodiscard]] int minIndex() const { return mMinIndex; }
    void setMinIndex(int minIndex) { mMinIndex = minIndex; }

    [[nodiscard]] int maxIndex() const { return mMaxIndex; }
    void setMaxIndex(int maxIndex) { mMaxIndex = maxIndex; }

    QString rollover(const QString &fileName) override;

private:
    Q_DISABLE_COPY_MOVE(DefaultRolloverStrategy)
    int mMinIndex;
    int mMaxIndex;
};

} // namespace Log4Qt

#endif // LOG4QT_DEFAULTROLLOVERSTRATEGY_H
