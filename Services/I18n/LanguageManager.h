#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QString>
class QWidget;

namespace Services {
namespace I18n {

class LanguageManager : public QObject
{
    Q_OBJECT

public:
    static LanguageManager* instance();

    void init();
    void switchLanguage(const QString& locale);
    QString currentLanguage() const;
    QStringList availableLanguages() const;

signals:
    void languageChanged(const QString& newLocale);

private:
    explicit LanguageManager(QObject* parent = nullptr);
    ~LanguageManager();

    void sendLanguageChangeEvent();

    static LanguageManager* m_instance;
    QTranslator* m_translator;
    QString m_currentLocale;
};

} // namespace I18n
} // namespace Services

#endif // LANGUAGEMANAGER_H