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

#include "resetwidget.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MDialog>
#include <MMessageBox>

#undef DEBUG
#include "../debug.h"

ResetWidget::ResetWidget (
        ResetBusinessLogic     *resetBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_ResetBusinessLogic (resetBusinessLogic),
    m_currentPlan (None)
{
    createContent();

    connect (resetBusinessLogic, SIGNAL (gotAccess ()),
             this, SLOT (doTheWork ()));
}

ResetWidget::~ResetWidget ()
{
}


void
ResetWidget::createContent ()
{
    MLayout             *layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    MButton             *restoreButton;
    MButton             *clearButton;

    layout = new MLayout;

    landscapePolicy = new MLinearLayoutPolicy (layout, Qt::Horizontal);
    portraitPolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    /*
     *
     */
    //% "Restore original settings"
    restoreButton = new MButton (qtTrId("qtn_rset_restore"));
    connect (restoreButton, SIGNAL(clicked()), 
            this, SLOT(restoreActivated()));

    /*
     *
     */
    //% "Clear device"
    clearButton = new MButton (qtTrId("qtn_rset_clear"));
    connect (clearButton, SIGNAL(clicked()), 
            this, SLOT(clearActivated()));
    
    landscapePolicy->addItem (restoreButton);
    landscapePolicy->addItem (clearButton);
    landscapePolicy->setStretchFactor (restoreButton, 2);
    landscapePolicy->setStretchFactor (clearButton, 2);

    portraitPolicy->addItem (restoreButton);
    portraitPolicy->addItem (clearButton);

    layout->setLandscapePolicy (landscapePolicy);
    layout->setPortraitPolicy (portraitPolicy);

    setLayout (layout);
}

void
ResetWidget::restoreActivated ()
{
    MDialog   *dialog;
    int        retval;

    //% "Restore original settings? The device will reboot, temporarily "
    //% "disabling all functions, including emergency calls. "
    //% "User created-content will be unaffected."
    QString    question = QString ("<p align=\"left\">") + 
                          qtTrId("qtn_rset_restore_query") + "</p>";
    // It is a bit ugly, but translations contains \n stuffs:
    question.replace ("\\n", "<br>");
    question.replace ("\n", "<br>");

    SYS_DEBUG ("");
    dialog = new MMessageBox ("", question, M::YesButton | M::NoButton);
    dialog->setTitleBarVisible (false);
    
    retval = dialog->exec();
    switch (retval) {
        case M::YesButton:
            SYS_DEBUG ("YES");
            m_currentPlan = ResetSettings;
            m_ResetBusinessLogic->getAccess ();
            break;

        case M::NoButton:
            SYS_DEBUG ("NO");
            /*
             * We do not have to do anything.
             */
            break;
    }
}

void
ResetWidget::clearActivated ()
{
    MDialog   *dialog;
    int        retval;

    //% "Clear all user data and restore original settings? "
    //% "The device will reboot, temporarily disabling all "
    //% "functions, including emergency calls."
    QString    question = QString ("<p align=\"left\">") + 
                          qtTrId("qtn_rset_clear_query") + "</p>";
    question.replace ("\\n", "<br>");
    question.replace ("\n", "<br>");

    SYS_DEBUG ("");
    dialog = new MMessageBox ("", question, M::YesButton | M::NoButton);
    dialog->setTitleBarVisible (false);
    
    retval = dialog->exec();
    switch (retval) {
        case M::YesButton:
            SYS_DEBUG ("YES");
            m_currentPlan = ClearData;
            m_ResetBusinessLogic->getAccess ();
            break;

        case M::NoButton:
            SYS_DEBUG ("NO");
            /*
             * We do not have to do anything.
             */
            break;
    }
}

void
ResetWidget::doTheWork ()
{
    switch (m_currentPlan)
    {
        case ResetSettings:
            m_ResetBusinessLogic->performRestoreSettings();
            break;
        case ClearData:
            m_ResetBusinessLogic->performClearData();
            break;
        default:
            SYS_WARNING ("Got access, but no plan ?!");
            break;
    }
    m_currentPlan = None;
}

