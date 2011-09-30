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
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
** Authors: David Kedves <dkedves@blumsoft.eu>
**          Laszlo Pere <lpere@blumsoft.eu>
**
****************************************************************************/

#include "alerttonebrowsersheet.h"
#include "alerttonebrowserstylable.h"

#include <QAction>
#include <MBasicSheetHeader>
#include <MPannableViewport>

AlertToneBrowserSheet::AlertToneBrowserSheet (
        AlertTone *alertTone) :
    MSheet (),
    m_Widget (0)
{
    setStyleName ("Inverted");    
    createHeaderWidget ();
    createCentralWidget (alertTone);
}


void
AlertToneBrowserSheet::createHeaderWidget ()
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader (this);
    QAction           *doneAction;
    QAction           *cancelAction;

    basicHeader->setStyleName ("Inverted");
    //basicHeader->setProperty ("opacity", 0.2);
    doneAction = new QAction(qtTrId("qtn_comm_command_done"), basicHeader);
    cancelAction = new QAction(qtTrId("qtn_comm_cancel"), basicHeader);
    
    connect(doneAction, SIGNAL(triggered()), SLOT(doneActivated()));
    connect (cancelAction, SIGNAL(triggered()), SLOT(cancelActivated()));

    basicHeader->setPositiveAction (doneAction);
    basicHeader->setNegativeAction (cancelAction);

    setHeaderWidget(basicHeader);
}

void 
AlertToneBrowserSheet::createCentralWidget (
        AlertTone *alertTone)
{
    MPannableViewport *pannableViewport = new MPannableViewport;
    pannableViewport->setObjectName("pannableViewport");

    Q_ASSERT (!m_Widget);
    m_Widget = new AlertToneBrowserStylable (alertTone);
    pannableViewport->setWidget(m_Widget);

    setCentralWidget (pannableViewport);
}

void
AlertToneBrowserSheet::doneActivated ()
{
    m_Widget->accept ();
    dismiss ();
}

void
AlertToneBrowserSheet::cancelActivated ()
{
    m_Widget->cancel ();
    dismiss ();
}


