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
#include <MLinearLayoutPolicy>
#include <MComboBox>
#include <MNotification>

#undef DEBUG
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
    m_logic (logic),
    m_error (0)
{
    setObjectName ("UsbView");
    initWidget ();
}
#else
/*
 * FIXME: to implement a variant that does not use the QmSystem library.
 */
UsbView::UsbView (void *logic) :
    m_error (0)
{
    setObjectName ("UsbView");
    initWidget ();
}
#endif

UsbView::~UsbView ()
{
    if (m_error != 0)
    {
        m_error->remove ();
        delete m_error;
        m_error = 0;
    }
}

void
UsbView::initWidget ()
{
    MLayout                 *mainLayout;
    MLinearLayoutPolicy     *mainPolicy;

    setObjectName ("CommonPanelInverted");

// Creating the main layout
    mainLayout = new MLayout;
    mainPolicy = new MLinearLayoutPolicy (mainLayout, Qt::Vertical);

    mainLayout->setContentsMargins (0. , 0. , 0. , 0.);
    mainPolicy->setSpacing (0. );

#ifndef MEEGO
// Create the title-bar
    MLabel *title = addTitleLabel (
            this, mainPolicy, "CommonApplicationHeaderInverted");
    //% "USB"
    title->setText (qtTrId ("qtn_usb_title"));
#endif

    m_UsbModeCombo = new MComboBox;
    m_UsbModeCombo->setTitle (qtTrId ("qtn_usb_default_info"));
    m_UsbModeCombo->setStyleName ("CommonComboBoxInverted");
    m_UsbModeCombo->setObjectName ("UsbModeCombo");

    m_UsbModeCombo->insertItem (0, qtTrId ("qtn_usb_always_ask"));
    m_UsbModeCombo->insertItem (1, qtTrId("qtn_usb_mass_storage"));
    m_UsbModeCombo->insertItem (2, qtTrId("qtn_usb_ovi_suite"));

#ifdef HAVE_QMSYSTEM
    m_UsbModeCombo->setCurrentIndex (usbModeIndex (m_logic->getDefaultMode ()));
#endif

    connect (m_UsbModeCombo, SIGNAL (activated (int)),
             SLOT (usbModeActivated (int)));

    mainPolicy->addItem (m_UsbModeCombo);
    mainPolicy->addStretch ();

    setLayout (mainLayout);

    retranslateUi ();
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
    if (m_logic->getDefaultMode() == usbModes[idx])
        return;

    /*
     * First remove the old error notification
     */
    if (m_error != 0)
    {
        m_error->remove ();
        delete m_error;
        m_error = 0;
    }

    QmUSBMode::Mode active = m_logic->getMode ();

    /*
     * If we are connected and some mode active, then
     * show an error message and set the mode back
     * to original
     */
    if ((active == QmUSBMode::MassStorage) ||
        (active == QmUSBMode::OviSuite))
    {
        /*
         * Set checked on the previously active button
         */
        m_UsbModeCombo->setCurrentIndex (usbModeIndex (m_logic->getDefaultMode ()));

        SYS_DEBUG ("newidx = %d", m_UsbModeCombo->currentIndex ());
        SYS_DEBUG ("text = %s", SYS_STR (m_UsbModeCombo->currentText ()));

        /*
         * Create the error notification
         */
        m_error = new MNotification (
            MNotification::DeviceErrorEvent,
//% "You cannot change USB mode while USB is connecting.<br/>Eject USB device first, and then change setting."
            "", qtTrId ("qtn_usb_change_incorrect"));

        /*
         * And show it
         */
        m_error->publish ();

        return;
    }

    QmUSBMode::Mode newmode = usbModes[idx];
    m_logic->setDefaultMode (newmode);

    /*
     * If we are connected, and we've changed the default
     * mode lets activate the selected mode...
     */
    if (m_logic->getMode () == QmUSBMode::ChargingOnly)
        m_logic->setMode (newmode);

    emit settingsChanged ();
#endif
}

void
UsbView::retranslateUi ()
{
    //% "Default USB device mode"
    m_UsbModeCombo->setTitle (qtTrId ("qtn_usb_default_info"));
    m_UsbModeCombo->setItemText (0, qtTrId ("qtn_usb_always_ask"));
    m_UsbModeCombo->setItemText (1, qtTrId ("qtn_usb_mass_storage"));
    m_UsbModeCombo->setItemText (2, qtTrId ("qtn_usb_ovi_suite"));
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

