#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include "ElaScrollPage.h"
#include "Widgets/FunctionNavCard.h"

class ElaImageCard;
class ElaFlowLayout;

namespace Views {
namespace Pages {

class DashboardPage : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget* parent = nullptr);
    ~DashboardPage() override;

Q_SIGNALS:
    void navigateToPage(const QString& pageName);

protected:
    virtual void changeEvent(QEvent* event) override;

public slots:
    void retranslateUi();

private:
    void initUI();
    void initWelcomeSection();
    QWidget* initNavigationCards();

private:
    ElaImageCard* _backgroundCard;
    QList<QString> _navigationPageNames;
    ElaText* _welcomeText;
    ElaText* _descriptionText;
    ElaText* _sectionTitle;
    QList<Widgets::FunctionNavCard*> _navCards;
};

}
}

#endif // DASHBOARDPAGE_H