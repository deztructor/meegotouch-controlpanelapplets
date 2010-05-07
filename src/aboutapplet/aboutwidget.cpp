/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "aboutwidget.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MDialog>
#include <MMessageBox>

#define DEBUG
#include "../debug.h"

AboutWidget::AboutWidget (
        AboutBusinessLogic     *aboutBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_AboutBusinessLogic (aboutBusinessLogic),
    m_Label1 (0)
{
    connect (aboutBusinessLogic, SIGNAL(ready()),
            this, SLOT(dataReady()));

    createContent();
    aboutBusinessLogic->initiateDataCollection();
}

AboutWidget::~AboutWidget ()
{
}


void
AboutWidget::createContent ()
{
    MLayout             *layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;

    layout = new MLayout;

    /*
     * I only added two policies, so that later we can use a separate layout for
     * portrait/landscape orientation. FIXME: We might want to remove one of
     * these when the UI spec. is finalized.
     */
    landscapePolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    portraitPolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    /*
     *
     */
    m_Label1 = new MLabel (labelText());
    
    landscapePolicy->addItem (m_Label1);
    portraitPolicy->addItem (m_Label1);

    layout->setLandscapePolicy (landscapePolicy);
    layout->setPortraitPolicy (portraitPolicy);

    setLayout (layout);
}


QString 
AboutWidget::labelText()
{
    QString retval;

    retval += "<h2>Name of the product</h2>";
    retval += "<h2>" + m_AboutBusinessLogic->osName () + "</h2>";
    retval += "<h2>Version</h2>";
    retval += m_AboutBusinessLogic->osVersion();
    retval += "<h2>Wi-Fi MAC address:</h2>";
    retval += m_AboutBusinessLogic->WiFiAddress ();
    retval += "<h2>Bluetooth address:</h2>";
    retval += m_AboutBusinessLogic->BluetoothAddress ();
    retval += "<h2>IMEI:</h2>";
    retval += m_AboutBusinessLogic->IMEI ();
    retval += "<p>This product includes certain free/open source software</p>";
    retval += "<p>The exact terms of the licenses, disclaimers, "
              "aknowledgements and notices are provided in the "
              "following document/through the following links:"
              "<a href=\"http://somethink.here\">[insert document/link]</a>. "
              "You may obtain the source code of the relevant free and open "
              "source software at "
              "<a href=\"http://somethink.here\">[insert the URL]</a>. "
              "Alternatively, Nokia offers to provide such source code to you "
              "on a CD-ROM upon written request to Nokia at:</p>";
    retval += "<p>Maemo Source Code Requests<br>";
    retval += "Nokia Corporation<br>";
    retval += "P.O.Box 407<br>";
    retval += "FI-00045 Nokia Group<br>";
    retval += "Finland<br>";
    retval += "<p>This offer is valid for a period of three (3) years "
              "from the date of the distribution of t his product by "
              "Nokia.</p>";
    retval += "<p>The Graphics Interchange Format (c) is the Copyright "
              "property of CompuServe Incorporated. GIF(sm) is a "
              "Service Mark property of Compuserve Incorporated.</p>";
    retval += "<p>AdobeAE FlashAE Player. Copyright (c) 1996 - 2007 "
              "Adobe Systems Incorporated. All Rights Reserved. "
              "Protected by U.S. Patent 6,879,327; Patents Pending in the "
              "United States and other countries. Adobe and Flas are either "
              "trademarks or registered trademarks in the United States "
              "and/or other countries.</p>";



    return retval;
}

void
AboutWidget::retranslateUi ()
{
    m_Label1->setText (labelText());
}

void 
AboutWidget::dataReady ()
{
    m_Label1->setText (labelText());
}

