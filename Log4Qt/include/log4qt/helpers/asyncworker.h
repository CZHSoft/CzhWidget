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

#ifndef LOG4QT_HELPERS_ASYNCWORKER_H
#define LOG4QT_HELPERS_ASYNCWORKER_H

#include <QThread>

namespace Log4Qt
{

class AsyncAppender;
class LoggingEvent;
template<typename T> class BoundedBlockingQueue;

/*!
 * \brief Worker thread that drains events from a BoundedBlockingQueue
 *        and dispatches them to the AsyncAppender's attached appenders.
 *
 * Replaces the event-loop-based Dispatcher for AsyncAppender with an
 * explicit queue-draining loop for bounded backpressure support.
 */
class AsyncWorker : public QThread
{
    Q_OBJECT

public:
    AsyncWorker(AsyncAppender *appender,
                BoundedBlockingQueue<LoggingEvent> *queue,
                QObject *parent = nullptr);

protected:
    void run() override;

private:
    Q_DISABLE_COPY_MOVE(AsyncWorker)

    AsyncAppender *mAppender;
    BoundedBlockingQueue<LoggingEvent> *mQueue;
};

} // namespace Log4Qt

#endif // LOG4QT_HELPERS_ASYNCWORKER_H
