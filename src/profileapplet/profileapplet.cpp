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

#include "profileapplet.h"
#include "profilewidget.h"
#include "profiledatainterface.h"

#include "dcpprofile.h"
#include "dcpwidget.h"

#include <QtGui>
#include <QDebug>

#include <MTheme>
#include <MAction>

Q_EXPORT_PLUGIN2(profileapplet, ProfileApplet)

const QString cssDir = "/usr/share/themes/base/meegotouch/duicontrolpanel/style/";

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
    MTheme::loadCSS(cssDir + "profileapplet.css");
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
