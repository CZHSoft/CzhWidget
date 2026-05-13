#include "Presenters/MainPresenter.h"
#include "Views/MainWindow.h"
#include "Services/Log/LogService.h"

#include <QDebug>

namespace Presenters {

MainPresenter::MainPresenter(QObject* parent)
    : QObject(parent)
    , m_view(nullptr)
{
    Services::Log::LogService::instance()->info("Constructor created", "MainPresenter");
}

MainPresenter::~MainPresenter()
{
    Services::Log::LogService::instance()->info("Destructor called", "MainPresenter");
}

void MainPresenter::setView(Views::MainWindow* view)
{
    qDebug() << "MainPresenter::setView: Setting view";

    if (!view)
    {
        qWarning() << "MainPresenter::setView: Attempted to set null view!";
        return;
    }

    m_view = view;
    m_view->setObserver(this);

    qDebug() << "MainPresenter::setView: View set and observer configured";
}

void MainPresenter::initialize()
{
    qDebug() << "MainPresenter::initialize: Initialization completed";

    if (!m_view)
    {
        qWarning() << "MainPresenter::initialize: View is null!";
        return;
    }

    m_view->initWindow();
}

}