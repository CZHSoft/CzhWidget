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

#ifndef LOG4QT_JSONLAYOUT_H
#define LOG4QT_JSONLAYOUT_H

#include "abstractstringlayout.h"

namespace Log4Qt
{

/*!
 * \brief Layout that formats each log event as a single-line JSON object.
 *
 * \par Output format
 * Each event is serialised as a compact JSON object followed by a newline,
 * suitable for NDJSON / JSON Lines log aggregators (Logstash, Fluentd,
 * Vector, …):
 * \code
 * {"timestamp":1743840000123,"level":"INFO","logger":"MyApp","thread":"main","message":"Started"}
 * \endcode
 *
 * The fields included in each object are controlled by the
 * \c include* properties. All fields default to \c true except
 * \c includeNdc, \c includeMdc, and \c includeLocation, which default to
 * \c false to keep the output compact.
 *
 * \par JSON array output
 * To produce a single valid JSON array across a whole file, set
 * \c header to \c "[" and \c footer to \c "]" on this layout and enable
 * \c prettyPrint if human readability is required. A rolling appender will
 * then write one valid JSON array per file.
 *
 * \par Timestamp
 * The \c "timestamp" field contains the Unix epoch in milliseconds
 * (a JSON number). This is intentionally a raw number rather than a
 * formatted string so that consumers can parse it without locale issues.
 *
 * \par Content type
 * \c contentType() returns \c "application/json; charset=UTF-8".
 * The \c charset property inherited from \c AbstractStringLayout has no
 * effect: JSON output is always encoded as UTF-8.
 *
 * \note The ownership and lifetime of objects of this class are managed. See
 *       \ref Ownership "Object ownership" for more details.
 *
 * \sa AbstractStringLayout, PatternLayout
 */
class LOG4QT_EXPORT JsonLayout : public AbstractStringLayout
{
    Q_OBJECT

    /*!
     * Include the \c "timestamp" field (epoch ms). Default: \c true.
     */
    Q_PROPERTY(bool includeTimestamp READ includeTimestamp WRITE setIncludeTimestamp)

    /*!
     * Include the \c "level" field. Default: \c true.
     */
    Q_PROPERTY(bool includeLevel READ includeLevel WRITE setIncludeLevel)

    /*!
     * Include the \c "logger" field. Default: \c true.
     */
    Q_PROPERTY(bool includeLogger READ includeLogger WRITE setIncludeLogger)

    /*!
     * Include the \c "thread" field. Default: \c true.
     */
    Q_PROPERTY(bool includeThread READ includeThread WRITE setIncludeThread)

    /*!
     * Include the \c "message" field. Default: \c true.
     */
    Q_PROPERTY(bool includeMessage READ includeMessage WRITE setIncludeMessage)

    /*!
     * Include the \c "ndc" field. Omitted when the NDC is empty.
     * Default: \c false.
     */
    Q_PROPERTY(bool includeNdc READ includeNdc WRITE setIncludeNdc)

    /*!
     * Include the \c "mdc" object. Omitted when the MDC is empty.
     * Default: \c false.
     */
    Q_PROPERTY(bool includeMdc READ includeMdc WRITE setIncludeMdc)

    /*!
     * Include \c "file", \c "line", and \c "function" fields.
     * Default: \c false.
     *
     * \sa requiresLocation()
     */
    Q_PROPERTY(bool includeLocation READ includeLocation WRITE setIncludeLocation)

    /*!
     * Produce indented JSON output. Default: \c false (compact).
     */
    Q_PROPERTY(bool prettyPrint READ prettyPrint WRITE setPrettyPrint)

public:
    explicit JsonLayout(QObject *parent = nullptr);

private:
    Q_DISABLE_COPY_MOVE(JsonLayout)

public:
    [[nodiscard]] bool includeTimestamp() const { return mIncludeTimestamp; }
    [[nodiscard]] bool includeLevel()     const { return mIncludeLevel;     }
    [[nodiscard]] bool includeLogger()    const { return mIncludeLogger;    }
    [[nodiscard]] bool includeThread()    const { return mIncludeThread;    }
    [[nodiscard]] bool includeMessage()   const { return mIncludeMessage;   }
    [[nodiscard]] bool includeNdc()       const { return mIncludeNdc;       }
    [[nodiscard]] bool includeMdc()       const { return mIncludeMdc;       }
    [[nodiscard]] bool includeLocation()  const { return mIncludeLocation;  }
    [[nodiscard]] bool prettyPrint()      const { return mPrettyPrint;      }

    void setIncludeTimestamp(bool v) { mIncludeTimestamp = v; }
    void setIncludeLevel(bool v)     { mIncludeLevel     = v; }
    void setIncludeLogger(bool v)    { mIncludeLogger    = v; }
    void setIncludeThread(bool v)    { mIncludeThread    = v; }
    void setIncludeMessage(bool v)   { mIncludeMessage   = v; }
    void setIncludeNdc(bool v)       { mIncludeNdc       = v; }
    void setIncludeMdc(bool v)       { mIncludeMdc       = v; }
    void setIncludeLocation(bool v)  { mIncludeLocation  = v; }
    void setPrettyPrint(bool v)      { mPrettyPrint      = v; }

    /*!
     * Returns \c "application/json; charset=UTF-8".
     */
    [[nodiscard]] QString contentType() const override;

    [[nodiscard]] QString format(const LoggingEvent &event) override;

    /*!
     * Returns \c true when \c includeLocation is \c true.
     */
    [[nodiscard]] bool requiresLocation() const override;

private:
    bool mIncludeTimestamp = true;
    bool mIncludeLevel     = true;
    bool mIncludeLogger    = true;
    bool mIncludeThread    = true;
    bool mIncludeMessage   = true;
    bool mIncludeNdc       = false;
    bool mIncludeMdc       = false;
    bool mIncludeLocation  = false;
    bool mPrettyPrint      = false;
};

} // namespace Log4Qt

#endif // LOG4QT_JSONLAYOUT_H
