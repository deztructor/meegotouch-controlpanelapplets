/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

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
    #ifdef HAVE_QMSYSTEM
    Maemo::QmSystemState  systemState;
    label1 = new MLabel (labelText().arg(
                systemState.getPowerOnTimeInSeconds ()));
    #else
    /*
     * FIXME: To implement a version that does not use the QmSystem.
     */
    label1 = new MLabel ("QmSystem is not available");
    #endif

    label1->setWordWrap (true);
    
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

