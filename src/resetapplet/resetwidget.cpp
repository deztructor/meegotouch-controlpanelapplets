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
#include "resetwidget.h"

#include <MContainer>
#include <MLayout>
#include <MBanner>
#include <MApplication>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MDialog>
#include <MMessageBox>
#include <MSeparator>

#undef DEBUG
#include "../debug.h"

#define qtTrIdShort(id) qtTrId(id).split(QChar(0x9c)).last()

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
    MSeparator            *spacer;


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
    spacer = new MSeparator;
    // Using this one instead of "CommonSpacer", margins look even.
    spacer->setStyleName ("CommonLargeSpacer");
    policy->addItem (spacer);

    /*
     * The first button.
     */
    //% "Restore original settings"
    restoreButton = new MButton (qtTrId("qtn_rset_restore"));
    restoreButton->setStyleName ("CommonSingleButtonInverted");
    restoreButton->setObjectName ("ResetAppletRFSButton");
    connect (restoreButton, SIGNAL(clicked()), 
            this, SLOT(restoreActivated()));

    
    /*
     * The second button.
     */
    //% "Clear device"
    clearButton = new MButton (qtTrId("qtn_rset_clear"));
    clearButton->setStyleName ("CommonSingleButtonInverted");
    clearButton->setObjectName ("ResetAppletCUDButton");
    connect (clearButton, SIGNAL(clicked()), 
            this, SLOT(clearActivated()));

    addButtonContainer (policy, restoreButton, clearButton);
    policy->addStretch();
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
    MSeparator          *spacer;
    
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
     * One spacer
     */
    spacer = new MSeparator;
    spacer->setStyleName ("CommonSpacer");

    /*
     * Adding the buttons and the spacer to the layouts
     */
    landscapePolicy->addStretch ();
    landscapePolicy->addItem (button1);
    landscapePolicy->addItem (button2);
    landscapePolicy->addStretch ();

    portraitPolicy->addItem (button1);
    portraitPolicy->setAlignment (button1, Qt::AlignHCenter);
    portraitPolicy->addItem (spacer);
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

    if (m_ResetBusinessLogic->isUsbConnected ())
    {
        showMassStorageWarning ();
        return;
    }

    //% "Restore original settings?"
    QString question = qtTrId ("qtn_rset_restore_query");
    // It is a bit ugly, but translations contains \n stuffs:
    question.replace ("\\n", "<br>");
    question.replace ("\n", "<br>");

    //% "Restore original settings?"
    dialog = new MMessageBox (qtTrId ("qtn_rset_restore_query_title"),
                              question, M::YesButton | M::NoButton);
    connect (dialog, SIGNAL (accepted ()), SLOT (restoreConfirmed ()));

    dialog->appear (MApplication::instance ()->activeWindow (),
                    MSceneWindow::DestroyWhenDone);
}

void
ResetWidget::restoreConfirmed ()
{
    SYS_DEBUG ("user choosen yes");
    m_currentPlan = ResetSettings;
    m_ResetBusinessLogic->getAccess ();
}

void
ResetWidget::clearActivated ()
{
    MDialog   *dialog;

    if (m_ResetBusinessLogic->isUsbConnected ())
    {
        showMassStorageWarning ();
        return;
    }

    //% "Clear all user data and restore original settings?"
    QString    question = qtTrId("qtn_rset_clear_query");
    question.replace ("\\n", "<br>");
    question.replace ("\n", "<br>");

    //% "Clear all data?"
    dialog = new MMessageBox (qtTrId ("qtn_rset_clear_query_title"),
                              question, M::YesButton | M::NoButton);
    connect (dialog, SIGNAL (accepted ()), SLOT (clearConfirmed ()));

    dialog->appear (MApplication::instance ()->activeWindow (),
                    MSceneWindow::DestroyWhenDone);
}

void
ResetWidget::clearConfirmed ()
{
    SYS_DEBUG ("user choosen yes");
    m_currentPlan = ClearData;
    m_ResetBusinessLogic->getAccess ();
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

void
ResetWidget::showMassStorageWarning ()
{
    SYS_DEBUG ("");
    MBanner *infoBanner = new MBanner;
    infoBanner->setStyleName ("InformationBanner");
    infoBanner->setObjectName ("InfoBanner");

    //% "Device resets are not possible while USB is connected in mass storage mode."
    infoBanner->setTitle (qtTrId ("qtn_rset_not_possible"));

    infoBanner->appear (MApplication::instance ()->activeWindow (),
                        MSceneWindow::DestroyWhenDone);
}

