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

class ContentWidget: public MStylableWidget
{
  public:
    ContentWidget(QGraphicsItem *parent = 0):
        MStylableWidget(parent)
    {
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout (Qt::Vertical);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);

        m_title = new MLabel;
        m_title->setStyleName ("CommonSubTitleTopInverted");

        m_subTitle = new MLabel;
        m_subTitle->setStyleName ("CommonTitleBottomInverted");

        layout->addItem(m_title);
        layout->addItem(m_subTitle);

        setStyleName("CommonPanelInverted");

        setLayout(layout);
    };
    void setTitle(QString const &title)
    {
        m_title->setText(title);
    };
    void setSubtitle(QString const &subTitle)
    {
        m_subTitle->setText(subTitle);
    };
  private:
    MLabel *m_title;
    MLabel *m_subTitle;
};

AboutWidget::AboutWidget (
        AboutBusinessLogic     *aboutBusinessLogic,
        QGraphicsWidget        *parent) :
    DcpStylableWidget (parent),
    m_AboutBusinessLogic (aboutBusinessLogic),
    m_TitleLabel (0),
    m_Version(0),
    m_ProductName(0),
    m_WiFi(0),
    m_Bt(0),
    m_IMEI(0),
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
    /*
     * Creating a layout that holds the rows of the internal widgets.
     */
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout (Qt::Vertical);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);
    setLayout (layout);

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
    addStretcher ("CommonItemDivider");
    addStretcher ("CommonSpacer");
    // Row 7: the license label
    addLicenseLabelContainer ();
}

void 
AboutWidget::addHeaderContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*>(layout());

    if (!mLayout) {
        return;
    }
    /*
     * The label that we use as title.
     */
    //% "About product"
    m_TitleLabel = new MLabel (qtTrId("qtn_prod_about_product"));
    m_TitleLabel->setStyleName ("CommonApplicationHeaderInverted");
    mLayout->addItem (m_TitleLabel);
    mLayout->setStretchFactor (m_TitleLabel, 0);
    mLayout->setAlignment (m_TitleLabel, Qt::AlignLeft);
}

void 
AboutWidget::addLogoContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*>(layout());

    if (!mLayout) {
        return;
    }
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);

    MImageWidget *logo;

    /*
     * The logo 
     */
    logo = new MImageWidget;
    logo->setImage ("icon-l-about-nokia-logo");
    logo->setObjectName ("AboutAppletLogoImage");

    layout->addItem (logo);
    layout->addStretch();
    mLayout->addItem(layout);
}

void
AboutWidget::addNamesContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*> (layout());

    if (!mLayout) {
        return;
    }

    m_ProductName = new ContentWidget;
    m_ProductName->setTitle(m_AboutBusinessLogic->productName());
    mLayout->addItem(m_ProductName);
}

void
AboutWidget::addVersionContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*> (layout());

    if (!mLayout) {
        return;
    }

    m_Version = new ContentWidget;
    m_Version->setSubtitle(m_AboutBusinessLogic->osVersion());
    mLayout->addItem(m_Version);
}

void
AboutWidget::addWiFiMACContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*> (layout());

    if (!mLayout) {
        return;
    }

    m_WiFi = new ContentWidget;
    m_WiFi->setSubtitle(m_AboutBusinessLogic->WiFiAddress ());
    mLayout->addItem(m_WiFi);
}

void
AboutWidget::addBtMACContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*> (layout());

    if (!mLayout) {
        return;
    }

    m_Bt = new ContentWidget;
    m_Bt->setSubtitle(m_AboutBusinessLogic->BluetoothAddress ());
    mLayout->addItem(m_Bt);
}

void
AboutWidget::addIMEIContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*> (layout());

    if (!mLayout) {
        return;
    }
    m_IMEI = new ContentWidget;
    m_IMEI->setSubtitle(m_AboutBusinessLogic->IMEI ());
    mLayout->addItem(m_IMEI);
}

void 
AboutWidget::addLicenseLabelContainer ()
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*> (layout());

    if (!mLayout) {
        return;
    }
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
    mLayout->addItem (m_LicenseLabel);
    mLayout->setStretchFactor (m_LicenseLabel, 0);
}


void 
AboutWidget::addStretcher (
        const QString &styleName)
{
    QGraphicsLinearLayout *mLayout = dynamic_cast<QGraphicsLinearLayout*> (layout());

    if (!mLayout) {
        return;
    }
    MStylableWidget *stretcher;

    stretcher = new MStylableWidget ();
    stretcher->setStyleName (styleName);
    mLayout->addItem (stretcher);
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
    if (m_ProductName) {
        m_ProductName->setSubtitle(m_AboutBusinessLogic->osName());
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

