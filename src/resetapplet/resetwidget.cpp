/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "resetwidget.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MDialog>
#include <MMessageBox>

#define DEBUG
#include "../debug.h"

ResetWidget::ResetWidget (
        ResetBusinessLogic     *resetBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_ResetBusinessLogic (resetBusinessLogic)
{
    createContent();
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

    //% "<p align=\"left\">Restore original settings?</p>"
    //% "<p align=\"left\">The device will reboot, temporarily disabling all "
    //% "functions, including emergency calls. User created-content "
    //% "will be unaffected.</p>"
    QString    question = qtTrId("qtn_rset_restore_query");

    SYS_DEBUG ("");
    dialog = new MMessageBox ("", question, M::YesButton | M::NoButton);
    
    retval = dialog->exec();
    switch (retval) {
        case M::YesButton:
            SYS_DEBUG ("YES");
            m_ResetBusinessLogic->performRestoreSettings();
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

    //% "<p align=\"left\">Clear all user data?</p>"
    //% "<p align=\"left\">The device will reboot, temporarily disabling all "
    //% "functions, including emergency calls.</p>"
    QString    question = qtTrId("qtn_rset_clear_query");

    SYS_DEBUG ("");
    dialog = new MMessageBox ("", question, M::YesButton | M::NoButton);
    
    retval = dialog->exec();
    switch (retval) {
        case M::YesButton:
            SYS_DEBUG ("YES");
            m_ResetBusinessLogic->performClearData();
            break;

        case M::NoButton:
            SYS_DEBUG ("NO");
            /*
             * We do not have to do anything.
             */
            break;
    }
}

