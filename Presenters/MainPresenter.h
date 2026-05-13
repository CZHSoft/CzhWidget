#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include <QWidget>
#include "Views/MainWindow.h"

namespace Presenters {

class MainPresenter : public QObject, public Views::IMainWindowObserver
{
    Q_OBJECT

public:
    explicit MainPresenter(QObject* parent = nullptr);
    ~MainPresenter() override;

    void setView(Views::MainWindow* view);
    void initialize();

private:
    Views::MainWindow* m_view;
};

}

#endif // MAINPRESENTER_H