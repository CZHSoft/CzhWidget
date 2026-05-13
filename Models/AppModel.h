#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QString>

namespace Models {

class AppModel : public QObject
{
    Q_OBJECT

public:
    explicit AppModel(QObject* parent = nullptr);
    ~AppModel() override;

    bool getIsStayTop() const { return m_isStayTop; }
    void setIsStayTop(bool isStayTop);

    int getCurrentBodyIndex() const { return m_currentBodyIndex; }
    void setCurrentBodyIndex(int index);

    QString getWindowTitle() const { return m_windowTitle; }
    void setWindowTitle(const QString& title);

signals:
    void isStayTopChanged(bool isStayTop);
    void currentBodyIndexChanged(int index);
    void windowTitleChanged(const QString& title);

private:
    bool m_isStayTop;
    int m_currentBodyIndex;
    QString m_windowTitle;
};

}

#endif // APPMODEL_H