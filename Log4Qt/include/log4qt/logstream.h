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

#ifndef LOG4QT_LOGSTREAM_H
#define LOG4QT_LOGSTREAM_H

#include "level.h"

#include <QTextStream>
#include <QString>
#include <QPointer>

#include <memory>

#ifdef __cpp_concepts
#include <concepts>

template<typename T>
concept QTextStreamable = requires(QTextStream &ts, const T &t) {
    { ts << t } -> std::same_as<QTextStream &>;
};
#endif

namespace Log4Qt
{
class Logger;

class LOG4QT_EXPORT LogStream
{
public:
    //! Constructs a LogStream for the given logger and level.
    //! If the level is disabled on the logger, no internal stream is allocated
    //! and all data streamed via operator<< is silently discarded.
    LogStream(const Logger &iLogger, Level iLevel);
#ifdef __cpp_concepts
    template<QTextStreamable T>
#else
    template<typename T>
#endif
    LogStream &operator<<(const T &t)
    {
        if (stream)
            stream->ts << t;
        return *this;
    }

private:
    struct Stream
    {
        Stream(const Logger *iLogger, Level iLevel);
        ~Stream();

        QTextStream ts;
        QString buffer;
        QPointer<const Logger> mLogger;
        Level mLevel;
    };
    std::shared_ptr<Stream> stream;
};
}

#endif // LOG4QT_LOGSTREAM_H
