#include "LanguageManager.h"
#include <QApplication>
#include <QLocale>
#include <QEvent>

namespace Services {
namespace I18n {

LanguageManager* LanguageManager::m_instance = nullptr;

LanguageManager* LanguageManager::instance()
{
    if (!m_instance) {
        m_instance = new LanguageManager();
    }
    return m_instance;
}

LanguageManager::LanguageManager(QObject* parent)
    : QObject(parent)
    , m_translator(nullptr)
{
}

LanguageManager::~LanguageManager()
{
    if (m_translator) {
        qApp->removeTranslator(m_translator);
        delete m_translator;
    }
}

void LanguageManager::init()
{
    m_translator = new QTranslator(this);
    QString systemLocale = QLocale::system().name();
    switchLanguage(systemLocale);
}

void LanguageManager::switchLanguage(const QString& locale)
{
    if (m_translator) {
        qApp->removeTranslator(m_translator);
    }

    QString qmFile = QString(":/i18n/CzhWidget_%1").arg(locale);

    if (m_translator->load(qmFile)) {
        qApp->installTranslator(m_translator);
        m_currentLocale = locale;
        sendLanguageChangeEvent();
        emit languageChanged(locale);
    } else {
        if (m_translator->load(":/i18n/CzhWidget_en")) {
            qApp->installTranslator(m_translator);
            m_currentLocale = "en";
            sendLanguageChangeEvent();
            emit languageChanged("en");
        }
    }
}

void LanguageManager::sendLanguageChangeEvent()
{
    QEvent event(QEvent::LanguageChange);
    QApplication::sendEvent(qApp, &event);
}

QString LanguageManager::currentLanguage() const
{
    return m_currentLocale;
}

QStringList LanguageManager::availableLanguages() const
{
    return {"en", "zh_CN", "ko_KR", "ja_JP"};
}

} // namespace I18n
} // namespace Services
