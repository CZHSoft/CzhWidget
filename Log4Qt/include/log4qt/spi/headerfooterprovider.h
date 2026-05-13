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

#ifndef LOG4QT_HEADERFOOTERPROVIDER_H
#define LOG4QT_HEADERFOOTERPROVIDER_H

#include "log4qt/log4qt.h"
#include "log4qt/log4qtsharedptr.h"

#include <QObject>
#include <memory>

namespace Log4Qt
{

class PatternFormatter;
class HeaderFooterProvider;

using HeaderFooterProviderSharedPtr = Log4QtSharedPtr<HeaderFooterProvider>;

/*!
 * \brief Abstract base class for header/footer providers.
 *
 * A \c HeaderFooterProvider supplies the header and footer strings for a
 * layout. The application subclasses this interface and either sets it on
 * individual layout instances (\c AbstractLayout::setHeaderFooterProvider())
 * or registers it globally as a fallback for all layouts
 * (\c AbstractLayout::setGlobalHeaderFooterProvider()).
 *
 * The default implementations of \c header() and \c footer() return an empty
 * string, which means "no content provided — fall through to the next source
 * in the priority chain." Returning an empty string from an override has the
 * same fall-through semantics.
 *
 * \sa AbstractLayout::setHeaderFooterProvider(),
 *     AbstractLayout::setGlobalHeaderFooterProvider(),
 *     PatternHeaderFooterProvider
 */
class LOG4QT_EXPORT HeaderFooterProvider : public QObject
{
    Q_OBJECT

public:
    explicit HeaderFooterProvider(QObject *parent = nullptr);
    ~HeaderFooterProvider() override;

    /*!
     * Returns the header string. An empty string means "no header provided".
     */
    virtual QString header() const;

    /*!
     * Returns the footer string. An empty string means "no footer provided".
     */
    virtual QString footer() const;

    /*!
     * Called by the layout's \c activateOptions() to allow the provider to
     * perform its own initialisation. The default implementation is a no-op.
     */
    virtual void activateOptions();

private:
    Q_DISABLE_COPY_MOVE(HeaderFooterProvider)
};

/*!
 * \brief A HeaderFooterProvider that formats header and footer strings using
 * log4qt conversion patterns.
 *
 * The \c headerPattern and \c footerPattern properties accept the same
 * conversion specifiers as \c PatternLayout::conversionPattern. The patterns
 * are evaluated at the moment \c header() or \c footer() is called (i.e. at
 * file-open / file-close time), so \c \%d reflects the actual timestamp.
 *
 * \section property-specifier User-defined properties via \c %P{key}
 *
 * The \c %P{key} specifier resolves \a key against the provider object's
 * QObject property system at format time. Two usage styles are supported:
 *
 * \b Subclass with Q_PROPERTY (recommended for factory-created providers):
 * \code
 * class MyProvider : public PatternHeaderFooterProvider {
 *     Q_OBJECT
 *     Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)
 * public:
 *     QString serialNumber() const { return mSn; }
 *     void setSerialNumber(const QString &v) { mSn = v; }
 * private:
 *     QString mSn;
 * };
 *
 * auto *p = new MyProvider;
 * p->setHeaderPattern(u"S/N: %P{serialNumber} started %d{HH:mm:ss}"_s);
 * p->setSerialNumber(u"SN-001"_s);
 * \endcode
 *
 * \b Dynamic property (no subclass needed):
 * \code
 * auto *p = new PatternHeaderFooterProvider;
 * p->setHeaderPattern(u"S/N: %P{serialNumber}"_s);
 * p->setProperty("serialNumber", u"SN-001"_s);  // QObject::setProperty
 * \endcode
 *
 * \sa HeaderFooterProvider, AbstractLayout::setGlobalHeaderFooterProvider(),
 *     PatternFormatter::setPropertySource()
 */
class LOG4QT_EXPORT PatternHeaderFooterProvider : public HeaderFooterProvider
{
    Q_OBJECT

    /*!
     * The conversion pattern used to format the header string.
     *
     * Evaluated at file-open time. Useful specifiers: \c \%d (date/time),
     * \c \%r (milliseconds since program start), \c \%P{key} (application
     * property), literal text.
     *
     * \sa headerPattern(), setHeaderPattern()
     */
    Q_PROPERTY(QString headerPattern READ headerPattern WRITE setHeaderPattern)

    /*!
     * The conversion pattern used to format the footer string.
     *
     * Symmetric to \c headerPattern; evaluated at file-close time.
     *
     * \sa footerPattern(), setFooterPattern()
     */
    Q_PROPERTY(QString footerPattern READ footerPattern WRITE setFooterPattern)

public:
    explicit PatternHeaderFooterProvider(QObject *parent = nullptr);
    ~PatternHeaderFooterProvider() override;

    [[nodiscard]] QString headerPattern() const { return mHeaderPattern; }
    void setHeaderPattern(const QString &pattern);

    [[nodiscard]] QString footerPattern() const { return mFooterPattern; }
    void setFooterPattern(const QString &pattern);

    [[nodiscard]] QString header() const override;
    [[nodiscard]] QString footer() const override;

private:
    Q_DISABLE_COPY_MOVE(PatternHeaderFooterProvider)

    QString mHeaderPattern;
    QString mFooterPattern;
    std::unique_ptr<PatternFormatter> mHeaderFormatter;
    std::unique_ptr<PatternFormatter> mFooterFormatter;
};

} // namespace Log4Qt

#endif // LOG4QT_HEADERFOOTERPROVIDER_H
