#ifndef LOGSPAGE_H
#define LOGSPAGE_H

#include "ElaScrollPage.h"

class QTextEdit;
class QVBoxLayout;
class ElaText;

namespace Views {
namespace Pages {

class LogsPage : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit LogsPage(QWidget* parent = nullptr);
    ~LogsPage() override;

protected:
    virtual void changeEvent(QEvent* event) override;

private slots:
    void onLogReceived(const QString& message, int level);

private:
    void initUI();
    void retranslateUi();
    QColor getLevelColor(int level);
    QString getLevelName(int level);

    QTextEdit* m_logTextEdit;
    QVBoxLayout* m_mainLayout;
    ElaText* m_titleLabel;
};

}
}

#endif // LOGSPAGE_H