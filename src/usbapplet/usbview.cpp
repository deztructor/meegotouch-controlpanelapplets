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

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButtonGroup>
#include <MComboBox>
#include <MNotification>
#include <MContainer>

#undef DEBUG
#include "../debug.h"
#ifdef HAVE_QMSYSTEM

static QmUSBMode::Mode usbModes[3] = {QmUSBMode::Ask, QmUSBMode::MassStorage, QmUSBMode::OviSuite};
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
#ifdef HAVE_QMSYSTEM
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
    MLabel *title =
        addTitleLabel (mainPolicy,
                       "CommonAppicationHeaderInverted",
                       "CommonApplitationHeaderInverted");
    //% "USB"
    title->setText (qtTrId ("qtn_usb_title"));
#endif

    m_UsbModeCombo = new MComboBox();                                           
    m_UsbModeCombo->setTitle(qtTrId ("qtn_usb_default_info"));            
    m_UsbModeCombo->setStyleName ("CommonComboBoxInverted");                    
    m_UsbModeCombo->setObjectName ("UsbModeCombo");              
                                                                                
    m_UsbModeCombo->addItem(qtTrId("qtn_usb_always_ask"));                            
    m_UsbModeCombo->addItem(qtTrId("qtn_usb_mass_storage"));                             
    m_UsbModeCombo->addItem(qtTrId("qtn_usb_ovi_suite"));                         
                                                                                
    #ifdef HAVE_QMSYSTEM
    QmUSBMode::Mode active = m_logic->getMode ();
    int currentIdx;
    switch (active)
    {
      case QmUSBMode::MassStorage:
		currentIdx = UsbModeMassStorage;
		break;
      case QmUSBMode::OviSuite:
		currentIdx = UsbModeOviSuite;
		break;
      default:
		currentIdx = UsbModeAlwaysAsk;
		break;
     }
    m_UsbModeCombo->setCurrentIndex(currentIdx);
    #endif 
    connect (m_UsbModeCombo, SIGNAL (activated (int)),                           
             this, SLOT (usbModeActivated (int)));                              

    mainPolicy->addItem (m_UsbModeCombo);
    mainPolicy->addStretch ();

    setLayout (mainLayout);

    retranslateUi ();
#endif
}

void
UsbView::usbModeActivated (int idx)
{
    #ifdef HAVE_QMSYSTEM
    QmUSBMode::Mode active = m_logic->getMode ();

    /*
     * Do nothing if we just tapped on the
     * currently selected one...
     */
    if ((int) m_logic->getDefaultMode() == idx)
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

    /*
     * If we are connected and some mode active, then
     * show an error message and set the mode back
     * to original
     */
    if ((active == QmUSBMode::MassStorage) ||
        (active == QmUSBMode::OviSuite))
    {
        m_UsbModeCombo->blockSignals (true);

        /*
         * Set checked on the previously active button
         */
        int current_setting = (int) m_logic->getDefaultMode ();
	m_UsbModeCombo->setCurrentIndex(current_setting);
        m_UsbModeCombo->blockSignals (false);

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

    SYS_DEBUG ("emit settingsChanged ()");
    emit settingsChanged ();
    #endif
}

void
UsbView::retranslateUi ()
{
    //% "Default USB device mode"
    m_UsbModeCombo->setTitle (qtTrId ("qtn_usb_default_info"));
    m_UsbModeCombo->setItemText (UsbModeAlwaysAsk, qtTrId ("qtn_usb_always_ask"));
    m_UsbModeCombo->setItemText (UsbModeMassStorage, qtTrId ("qtn_usb_mass_storage"));
    m_UsbModeCombo->setItemText (UsbModeOviSuite, qtTrId ("qtn_usb_ovi_suite"));
}

MLabel *
UsbView::addTitleLabel (
    MLinearLayoutPolicy     *targetPolicy,
    const char              *panelStyleName,
    const char              *labelStyleName)
{
    MLabel                  *label;
    MContainer              *container;
    QGraphicsLinearLayout   *containerLayout;

    label = new MLabel;
    label->setStyleName (labelStyleName);

    container = new MContainer;
    container->setStyleName (panelStyleName);
    container->setContentsMargins (0., 0., 0., 0.);
    container->setHeaderVisible (false);

    containerLayout = new QGraphicsLinearLayout (Qt::Horizontal);
    containerLayout->setContentsMargins (0., 0., 0., 0.);
    containerLayout->addItem (label);

    container->centralWidget ()->setLayout (containerLayout);

    targetPolicy->addItem (container);
    return label;
}

