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
#include <QVariant>
#include <QSettings>

#include <MImageWidget>
#include <MLabel>
#include <MSeparator>
#include <QGraphicsLinearLayout>
#include <MSeparator>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <MLayout>

#include "../styles.h"

#define DEBUG
#define WARNING
#include "../debug.h"

#ifndef LICENSE_PATH
/* Define an empty string if fallback license file is not defined */
#define LICENSE_PATH ""
#endif

static const QString configPath ("/usr/share/about-contents/");
static const QString configFile (configPath + "contents.ini");

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(AboutWidget)

class ContentWidget: public MStylableWidget
{
  public:
    ContentWidget (QGraphicsItem *parent = 0):
        MStylableWidget (parent),
        m_title (0), m_subTitle (0)
    {
        setContentsMargins (0,0,0,0);
    }
    void setTitle(QString const &title)
    {
        init ();
        m_title->setText(title);
    }
    void setSubtitle(QString const &subTitle)
    {
        init ();
        m_subTitle->setText(subTitle);
    }
    void init ()
    {
        if (m_title || m_subTitle)
            return;

        QGraphicsLinearLayout *layout =
            new QGraphicsLinearLayout (Qt::Vertical);
        layout->setSpacing (0);
        layout->setContentsMargins (0, 0, 0, 0);

        m_title = new MLabel;
        m_title->setStyleName ("CommonSubTitleTopInverted");

        m_subTitle = new MLabel;
        m_subTitle->setStyleName ("CommonTitleBottomInverted");

        layout->addItem(m_title);
        layout->addItem(m_subTitle);

        setStyleName("CommonPanelInverted");

        setLayout(layout);
    }
  private:
    MLabel *m_title;
    MLabel *m_subTitle;
};

AboutWidget::AboutWidget (
        AboutBusinessLogic     *aboutBusinessLogic,
        QGraphicsWidget        *parent) :
    DcpStylableWidget (parent),
    m_layout (0),
    m_AboutBusinessLogic (aboutBusinessLogic),
    m_TitleLabel (0),
    m_Version(0),
    m_ProductName(0),
    m_WiFi(0),
    m_Bt(0),
    m_IMEI(0),
    m_LicenseLabel (0)
{
    initialize ();
    createContent ();

    connect (m_AboutBusinessLogic, SIGNAL (refreshNeeded ()),
             SLOT (refresh ()));
}

AboutWidget::~AboutWidget ()
{
}

void
AboutWidget::initialize ()
{
    QSettings content (configFile, QSettings::IniFormat);

    m_swName =
        content.value ("swname",
          m_AboutBusinessLogic->productName()).toString ();
    m_prodName = content.value ("prodname", "").toString ();
    m_certsImage = content.value ("swtypeimage", "").toString ();
    m_licenseFile =
        content.value ("abouttext", QString (LICENSE_PATH)).toString ();

    SYS_DEBUG ("\nAbout applet configuration:\n"
               "Product name        : %s\n"
               "Software version    : %s\n"
               "License file        : %s\n"
               "Type approval img.  : %s\n",
               SYS_STR (m_swName), SYS_STR (m_prodName),
               SYS_STR (m_licenseFile), SYS_STR (m_certsImage));
}


void
AboutWidget::createContent ()
{
    /*
     * Creating a layout that holds the rows of the internal widgets.
     */
    m_layout = new QGraphicsLinearLayout (Qt::Vertical);
    m_layout->setContentsMargins (0., 0., 0., 0.);
    m_layout->setSpacing (0.);

    // Row 1: the title
    addHeaderContainer ();
    // Row 2: the logo
    addLogoContainer ();
    // Row 3: the product name
    addNamesContainer ();
    // Row 4: versions
    addVersionContainer ();
    // Row 5: WiFi
    addWiFiMACContainer ();
    // Row 6: KekFog
    addBtMACContainer ();
    // Row 7: IMEI
    addIMEIContainer ();

    if (! m_certsImage.isEmpty ())
    {
        addStretcher ("CommonSpacer");
        addCertsContainer ();
    }

    addStretcher ("CommonItemDivider");
    addStretcher ("CommonSpacer");
    // Row ?: the license label
    addLicenseLabelContainer ();

    /*
     * And set the layout...
     */
    setLayout (m_layout);

    retranslateUi ();
}

void
AboutWidget::addHeaderContainer ()
{
    MSeparator *stretcher;

    if (!m_layout)
        return;

    /*
     * The label that we use as title.
     */
    //% "About product"
    m_TitleLabel = new MLabel (qtTrId("qtn_prod_about_product"));
    m_TitleLabel->setStyleName (APP_TITLE_LABEL_STYLE_NAME);
    m_layout->addItem (m_TitleLabel);
    m_layout->setStretchFactor (m_TitleLabel, 0);
    m_layout->setAlignment (m_TitleLabel, Qt::AlignLeft);

    #ifdef APP_TITLE_USES_SPACER
    stretcher = new MSeparator ();
    stretcher->setStyleName (APP_TITLE_DIVIDER_STYLE_NAME);
    m_layout->addItem (stretcher);
    #endif
}

