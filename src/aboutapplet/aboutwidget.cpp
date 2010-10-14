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

#include "aboutwidget.h"

#include <QGraphicsLinearLayout>

#include <MImageWidget>
#include <MLabel>
#include <QGraphicsLinearLayout>
#include <MStylableWidget>

#undef DEBUG
#include "../debug.h"

AboutWidget::AboutWidget (
        AboutBusinessLogic     *aboutBusinessLogic,
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_AboutBusinessLogic (aboutBusinessLogic),
    m_Label1 (0)
{
    connect (aboutBusinessLogic, SIGNAL (ready ()),
             this, SLOT (dataReady ()));

    createContent ();
    aboutBusinessLogic->initiateDataCollection ();
}

AboutWidget::~AboutWidget ()
{
}


void
AboutWidget::createContent ()
{
    QGraphicsLinearLayout   *layout;
    MImageWidget            *logo;
    QGraphicsLinearLayout   *logoLayout;
    MStylableWidget         *stretcher;

    layout = new QGraphicsLinearLayout (Qt::Vertical);
    layout->setContentsMargins (0., 0., 0., 0.);

    /*
     * A stretcher.
     */
    stretcher = new MStylableWidget ();
    stretcher->setObjectName ("CommonSpacer");
    layout->addItem (stretcher);

    /*
     * The first row: a logo
     */
    logoLayout = new QGraphicsLinearLayout (Qt::Horizontal);
    logoLayout->setContentsMargins (0., 0., 0., 0.);

    /*
     * FIXME: How about other vendors? this should be some generic logo
     * or should come from some CSS ??
     */
    logo = new MImageWidget;
    logo->setImage ("icon-l-about-nokia-logo");
    logo->setAspectRatioMode (Qt::KeepAspectRatio);
    logoLayout->addStretch ();
    layout->addItem (logoLayout);
    
    /*
     * A stretcher.
     */
    stretcher = new MStylableWidget ();
    stretcher->setObjectName ("CommonSpacer");
    layout->addItem (stretcher);

    /*
     * A label... FIXME: This might be wrong, the layout guide seems to define
     * a stretcher inside the text, so we might want to create two separate
     * labels...
     */
    m_Label1 = new MLabel;
    m_Label1->setWordWrap (true);

    layout->addItem (m_Label1);
    layout->addStretch ();

    setLayout (layout);
}


QString 
AboutWidget::labelText()
{
    QString retval;

    retval += "<h3>" + m_AboutBusinessLogic->osName () + "</h3>";
    //% "Version"
    retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_version"));
    retval += m_AboutBusinessLogic->osVersion();
    //% "WLAN MAC address"
    retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_wlan_mac_address"));
    retval += m_AboutBusinessLogic->WiFiAddress ();
    //% "Bluetooth address"
    retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_bt_address"));
    retval += m_AboutBusinessLogic->BluetoothAddress ();
    //% "IMEI"
    retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_imei"));
    retval += m_AboutBusinessLogic->IMEI ();
    retval += "<hr />";
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
    m_Label1->setText (labelText ());
}

void 
AboutWidget::dataReady ()
{
    m_Label1->setText (labelText ());
}

