/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryapplet.h"
#include "batterywidget.h"
#include "batterybrief.h"

#include "dcpbattery.h"
#include "batterywidget.h"

#include <dcpwidget.h>

#include <QtGui>
#include <QDebug>
#include <QDBusInterface>

#include <MTheme>
#include <MAction>

#undef DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(batteryapplet, BatteryApplet)

const QString cssDir = "/usr/share/themes/base/meegotouch/duicontrolpanel/style/";

BatteryApplet::BatteryApplet ():
	m_MainWidget (0)
{
}

BatteryApplet::~BatteryApplet ()
{
    SYS_DEBUG ("Destroying %p", this);
}

void BatteryApplet::init()
{
    MTheme::loadCSS(cssDir + "batteryapplet.css");
}

DcpWidget *
BatteryApplet::constructWidget (
		int widgetId)
{
    Q_UNUSED (widgetId);
    return pageMain();
}

DcpWidget* 
BatteryApplet::pageMain ()
{
    SYS_DEBUG ("Starting on %p", this);
    /*
     * Please note that the m_MainWidget is a QPointer that will nullify itself
     * when the widget is destroyed. Then we need to create a new one when we
     * asked for it.
     */
    if (m_MainWidget == NULL)
        m_MainWidget = new BatteryWidget();

    return m_MainWidget;
}

QString BatteryApplet::title() const
{
    //% "Battery"
    return qtTrId ("qtn_ener_battery");
}

QVector<MAction*> BatteryApplet::viewMenuItems()
{
    QVector<MAction*> vector;
    //% "User Guide"
    MAction* helpAction = new MAction(qtTrId ("qtn_comm_userguide"), pageMain());
    vector.append(helpAction);
    helpAction->setLocation(MAction::ApplicationMenuLocation);

    connect (helpAction, SIGNAL (triggered (bool)),
             this, SLOT (userGuide ()));

    return vector;
}

void
BatteryApplet::userGuide ()
{
    QDBusInterface userguide ("com.nokia.userguide", "/",
                              "com.nokia.UserGuideIf");
    userguide.call ("pageByPath", "fullguide-1-1-list-2.cfg");
    SYS_DEBUG ("");
}

DcpBrief* BatteryApplet::constructBrief(int partId)
{
    Q_UNUSED(partId);
    return new BatteryBrief();
}

