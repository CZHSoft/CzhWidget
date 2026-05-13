#ifndef FUNCTIONNAVCARD_H
#define FUNCTIONNAVCARD_H

#include <QWidget>
#include <QPixmap>
#include <QString>

namespace Widgets {

class ElaText;
class FunctionNavCard : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionNavCard(QWidget* parent = nullptr);
    ~FunctionNavCard() override;

    void setIcon(const QPixmap& pixmap);
    void setIconSize(const QSize& size);
    void setTitle(const QString& title);
    void setDescription(const QString& description);
    void setPageName(const QString& pageName);

Q_SIGNALS:
    void cardClicked(const QString& pageName);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    void updateStyle();

private:
    QString _pageName;
    QString _title;
    QString _description;
    QPixmap _iconPixmap;
    QSize _iconSize;
    bool _isHovered;
    bool _isPressed;
};

}

#endif