void
AboutWidget::addLogoContainer ()
{
    if (!m_layout)
        return;

    QGraphicsLinearLayout *layout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    /*
     * The logo
     */
    MImageWidget *logo;
    logo = new MImageWidget;
    logo->setImage ("icon-l-about-nokia-logo");
    logo->setObjectName ("AboutAppletLogoImage");

    layout->addItem (logo);
    layout->addStretch();
    m_layout->addItem(layout);
}

void
AboutWidget::addCertsContainer ()
{
    if (!m_layout)
        return;

    QGraphicsLinearLayout *layout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    /* Load the picture ... */
    if (m_certsImage.at (0) != '/')
        m_certsImage = configPath + m_certsImage;

    QImage img (m_certsImage);
    SYS_DEBUG ("Image path = %s", SYS_STR (m_certsImage));

    MImageWidget *certsPic = new MImageWidget (&img);
    certsPic->setObjectName ("AboutAppletCertificates");

    layout->addItem (certsPic);
    layout->addStretch();
    m_layout->addItem(layout);
}

void
AboutWidget::addNamesContainer ()
{
    if (!m_layout)
        return;

    m_ProductName = new ContentWidget;
    m_ProductName->setTitle (m_prodName);
    m_ProductName->setSubtitle (m_swName);
    m_layout->addItem (m_ProductName);
}

void
AboutWidget::addVersionContainer ()
{
    if (!m_layout)
        return;

    m_Version = new ContentWidget;
    m_Version->setTitle (qtTrId ("qtn_prod_version"));
    m_Version->setSubtitle (m_AboutBusinessLogic->osVersion ());
    m_layout->addItem (m_Version);
}

void
AboutWidget::addWiFiMACContainer ()
{
    if (!m_layout)
        return;

    m_WiFi = new ContentWidget;
    m_WiFi->setSubtitle(m_AboutBusinessLogic->WiFiAddress ());
    m_layout->addItem(m_WiFi);
}

void
AboutWidget::addBtMACContainer ()
{
    if (!m_layout)
        return;

    m_Bt = new ContentWidget;
    m_Bt->setSubtitle(m_AboutBusinessLogic->BluetoothAddress ());
    m_layout->addItem(m_Bt);
}

void
AboutWidget::addIMEIContainer ()
{
    if (!m_layout)
        return;

    m_IMEI = new ContentWidget;
    m_IMEI->setSubtitle(m_AboutBusinessLogic->IMEI ());
    m_layout->addItem(m_IMEI);
}

void
AboutWidget::addLicenseLabelContainer ()
{
    if (!m_layout)
        return;

    /*
     * The label
     */
    m_LicenseLabel = new MLabel;
    m_LicenseLabel->setWordWrap (true);
    // this text is not translated!
    m_LicenseLabel->setText (licenseText ());
    m_LicenseLabel->setStyleName ("CommonBodyTextInverted");
    m_LicenseLabel->setObjectName ("AboutAppletLicenseLabel");

    /*
     * Adding the whole row to the main container.
     */
    m_layout->addItem (m_LicenseLabel);
    m_layout->setStretchFactor (m_LicenseLabel, 0);
}


void
AboutWidget::addStretcher (
        const QString &styleName)
{
    if (!m_layout)
        return;

    MStylableWidget *stretcher;
    stretcher = new MStylableWidget;
    stretcher->setStyleName (styleName);
    m_layout->addItem (stretcher);
}

QString
AboutWidget::licenseText ()
{
    if (m_licenseFile.at (0) != '/')
        m_licenseFile = configPath + m_licenseFile;

    QFile licenseFile (m_licenseFile);
    if (!licenseFile.open (QIODevice::ReadOnly | QIODevice::Text))
    {
        SYS_WARNING ("Unable to open %s", SYS_STR (configPath + m_licenseFile));
        return "";
    }

    QTextStream inStream (&licenseFile);

    return inStream.readAll ();
}
void
AboutWidget::refresh ()
{
    if (m_Bt) {
        m_Bt->setSubtitle(m_AboutBusinessLogic->BluetoothAddress ());
    }
}

void
AboutWidget::retranslateUi ()
{
    //% "About product"
    if (m_TitleLabel)
        m_TitleLabel->setText (qtTrId("qtn_prod_about_product"));

    if (m_Version) {
        //% "Version"
        m_Version->setTitle (qtTrId("qtn_prod_version"));
    }

    if (m_WiFi) {
        //% "WLAN MAC address"
        m_WiFi->setTitle (qtTrId("qtn_prod_wlan_mac_address"));
    }

    if (m_Bt) {
        //% "Bluetooth address"
        m_Bt->setTitle (qtTrId("qtn_prod_bt_address"));
    }

    if (m_IMEI) {
        //% "IMEI"
        m_IMEI->setTitle (qtTrId("qtn_prod_imei"));
    }
}

