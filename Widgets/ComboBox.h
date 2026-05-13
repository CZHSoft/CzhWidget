#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <QString>
#include "ElaDef.h"

namespace Widgets {

class ComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(qreal expandIconRotate READ getExpandIconRotate WRITE setExpandIconRotate NOTIFY expandIconRotateChanged)
    Q_PROPERTY(qreal expandMarkWidth READ getExpandMarkWidth WRITE setExpandMarkWidth NOTIFY expandMarkWidthChanged)

public:
    explicit ComboBox(QWidget* parent = nullptr);
    ~ComboBox() override;

    qreal getExpandIconRotate() const { return _expandIconRotate; }
    void setExpandIconRotate(qreal rotate) { _expandIconRotate = rotate; update(); }

    qreal getExpandMarkWidth() const { return _expandMarkWidth; }
    void setExpandMarkWidth(qreal width) { _expandMarkWidth = width; update(); }

    void setPlaceholderText(const QString& text);
    QString placeholderText() const;

signals:
    void expandIconRotateChanged();
    void expandMarkWidthChanged();

protected:
    void showEvent(QShowEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void showPopup() override;
    void hidePopup() override;
    bool event(QEvent* event) override;

private slots:
    void onThemeModeChanged(ElaThemeType::ThemeMode mode);

private:
    void updateThemeColors();
    void applyThemeColors();

    ElaThemeType::ThemeMode _themeMode;
    QString _placeholderText;
    bool _isInitialized;
    qreal _expandIconRotate;
    qreal _expandMarkWidth;
};

} // namespace Widgets

#endif // COMBOBOX_H
