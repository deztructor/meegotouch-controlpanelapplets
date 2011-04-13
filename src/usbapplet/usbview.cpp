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
#include "usbview.h"

#include <MLabel>
#include <MLayout>
#include <QGraphicsLinearLayout>
#include <MLinearLayoutPolicy>
#include <MComboBox>
#include <MBanner>
#include <MApplication>
#include <MImageWidget>
#include <MWidgetController>

#include <QTimer>

#define DEBUG
#define WARNING
#include "../debug.h"

#ifdef HAVE_QMSYSTEM
using namespace MeeGo;

static QmUSBMode::Mode usbModes[3] =
       { QmUSBMode::Ask,
         QmUSBMode::MassStorage,
         QmUSBMode::OviSuite };

inline int
usbModeIndex (QmUSBMode::Mode mode)
{
    int ret = 0;
    for (int i = 0; i < 3; i++)
        if (usbModes[i] == mode)
        {
            ret = i;
            break;
        }
    return ret;
}

UsbView::UsbView (MeeGo::QmUSBMode *logic) :
    DcpStylableWidget (0),
    m_logic (logic),
    m_policy (0),
    m_infoLabel (0)
{
    setObjectName ("UsbView");

    initWidget ();

    connect (m_logic, SIGNAL (modeChanged (MeeGo::QmUSBMode::Mode)),
             SLOT (updateInfoLabel ()));
}
#else
/*
 * FIXME: to implement a variant that does not use the QmSystem library.
 */
UsbView::UsbView (void *logic) :
    DcpStylableWidget (0),
    m_policy (0),
    m_infoLabel (0)
{
    setObjectName ("UsbView");
    initWidget ();
}
#endif

UsbView::~UsbView ()
{
}

void
UsbView::initWidget ()
{
    SYS_DEBUG ("");
    MLayout                 *mainLayout;

    m_infoOrder = 0;
    setStyleName ("CommonPanelInverted");

// Creating the main layout
    mainLayout = new MLayout;
    m_policy = new MLinearLayoutPolicy (mainLayout, Qt::Vertical);

    mainLayout->setContentsMargins (0,0,0,0);
    m_policy->setContentsMargins (0,0,0,0);
    m_policy->setSpacing (0);

#ifndef MEEGO
// Create the title-bar
    MLabel *title = addTitleLabel (
            this, m_policy, "CommonApplicationHeaderInverted");
    //% "USB"
    title->setText (qtTrId ("qtn_usb_title"));
    m_infoOrder++;
#endif

    MWidgetController *spacer = new MWidgetController;
    spacer->setStyleName ("CommonSpacer");
    m_policy->addItem (spacer);
    m_infoOrder++;

    m_UsbModeCombo = new MComboBox;
    //% "Default USB device mode"
    m_UsbModeCombo->setTitle (qtTrId ("qtn_usb_default_info"));
    m_UsbModeCombo->setStyleName ("CommonComboBoxInverted");
    m_UsbModeCombo->setObjectName ("UsbModeCombo");

    //% "Always ask"
    m_UsbModeCombo->insertItem (0, qtTrId ("qtn_usb_always_ask"));
    //% "Mass storage mode"
    m_UsbModeCombo->insertItem (1, qtTrId("qtn_usb_mass_storage"));
    //% "Ovi suite mode"
    m_UsbModeCombo->insertItem (2, qtTrId("qtn_usb_ovi_suite"));

#ifdef HAVE_QMSYSTEM
    m_UsbModeCombo->setCurrentIndex (usbModeIndex (m_logic->getDefaultMode ()));
#endif

    connect (m_UsbModeCombo, SIGNAL (activated (int)),
             SLOT (usbModeActivated (int)), Qt::QueuedConnection);

    m_policy->addItem (m_UsbModeCombo);
    m_policy->addStretch ();

    setLayout (mainLayout);

    updateInfoLabel ();
}

