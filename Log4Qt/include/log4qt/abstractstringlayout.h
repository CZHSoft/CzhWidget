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

#ifndef LOG4QT_ABSTRACTSTRINGLAYOUT_H
#define LOG4QT_ABSTRACTSTRINGLAYOUT_H

#include "abstractlayout.h"

#include <QByteArray>

namespace Log4Qt
{

/*!
 * \brief Abstract base class for all layouts that produce text output.
 *
 * AbstractStringLayout sits between \c Layout and the concrete text-producing
 * layouts (PatternLayout, TTCCLayout, SimpleLayout, SimpleTimeLayout,
 * JsonLayout, …). It adds three capabilities inspired by the log4j 2
 * AbstractStringLayout design:
 *
 * \li \b Charset management — a \c charset Q_PROPERTY (default \c "UTF-8")
 *     that is included in \c contentType() and used by the default
 *     \c formatTo() implementation when encoding the formatted string to bytes.
 *
 * \li \b Direct byte-encoding path — \c formatTo() writes the encoded event
 *     directly into a caller-supplied \c QByteArray, avoiding the temporary
 *     \c QString copy that would result from calling \c format() followed by
 *     \c toUtf8(). Appenders that work with raw bytes (e.g.
 *     \c RandomAccessFileAppender) should call \c formatTo() in their
 *     \c preAppend() or \c append() implementations instead of
 *     \c format().toUtf8().
 *
 * \li \b Thread-local scratch buffer — \c threadLocalBuffer() returns a
 *     per-thread \c QByteArray backed by \c thread_local storage. Appenders
 *     that call \c formatTo() outside the appender lock (see
 *     \c AppenderSkeleton::preAppend()) should use this buffer as the
 *     destination, clear it before use, and consume it in \c append().
 *
 * \note The ownership and lifetime of objects of this class are managed. See
 *       \ref Ownership "Object ownership" for more details.
 *
 * \sa Layout, PatternLayout, JsonLayout, AppenderSkeleton::preAppend()
 */
class LOG4QT_EXPORT AbstractStringLayout : public AbstractLayout
{
    Q_OBJECT

    /*!
     * The character set (IANA encoding name) used to encode the formatted
     * string to bytes in \c formatTo() and reported in \c contentType().
     *
     * The default is \c "UTF-8".
     *
     * \sa charset(), setCharset()
     */
    Q_PROPERTY(QString charset READ charset WRITE setCharset)

public:
    explicit AbstractStringLayout(QObject *parent = nullptr);

private:
    Q_DISABLE_COPY_MOVE(AbstractStringLayout)

public:
    [[nodiscard]] QString charset() const { return mCharset; }
    void setCharset(const QString &charset) { mCharset = charset; }

    /*!
     * Returns \c "text/plain; charset=<charset>".
     *
     * Subclasses should override this to return a more specific MIME type
     * (e.g. \c "application/json; charset=UTF-8").
     */
    [[nodiscard]] QString contentType() const override;

    /*!
     * Formats \a event and appends the encoded bytes to \a dest.
     *
     * The default implementation calls \c format(event).toUtf8() and appends
     * the result to \a dest. Subclasses may override this method to write
     * directly into the byte array, avoiding the intermediate \c QString
     * allocation entirely.
     *
     * \note \a dest is \b not cleared before writing; the caller is
     *       responsible for clearing it when a fresh buffer is needed.
     *
     * \sa threadLocalBuffer()
     */
    virtual void formatTo(const LoggingEvent &event, QByteArray &dest);

    /*!
     * Returns a reference to the calling thread's scratch buffer.
     *
     * The buffer is backed by \c thread_local storage and lives for the
     * lifetime of the calling thread. Callers that need a clean slate must
     * call \c QByteArray::clear() before use.
     *
     * Intended for use by appenders inside \c preAppend(): fill this buffer
     * via \c formatTo() outside the appender lock, then consume it in
     * \c append() under the lock.
     *
     * \sa formatTo(), AppenderSkeleton::preAppend()
     */
    static QByteArray &threadLocalBuffer();

private:
    QString mCharset;
};

} // namespace Log4Qt

#endif // LOG4QT_ABSTRACTSTRINGLAYOUT_H
