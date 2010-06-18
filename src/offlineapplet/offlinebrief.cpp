#include <DcpWidgetTypes>
#include <qmdevicemode.h>
#include "offlinebrief.h"

#undef DEBUG
#include "../debug.h"
using namespace Maemo;

OfflineBrief::OfflineBrief():
    m_DevMode(new QmDeviceMode())
{
    connect(m_DevMode, SIGNAL(deviceModeChanged(Maemo::QmDeviceMode::DeviceMode)),
            this, SLOT(devModeChanged(Maemo::QmDeviceMode::DeviceMode)));
    m_LastMode = m_DevMode->getMode();
}

OfflineBrief::~OfflineBrief()
{
    delete m_DevMode;
}

void OfflineBrief::devModeChanged(Maemo::QmDeviceMode::DeviceMode mode)
{
    SYS_DEBUG("newmode %d", mode);
    m_LastMode = mode;
    emit valuesChanged();
}

QString OfflineBrief::valueText() const
{
    SYS_DEBUG("");
    return currentText();
}
QString OfflineBrief::currentText() const
{
    SYS_DEBUG("");
    switch (m_LastMode)
    {
        case QmDeviceMode::Normal:
            //% "Activate offline mode"
            return qtTrId("qtn_offl_activate");
        case QmDeviceMode::Flight:
            //% "Deactivate offline mode"
            return qtTrId("qtn_offl_deactivate");
        default:
            SYS_DEBUG("An error occured while getting the device mode");
    }
    return QString();
}

void OfflineBrief::setToggle (bool toggle)
{
    Q_UNUSED(toggle);
    Maemo::QmDeviceMode::DeviceMode newDevMode;

    SYS_DEBUG("");
    if (m_LastMode == QmDeviceMode::Flight)
    {
        newDevMode = QmDeviceMode::Normal;
    }
    else
    {
        newDevMode = QmDeviceMode::Flight;
    }
    m_DevMode->setMode(newDevMode);
}

int OfflineBrief::widgetTypeID() const
{
    SYS_DEBUG("");
    return DcpWidgetType::Button;
}