void
UsbView::usbModeActivated (int idx)
{
    SYS_DEBUG ("idx = %d", idx);
#ifdef HAVE_QMSYSTEM
    /*
     * Do nothing if we just tapped on the
     * currently selected one...
     */
    SYS_DEBUG ("check if this mode is already set...");
    if (m_logic->getDefaultMode() == usbModes[idx])
        return;

    SYS_DEBUG ("mode is not set already... check the current mode...");
    QmUSBMode::Mode active = m_logic->getMode ();
    SYS_DEBUG ("current mode known...")

    /*
     * If we are connected and some mode active, then
     * show an error message and set the mode back
     * to original
     */
    if ((active == QmUSBMode::MassStorage) ||
        (active == QmUSBMode::OviSuite))
    {
        SYS_DEBUG ("Currently USB is active, throwing a notification...");
        /*
         * Set checked on the previously active button
         */
        m_UsbModeCombo->setCurrentIndex (usbModeIndex (m_logic->getDefaultMode ()));

        SYS_DEBUG ("newidx = %d", m_UsbModeCombo->currentIndex ());
        SYS_DEBUG ("text = %s", SYS_STR (m_UsbModeCombo->currentText ()));

        MBanner *infoBanner = new MBanner;
        infoBanner->setIconID ("icon-m-common-usb");
        //% "Cannot change the USB mode when it is connected"
        infoBanner->setTitle (qtTrId ("qtn_usb_change_incorrect"));

        infoBanner->appear (MApplication::instance ()->activeWindow (),
                            MSceneWindow::DestroyWhenDone);
        SYS_DEBUG ("notification appear called...");
        return;
    }

    SYS_DEBUG ("setting the new default mode: start");
    QmUSBMode::Mode newmode = usbModes[idx];
    m_logic->setDefaultMode (newmode);
    SYS_DEBUG ("setting the new default mode: stop");

    /*
     * If we are connected, and we've changed the default
     * mode lets activate the selected mode...
     */
    SYS_DEBUG ("Check if active mode is charging only...");
    if (active == QmUSBMode::ChargingOnly)
        m_logic->setMode (newmode);

    SYS_DEBUG ("end");
#endif
}

MLabel *
UsbView::addTitleLabel (
        QGraphicsWidget     *parent,
        MLinearLayoutPolicy *targetPolicy,
        const char          *labelStyleName)
{
    Q_UNUSED (parent);
    MLabel                  *label;

    label = new MLabel;
    label->setStyleName (labelStyleName);

    targetPolicy->addItem (label);
    return label;
}

QString
UsbView::currentText () const
{
    SYS_DEBUG ("");
#ifdef HAVE_QMSYSTEM
    QmUSBMode::Mode active = m_logic->getMode ();

    switch (active)
    {
        case QmUSBMode::MassStorage:
            //% "%1 active"
            return qtTrId ("qtn_usb_active_mode").arg (
            //% "Mass storage mode"
                   qtTrId ("qtn_usb_mass_storage"));
            break;
        case QmUSBMode::OviSuite:
            //% "%1 active"
            return qtTrId ("qtn_usb_active_mode").arg (
            //% "Mass storage mode"
                   qtTrId ("qtn_usb_ovi_suite"));
            break;
        case QmUSBMode::ChargingOnly:
            //% "Current state: Charging only"
            return qtTrId ("qtn_usb_charging");
            break;
        default:
            return "";
            break;
    }
#endif
    return "";
}

void
UsbView::updateInfoLabel ()
{
    if (! m_policy)
        return;

    QString infoText = currentText ();

    if ((! infoText.isEmpty ()) && m_infoLabel)
    {
        /*
         * Label is already there, lets update the text
         */
        m_infoLabel->setText (infoText);
        return;
    }

    if (infoText.isEmpty ())
    {
        /*
         * Label needs to be removed if it is there...
         */
        if (! m_infoLabel)
            return;

        /*
         * Delete the infoWidget
         */
        delete m_policy->itemAt (m_infoOrder);
        m_infoLabel = 0;
        return;
    }

    SYS_DEBUG ("creating the info-widget");
    /*
     * Create the info label widget, and
     * initialize its contents...
     */
    MWidgetController *infoWidget = new MWidgetController;
    infoWidget->setStyleName ("CommonTextFrameInverted");
    infoWidget->setContentsMargins (0,0,0,0);

    QGraphicsLinearLayout *iwLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);
    iwLayout->setContentsMargins (0,0,0,0);
    iwLayout->setSpacing (0);

    MImageWidget *iwIcon = new MImageWidget ("icon-m-common-usb");
    iwIcon->setStyleName ("CommonMainIcon");
    iwLayout->addItem (iwIcon);

    m_infoLabel = new MLabel;
    m_infoLabel->setStyleName ("CommonSingleTitleInverted");
    m_infoLabel->setText (infoText);
    iwLayout->addItem (m_infoLabel);

    infoWidget->setLayout (iwLayout);

    /*
     * Insert it to the proper place...
     */
    m_policy->insertItem (m_infoOrder, infoWidget);
}

