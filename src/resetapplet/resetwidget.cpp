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

#include <MContainer>
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
    MLinearLayoutPolicy *policy;
    MButton             *restoreButton;
    MButton             *clearButton;
    /*
     *
     */
    layout = new MLayout;
    policy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    policy->setContentsMargins (0., 0., 0., 0.);
    policy->setSpacing (0.);

    /*
     *
     */
    //% "Restore original settings"
    restoreButton = new MButton (qtTrId("qtn_rset_restore"));
    restoreButton->setStyleName ("CommonTopButtonInverted");
    restoreButton->setObjectName ("ResetAppletRFSButton");
    connect (restoreButton, SIGNAL(clicked()), 
            this, SLOT(restoreActivated()));

    /*
     * The second button.
     */
    //% "Clear device"
    clearButton = new MButton (qtTrId("qtn_rset_clear"));
    clearButton->setStyleName ("CommonBottomButtonInverted");
    clearButton->setObjectName ("ResetAppletCUDButton");
    connect (clearButton, SIGNAL(clicked()), 
            this, SLOT(clearActivated()));

    addButtonContainer (policy, restoreButton, clearButton);

    /*
     *
     */
    layout->setPolicy (policy);
    setLayout (layout);
}

void
ResetWidget::addButtonContainer (
            MLinearLayoutPolicy *mainLayout,
            MButton             *button1,
            MButton             *button2)
{
    MContainer          *container;
    MLayout             *layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonLargePanelInverted");
    container->setHeaderVisible (false);

    layout = new MLayout;
    layout->setContentsMargins (0., 0., 0., 0.);
    
    landscapePolicy = new MLinearLayoutPolicy (layout, Qt::Horizontal);
    landscapePolicy->setContentsMargins (0., 0., 0., 0.);

    portraitPolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    portraitPolicy->setContentsMargins (0., 0., 0., 0.);

    layout->setLandscapePolicy (landscapePolicy);
    layout->setPortraitPolicy (portraitPolicy);

    container->centralWidget()->setLayout (layout);

    /*
     * Adding the buttons to the layouts
     */
    landscapePolicy->addStretch ();
    landscapePolicy->addItem (button1);
    landscapePolicy->addItem (button2);
    landscapePolicy->addStretch ();

    portraitPolicy->addItem (button1);
    portraitPolicy->setAlignment (button1, Qt::AlignHCenter);
    portraitPolicy->addItem (button2);
    portraitPolicy->setAlignment (button2, Qt::AlignHCenter);

    /*
     *
     */
    mainLayout->addItem (container);
    mainLayout->setStretchFactor (container, 0);
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

