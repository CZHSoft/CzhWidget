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

#ifndef LOG4QT_RANDOMACCESSFILEAPPENDER_H
#define LOG4QT_RANDOMACCESSFILEAPPENDER_H

#include "appenderskeleton.h"

#include <atomic>
#include <memory>

class QFile;

namespace Log4Qt
{

/*!
 * \brief High-throughput file appender that bypasses QTextStream and formats
 *        log events outside the appender lock.
 *
 * \par Motivation
 * FileAppender routes every log event through a QTextStream, which adds a
 * codec-conversion layer and holds the appender mutex for the full duration
 * of formatting \e and writing. For workloads with many concurrent threads or
 * expensive layout patterns (e.g. \c %d{ISO8601}), this serialisation
 * dominates the cost of logging.
 *
 * \par Design
 * RandomAccessFileAppender improves on FileAppender in two complementary ways:
 *
 * \b 1. Direct byte-buffer I/O
 *
 * Instead of writing through QTextStream, formatted messages are encoded to
 * UTF-8 and accumulated in a pre-allocated \c QByteArray buffer
 * (\ref bufferSize, default 256 KB). The buffer is flushed to disk via a
 * single \c QFile::write() call when it fills or when the appender is closed.
 * This eliminates the QTextStream abstraction layer and reduces the number of
 * write syscalls.
 *
 * \b 2. Split-lock formatting (concurrent throughput)
 *
 * The appender overrides \c preAppend(), which \c AppenderSkeleton::doAppend()
 * calls \e outside the appender mutex. Layout formatting and UTF-8 encoding
 * happen in \c preAppend() into a thread-local staging buffer. The mutex is
 * only held for the subsequent \c append() call that copies those bytes into
 * the shared write buffer. As a result, multiple threads can format log events
 * concurrently; only the buffer copy is serialised.
 *
 * \par Performance (measured, PatternLayout \c %d{ISO8601} [%t] %-5p %c - %m%n)
 * \code
 * Threads │ FileAppender │ RandomAccessFileAppender │ Speedup
 * ────────┼──────────────┼──────────────────────────┼────────
 *  1      │   30 ms/1k   │   29 ms/1k               │  ~1×
 *  2      │  421 ms/10k  │  212 ms/10k              │  ~2×
 *  4      │  863 ms/20k  │  318 ms/20k              │  ~2.7×
 *  8      │ 1681 ms/40k  │ 1348 ms/40k              │  ~1.25×
 * \endcode
 * Single-threaded gain is modest (the datetime formatting cost is unchanged);
 * the benefit scales with thread count until the write syscall becomes the
 * bottleneck.
 *
 * \par Differences from FileAppender
 * \li Inherits \c AppenderSkeleton directly — no QTextStream involved.
 * \li \c immediateFlush defaults to \c false (FileAppender defaults to
 *     \c true). Set it to \c true when log entries must reach disk
 *     immediately, e.g. when debugging crashes.
 * \li Adds \ref bufferSize property (default 262144 bytes / 256 KB).
 * \li Log file is opened without \c QIODevice::Text; the layout's
 *     \c endOfLine() already provides the correct platform line ending.
 * \li All buffered data is guaranteed to be flushed to disk in the
 *     destructor, even if \c close() is never called explicitly.
 *
 * \par Pairing with AsyncAppender
 * For maximum throughput, wrap this appender with \c AsyncAppender.
 * \c AsyncAppender queues \c LoggingEvent objects and dispatches them on a
 * background thread, so the calling threads never block on I/O at all.
 * \c RandomAccessFileAppender then provides fast bulk writes on the consumer
 * side.
 *
 * \note All the functions declared in this class are thread-safe.
 *
 * \note The ownership and lifetime of objects of this class are managed. See
 *       \ref Ownership "Object ownership" for more details.
 *
 * \sa FileAppender, AsyncAppender, AppenderSkeleton::preAppend()
 */
class LOG4QT_EXPORT RandomAccessFileAppender : public AppenderSkeleton
{
    Q_OBJECT

