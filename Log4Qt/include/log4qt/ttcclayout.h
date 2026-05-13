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

#ifndef LOG4QT_TTCCLAYOUT_H
#define LOG4QT_TTCCLAYOUT_H

#include "abstractstringlayout.h"
#include "helpers/patternformatter.h"

#include <memory>

namespace Log4Qt
{

class LoggingEvent;

/*!
 * \brief The class TTCCLayout outputs the time, thread, logger and nested
 *        diagnostic context information of a logging event.
 *
 * \note The ownership and lifetime of objects of this class are managed.
 *       See \ref Ownership "Object ownership" for more details.
 */
class LOG4QT_EXPORT TTCCLayout : public AbstractStringLayout
{
    Q_OBJECT

    /*!
     * The property holds if the logger name is part of the formatted output.
     *
     * The default value is true for including the logger name.
     *
     * \sa categoryPrefixing(), setCategoryPrefixing()
     */
    Q_PROPERTY(bool categoryPrefixing READ categoryPrefixing WRITE setCategoryPrefixing)

    /*!
     * The property holds if the nested context information is part of the
     * formatted output.
     *
     * The default value it true for including the nested context information.
     *
     * \sa contextPrinting(), setContextPrinting()
     */
    Q_PROPERTY(bool contextPrinting READ contextPrinting WRITE setContextPrinting)

    /*!
     * The property holds the date format used by the layout.
     *
     * The default date format is "RELATIVE".
     *
     * \sa dateFormat(), setDateFormat()
     */
    Q_PROPERTY(QString dateFormat READ dateFormat WRITE setDateFormat)

    /*!
     * The property holds if the thread name is part of the formatted output.
     *
     * The default value it true for including the thread name.
     *
     * \sa threadPrinting(), setThreadPrinting()
     */
    Q_PROPERTY(bool threadPrinting READ threadPrinting WRITE setThreadPrinting)

public:
    /*!
     * The enum DateFormat defines constants for date formats.
     *
     * \sa setDateFormat(DateFormat), DateTime::toString()
     */
    enum DateFormat
    {
        /*! The none date format string is "NONE".  */
        None,
        /*!
         * The iso8601 date format string is "ISO8601". The date will be
         * formatted as yyyy-MM-dd hh:mm:ss.zzz.
         */
        Iso8601,
        /*!
         * The absolute date format string is "ABSOLUTE". The date will be
         * formatted as HH:mm:ss.zzz.
         */
        Absolute,
        /*!
         * The date date format string is "DATE". The date will be formatted
         * as MMM YYYY HH:mm:ss.zzz.
         */
        Date,
        /*!
         * The relative date format string is "RELATIVE". The date will be
         * formatted as milliseconds since start of the program.
         */
        Relative
    };
    Q_ENUM(DateFormat)

    TTCCLayout(QObject *parent = nullptr);
    TTCCLayout(const QString &dateFormat,
               QObject *parent = nullptr);

    /*!
     * Creates a TTCCLayout with the date formar value specified by
     * the \a dateFormat constant and the parent \a parent.
     */
    TTCCLayout(DateFormat dateFormat,
               QObject *parent = nullptr);

private:
    Q_DISABLE_COPY_MOVE(TTCCLayout)

public:
    [[nodiscard]] bool categoryPrefixing() const { return mCategoryPrefixing; }
    [[nodiscard]] bool contextPrinting() const { return mContextPrinting; }
    [[nodiscard]] QString dateFormat() const { return mDateFormat; }
    [[nodiscard]] bool threadPrinting() const { return mThreadPrinting; }
    void setCategoryPrefixing(bool categoryPrefixing)
    {
        mCategoryPrefixing = categoryPrefixing;
        updatePatternFormatter();
    }
    void setContextPrinting(bool contextPrinting)
    {
        mContextPrinting = contextPrinting;
        updatePatternFormatter();
    }
    void setDateFormat(const QString &dateFormat)
    {
        mDateFormat = dateFormat;
        updatePatternFormatter();
    }

    /*!
    * Sets the date format to the value specified by the \a dateFormat
    * constant.
    */
    void setDateFormat(DateFormat dateFormat);

    void setThreadPrinting(bool threadPrinting)
    {
        mThreadPrinting = threadPrinting;
        updatePatternFormatter();
    }
    virtual QString format(const LoggingEvent &event) override;

private:
    void updatePatternFormatter();

private:
    bool mCategoryPrefixing;
    bool mContextPrinting;
    QString mDateFormat;
    bool mThreadPrinting;
    std::unique_ptr<PatternFormatter> mPatternFormatter;
};

} // namespace Log4Qt

#endif // LOG4QT_TTCCLAYOUT_H
