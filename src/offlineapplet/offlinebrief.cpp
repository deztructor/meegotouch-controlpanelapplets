#include <DcpWidgetTypes>
#include <qmdevicemode.h>
#include "offlinebrief.h"
#include <MBanner>
#include <MMessageBox>
#include <MLabel>

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
        case QmDeviceMode::Flight:
            //% "Deactivate offline mode"
            return qtTrId("qtn_offl_deactivate");
        case QmDeviceMode::Normal:
        default:
            //% "Activate offline mode"
            return qtTrId("qtn_offl_activate");
    }
    return QString();
}

void OfflineBrief::setToggle (bool toggle)
{
    Q_UNUSED(toggle);

    SYS_DEBUG("");
    if (m_LastMode == QmDeviceMode::Flight)
    {
        //% "Exit offline mode?"
        MMessageBox* dialog = new MMessageBox("", qtTrId("qtn_offl_exiting"),
            M::YesButton | M::NoButton);
        connect(dialog, SIGNAL(disappeared()), this, SLOT(processDialogResult()));
        dialog->appear();
    }
    else
    {
        if (m_DevMode->setMode(QmDeviceMode::Flight))
        {
            MBanner *infoBanner = new MBanner();
            //% "Closing all connections. Switching to offline mode."
            infoBanner->setSubtitle(qtTrId("qtn_offl_entering"));
            infoBanner->appear(MSceneWindow::DestroyWhenDone);
        }
    }
}

void OfflineBrief::processDialogResult()
{
    MMessageBox *dialog = static_cast<MMessageBox*>(sender());
    if(dialog->result() == MDialog::Accepted)
    {
        m_DevMode->setMode(QmDeviceMode::Normal);
    }
}

int OfflineBrief::widgetTypeID() const
{
    SYS_DEBUG("");
    return DcpWidgetType::Button;
}