    /*!
     * The property holds whether the output is appended to an existing file.
     *
     * The default is false (truncate on open).
     *
     * \sa appendFile(), setAppendFile()
     */
    Q_PROPERTY(bool appendFile READ appendFile WRITE setAppendFile)

    /*!
     * The property holds the name of the log file.
     *
     * \sa file(), setFile()
     */
    Q_PROPERTY(QString file READ file WRITE setFile)

    /*!
     * The property holds the size of the in-memory byte buffer in bytes.
     *
     * The default is 262144 (256 KB).
     *
     * \sa bufferSize(), setBufferSize()
     */
    Q_PROPERTY(int bufferSize READ bufferSize WRITE setBufferSize)

    /*!
     * The property holds whether the buffer is flushed after every append.
     *
     * The default is false.
     *
     * \sa immediateFlush(), setImmediateFlush()
     */
    Q_PROPERTY(bool immediateFlush READ immediateFlush WRITE setImmediateFlush)

public:
    explicit RandomAccessFileAppender(QObject *parent = nullptr);
    RandomAccessFileAppender(const LayoutSharedPtr &layout,
                             const QString &fileName,
                             QObject *parent = nullptr);
    RandomAccessFileAppender(const LayoutSharedPtr &layout,
                             const QString &fileName,
                             bool append,
                             QObject *parent = nullptr);
    ~RandomAccessFileAppender() override;

private:
    Q_DISABLE_COPY_MOVE(RandomAccessFileAppender)

public:
    [[nodiscard]] bool appendFile() const { return mAppendFile.load(std::memory_order_relaxed); }
    [[nodiscard]] QString file() const;
    [[nodiscard]] int bufferSize() const { return mBufferSize.load(std::memory_order_relaxed); }
    [[nodiscard]] bool immediateFlush() const { return mImmediateFlush.load(std::memory_order_relaxed); }

    void setAppendFile(bool append) { mAppendFile.store(append, std::memory_order_relaxed); }
    void setFile(const QString &fileName);
    void setBufferSize(int bufferSize);
    void setImmediateFlush(bool immediateFlush) { mImmediateFlush.store(immediateFlush, std::memory_order_relaxed); }

    bool requiresLayout() const override;

    void activateOptions() override;
    void close() override;

protected:
    /*!
     * Pre-formats the log event outside \c mObjectGuard.
     *
     * Encodes \c layout->format(event) to UTF-8 and stores the result in a
     * thread-local byte array so that \c append() can write it directly
     * without re-acquiring the layout or performing any string allocation
     * under the lock.
     */
    void preAppend(const LoggingEvent &event, const LayoutSharedPtr &layout) override;

    void append(const LoggingEvent &event) override;

    /*!
     * Tests if all entry conditions for using append() in this class are met.
     *
     * Checks that a file is open (AppenderNoOpenFileError), then delegates
     * to AppenderSkeleton::checkEntryConditions().
     */
    bool checkEntryConditions() const override;

    bool handleIoErrors() const;
    void flushBuffer();

    /*!
     * Opens the file for writing. Creates parent directories if needed and
     * expands Windows environment variables in the file path.
     *
     * Declared virtual to allow rolling subclasses to override.
     */
    virtual void openFile();
    void closeFile();

    bool removeFile(QFile &file) const;
    bool renameFile(QFile &file, const QString &fileName) const;

private:
    void closeInternal();

    std::atomic<bool> mAppendFile;
    std::atomic<int>  mBufferSize;
    std::atomic<bool> mImmediateFlush;
    QString           mFileName;      // guarded by mObjectGuard
    QByteArray        mByteBuffer;    // guarded by mObjectGuard
    std::unique_ptr<QFile> mFile;     // guarded by mObjectGuard
};

} // namespace Log4Qt

#endif // LOG4QT_RANDOMACCESSFILEAPPENDER_H
