#include <QtGui>
#include <DcpWidget>
#include "offlineapplet.h"
#include "offlinebrief.h"

Q_EXPORT_PLUGIN2(offlineapplet, OfflineApplet)

void OfflineApplet::init()
{
};

DcpWidget* OfflineApplet::constructWidget(int widgetId)
{
    // No views for button-like applets by default
    Q_UNUSED(widgetId);
    return 0;
}

QString OfflineApplet::title() const
{
    return QString();
}

QVector<MAction*> OfflineApplet::viewMenuItems()
{
    QVector<MAction*> vector;
    return vector;
}

DcpBrief* OfflineApplet::constructBrief(int)
{
    return new OfflineBrief();
}

