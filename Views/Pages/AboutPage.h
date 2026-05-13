#ifndef ABOUTPAGE_H
#define ABOUTPAGE_H

#include "ElaScrollPage.h"
#include "ElaText.h"

#include <QVBoxLayout>


class ElaImageCard;
class ElaFlowLayout;

namespace Views {
namespace Pages {

class AboutPage : public ElaScrollPage
{
    Q_OBJECT

public:
    explicit AboutPage(QWidget* parent = nullptr);
    ~AboutPage() override;

protected:
    virtual void changeEvent(QEvent* event) override;
    virtual bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
    void retranslateUi();

private:
    void initUI();
    void initProductSection(QVBoxLayout* parentLayout);
    void initAuthorSection(QVBoxLayout* parentLayout);
    void initAcknowledgementSection(QVBoxLayout* parentLayout);
    QLabel* createLinkLabel(const QString& text, const QString& url);
    void addWorkCard(ElaFlowLayout* parentLayout, const QString& imagePath, const QString& title, const QString& url);

private:
    ElaText* _titleLabel;
    ElaText* _appNameLabel;
    ElaText* _versionLabel;
    ElaText* _descriptionLabel;
    ElaText* _authorNameLabel;
    ElaText* _sectionTitle;
    ElaText* _worksTitle;
    ElaText* _acknowledgementTitle;
    QLabel* _personalLinkLabel;
    QLabel* _githubLinkLabel;
    QLabel* _qtLinkLabel;
    QLabel* _elaWidgetToolsLinkLabel;
    QLabel* _log4QtLinkLabel;
    QLabel* _treaLinkLabel;
    QLabel* _specialThanksLabel;
    QMap<ElaImageCard*, QString> _workCards;
};

}
}

#endif // ABOUTPAGE_H
