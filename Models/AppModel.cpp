#include "Models/AppModel.h"
#include "Services/Log/LogService.h"

#include <QDebug>

namespace Models {

AppModel::AppModel(QObject* parent)
    : QObject(parent)
    , m_isStayTop(false)
    , m_currentBodyIndex(0)
    , m_windowTitle("CzhWidget")
{
    Services::Log::LogService::instance()->info("Constructor created", "AppModel");
}

AppModel::~AppModel()
{
    Services::Log::LogService::instance()->info("Destructor called", "AppModel");
}

void AppModel::setIsStayTop(bool isStayTop)
{
    if (m_isStayTop != isStayTop)
    {
        m_isStayTop = isStayTop;
        emit isStayTopChanged(m_isStayTop);
        qDebug() << "AppModel: isStayTop changed to:" << m_isStayTop;
    }
}

void AppModel::setCurrentBodyIndex(int index)
{
    if (m_currentBodyIndex != index)
    {
        m_currentBodyIndex = index;
        emit currentBodyIndexChanged(m_currentBodyIndex);
        qDebug() << "AppModel: currentBodyIndex changed to:" << m_currentBodyIndex;
    }
}

void AppModel::setWindowTitle(const QString& title)
{
    if (m_windowTitle != title)
    {
        m_windowTitle = title;
        emit windowTitleChanged(m_windowTitle);
        qDebug() << "AppModel: windowTitle changed to:" << m_windowTitle;
    }
}

}