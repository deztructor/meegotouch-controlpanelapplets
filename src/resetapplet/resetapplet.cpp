/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "resetapplet.h"
#include "resetwidget.h"
#include "resetbrief.h"

#include <MTheme>
#include <MAction>
#include <QDBusInterface>

#undef DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(resetapplet, ResetApplet)

ResetApplet::ResetApplet() :
    m_ResetBusinessLogic (new ResetBusinessLogic)
{
}

ResetApplet::~ResetApplet() 
{
    delete m_ResetBusinessLogic;
}

void 
ResetApplet::init()
{
}

DcpWidget *
ResetApplet::pageMain(
        int widgetId)
{
    SYS_DEBUG ("widgetId = %d", widgetId);
    switch (widgetId) {
        case 0:
            if (m_MainWidget == 0) 
                m_MainWidget = new ResetWidget (m_ResetBusinessLogic);
            return m_MainWidget;

        default:
            SYS_WARNING ("Unknown widgetId: %d", widgetId);
    }

    return 0;
}

DcpWidget *
ResetApplet::constructWidget (
        int widgetId)
{
    SYS_DEBUG ("-----------------------------------");
    SYS_DEBUG ("*** widgetId = %d", widgetId);
    return pageMain (widgetId);
}

QString
ResetApplet::title() const
{
    //% "Reset settings"
    return qtTrId ("qtn_rset_reset_settings");
}

QVector<MAction*>
ResetApplet::viewMenuItems()
{
    MAction            *helpAction;
    QVector<MAction*>   vector;

    SYS_DEBUG ("");
    helpAction = new MAction (
            //% "User Guide"
            qtTrId ("qtn_comm_userguide"), 
            pageMain (0));
    helpAction->setLocation (MAction::ApplicationMenuLocation);

    connect (helpAction, SIGNAL (triggered (bool)),
             this, SLOT (userGuide ()));

    vector.append(helpAction);

    return vector;
}

void
ResetApplet::userGuide ()
{
    QDBusInterface userguide ("com.nokia.userguide", "/",
                              "com.nokia.UserGuideIf");
    userguide.call ("pageByPath", "tips.cfg");
    SYS_DEBUG ("");
}


DcpBrief *
ResetApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new ResetBrief (m_ResetBusinessLogic);
}
