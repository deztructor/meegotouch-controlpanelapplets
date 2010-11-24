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
#include <MSeparator>
#include <QGraphicsLinearLayout>
#include <MSeparator>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <MLayout>

#undef DEBUG
#include "../debug.h"

AboutWidget::AboutWidget (
        AboutBusinessLogic     *aboutBusinessLogic,
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_AboutBusinessLogic (aboutBusinessLogic),
    m_MainLayout (0),
    m_TitleLabel (0),
    m_InfoLabel (0),
    m_LicenseLabel (0)
{
    createContent ();

    connect (m_AboutBusinessLogic, SIGNAL (refreshNeeded ()), 
            SLOT (refresh ()));

    retranslateUi ();
}

AboutWidget::~AboutWidget ()
{
}


void
AboutWidget::createContent ()
{
    MLayout     *layout;
   
    /*
     * Creating a layout that holds the rows of the internal widgets.
     */
    layout = new MLayout (this);
    m_MainLayout = new MLinearLayoutPolicy (layout, Qt::Vertical);
    m_MainLayout->setContentsMargins (0., 0., 0., 0.);
    m_MainLayout->setSpacing (0.);
    setLayout (layout);

    // Row 1: the title
    addHeaderContainer ();
    addStretcher ("CommonSmallSpacerInverted");
    // Row 3: the logo
    addLogoContainer ();
    addStretcher ("CommonSpacerInverted");
    // Row 5: the info label
    addInfoLabelContainer ();
    addStretcher ("CommonGroupDividerInverted");
    // Row 7: the license label
    addLicenseLabelContainer ();
    addStretcher ("CommonSmallSpacerInverted");
    
    //m_MainLayout->addStretch ();
}

void 
AboutWidget::addHeaderContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonXLargeHeaderPanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * The label that we use as title.
     */
    //% "About product"
    m_TitleLabel = new MLabel (qtTrId("qtn_prod_about_product"));
    m_TitleLabel->setStyleName ("CommonXLargeHeaderInverted");
    layout->addItem (m_TitleLabel);
    layout->setAlignment (m_TitleLabel, Qt::AlignLeft);
    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}

void 
AboutWidget::addLogoContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;
    MImageWidget *logo;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonPanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * The logo 
     */
    logo = new MImageWidget;
    logo->setImage ("icon-l-about-nokia-logo");
    logo->setObjectName ("AboutAppletLogoImage");
   
    layout->addItem (logo);
    layout->setAlignment (logo, Qt::AlignLeft);
    layout->addStretch ();

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}

void 
AboutWidget::addInfoLabelContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonLargePanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * The logo 
     */
    m_InfoLabel = new MLabel;
    m_InfoLabel->setObjectName ("AboutAppletInfoLabel");
    //m_InfoLabel->setStyleName ("CommonSubTitleInverted");
   
    layout->addItem (m_InfoLabel);

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}

void 
AboutWidget::addLicenseLabelContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonLargePanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * The label
     */
    m_LicenseLabel = new MLabel;
    m_LicenseLabel->setWordWrap (true);
    // this text is not translated!
    m_LicenseLabel->setText (licenseText ());
    //m_LicenseLabel->setStyleName ("CommonSubTitleInverted"); 
    m_LicenseLabel->setObjectName ("AboutAppletLicenseLabel");

    layout->addItem (m_LicenseLabel);

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}


void 
AboutWidget::addStretcher (
        const QString &styleName)
{
    MSeparator *stretcher;

    Q_ASSERT (m_MainLayout);

    stretcher = new MSeparator ();
    stretcher->setStyleName (styleName);
    m_MainLayout->addItem (stretcher);
}

QString
AboutWidget::labelText()
{
    QString retval;
    QString tmp;

    retval += "<h3>" + m_AboutBusinessLogic->osName () + "</h3>";
    //% "Version"
    retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_version"));
    retval += m_AboutBusinessLogic->osVersion();

    tmp = m_AboutBusinessLogic->WiFiAddress ();
    if (tmp.isEmpty () == false)
    {
        //% "WLAN MAC address"
        retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_wlan_mac_address"));
        retval += tmp;
    }

    tmp = m_AboutBusinessLogic->BluetoothAddress ();
    if (tmp.isEmpty () == false)
    {
        //% "Bluetooth address"
        retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_bt_address"));
        retval += tmp;
    }

    tmp = m_AboutBusinessLogic->IMEI ();
    if (tmp.isEmpty () == false)
    {
        //% "IMEI"
        retval += QString ("<h3>%1</h3>").arg (qtTrId ("qtn_prod_imei"));
        retval += tmp;
    }
//    retval += "<hr />";

    return retval;
}

QString
AboutWidget::licenseText()
{
    QString retval;

    // TODO: make this customizable [eg.: for meego.com]

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
    retval += "<p>MeeGo Source Code Requests<br>";
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
AboutWidget::refresh ()
{
    m_InfoLabel->setText (labelText ());
}

void
AboutWidget::retranslateUi ()
{
    //% "About product"
    if (m_TitleLabel)
        m_TitleLabel->setText (qtTrId("qtn_prod_about_product"));
    refresh ();
}

