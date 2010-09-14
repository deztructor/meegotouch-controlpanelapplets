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

#include "warrantywidget.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MDialog>
#include <MMessageBox>

#ifdef HAVE_QMSYSTEM
#  include <qmsystemstate.h>
#endif

#define DEBUG
#include "../debug.h"

WarrantyWidget::WarrantyWidget (
        WarrantyBusinessLogic  *warrantyBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WarrantyBusinessLogic (warrantyBusinessLogic)
{
    createContent();
}

WarrantyWidget::~WarrantyWidget ()
{
}


void
WarrantyWidget::createContent ()
{
    MLayout             *layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    MLabel              *label1;


    layout = new MLayout;

    /*
     * I only added two policies, so that later we can use a separate layout for
     * portrait/landscape orientation. FIXME: We might want to remove one of
     * these when the UI spec. is finalized.
     */
    landscapePolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    portraitPolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    /*
     * This will be used here, so currently it is just a test be we keep it
     * here.
     */
    label1 = new MLabel;
    label1->setWordWrap (true);

    #ifdef HAVE_QMSYSTEM
    Maemo::QmSystemState  systemState;
    label1->setText (labelText ().arg (systemState.getPowerOnTimeInSeconds ()));
    #else
    /*
     * FIXME: To implement a version that does not use the QmSystem.
     */
    label1->setText ("QmSystem is not available");
    #endif

    landscapePolicy->addItem (label1);
    portraitPolicy->addItem (label1);

    layout->setLandscapePolicy (landscapePolicy);
    layout->setPortraitPolicy (portraitPolicy);

    setLayout (layout);
}


QString 
WarrantyWidget::labelText()
{
    QString retval;

    retval += "<h2>The text of the warranty</h2>";
    retval += "<p>Here is going to be shown the text of the WARRANTY ";
    retval += "once the text is going to be specified.</p>";
    retval += "<br/><p>Power on time: %L1 seconds</p>";

    return retval;
}

