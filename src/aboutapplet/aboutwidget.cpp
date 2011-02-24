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

#include "aboutwidget.h"

#include <QGraphicsLinearLayout>
#include <QFile>
#include <QTextStream>

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

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(AboutWidget)

static const char *LicensePath = LICENSE_PATH;

AboutWidget::AboutWidget (
        AboutBusinessLogic     *aboutBusinessLogic,
        QGraphicsWidget        *parent) :
    DcpStylableWidget (parent),
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

    retval += "<h3>" + m_AboutBusinessLogic->productName () + "</h3>";
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

    QFile licenseFile(LicensePath);
    if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Unable to open: " << LicensePath;
        return QString();
    }

    QTextStream inStream(&licenseFile);

    retval = inStream.readAll();

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

