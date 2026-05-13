#ifndef GROUPBOX_H
#define GROUPBOX_H

#include <QWidget>
#include <QString>
#include <QPaintEvent>
#include <QStyleOption>
#include "ElaDef.h"

namespace Widgets {

class GroupBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle)

public:
    explicit GroupBox(QWidget* parent = nullptr);
    explicit GroupBox(const QString& title, QWidget* parent = nullptr);
    ~GroupBox() override;

    QString getTitle() const;
    void setTitle(const QString& title);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    bool event(QEvent* event) override;

private slots:
    void onThemeModeChanged(ElaThemeType::ThemeMode mode) {
        _themeMode = mode;
        update();
    }

private:
    void init();
    void drawTitle(QPainter* painter, const QRect& rect);

    QString _title;
    ElaThemeType::ThemeMode _themeMode;
    QRect _titleRect;
};

}

#endif
