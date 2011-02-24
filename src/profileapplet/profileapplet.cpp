/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "profileapplet.h"
#include "profilewidget.h"
#include "profiledatainterface.h"

#include "dcpprofile.h"
#include "dcpwidget.h"

#include <QtGui>
#include <QDebug>
#include <MAction>

#ifndef UNIT_TEST
#include <MLibrary>
M_LIBRARY
#endif

Q_EXPORT_PLUGIN2(profileapplet, ProfileApplet)

ProfileApplet::ProfileApplet() :
    m_Api (new ProfileDataInterface)
{
}

ProfileApplet::~ProfileApplet() 
{
    delete m_Api;
}


void 
ProfileApplet::init()
{
}

DcpWidget *
ProfileApplet::constructWidget (
        int widgetId)
{
    Q_UNUSED(widgetId);
    return pageMain();
}

DcpWidget* ProfileApplet::pageMain()
{
    if (m_MainWidget == NULL)
        m_MainWidget = new ProfileWidget(m_Api);

    return m_MainWidget;
}

QString ProfileApplet::title() const
{
    //% "Profiles"
    return qtTrId ("qtn_prof_profile");
}

QVector<MAction*> ProfileApplet::viewMenuItems()
{
    QVector<MAction*> vector;
    return vector;
}

DcpBrief *
ProfileApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return 0;
}

