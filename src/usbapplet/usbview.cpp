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

#include "usbview.h"

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButtonGroup>
#include <MNotification>
#include <MLocale>

#define DEBUG
#include "../debug.h"

#define BUTTON_ALWAYS_ASK   0
#define BUTTON_MASS_STORAGE 1
#define BUTTON_OVI_SUITE    2

#ifdef HAVE_QMSYSTEM
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
    QGraphicsLinearLayout   *layout;

// Creating the main layout
    layout = new QGraphicsLinearLayout (Qt::Vertical);

// Creating & adding the info-label

    m_info_label = new MLabel;
    layout->addItem (m_info_label);

// Creating, filling and adding the mode-list
    m_btn_group = new MButtonGroup ();
    m_btn_group->setExclusive (true);

    MLayout *buttonsLayout = new MLayout;
    MLinearLayoutPolicy *vlayout =
        new MLinearLayoutPolicy (buttonsLayout, Qt::Vertical);
    vlayout->setNotifyWidgetsOfLayoutPositionEnabled (true);
    vlayout->setSpacing (0.);

    MLinearLayoutPolicy *hlayout =
        new MLinearLayoutPolicy (buttonsLayout, Qt::Horizontal);
    hlayout->setNotifyWidgetsOfLayoutPositionEnabled (true);
    hlayout->setSpacing (0.);

    buttonsLayout->setPortraitPolicy (vlayout);
    buttonsLayout->setLandscapePolicy (hlayout);

    for (int i = 0; i < 3; i++)
    {
        int id;
        m_buttons[i] = new MButton;
        m_buttons[i]->setViewType(MButton::groupType);
        m_buttons[i]->setCheckable (true);
        vlayout->addItem (m_buttons[i]);
        hlayout->addItem (m_buttons[i]);
        hlayout->setStretchFactor (m_buttons[i], 2);

        switch (i)
        {
            case BUTTON_MASS_STORAGE:
                m_buttons[i]->setObjectName("MassStorage");
                id = (int) QmUSBMode::MassStorage;
                break;
            case BUTTON_OVI_SUITE:
                m_buttons[i]->setObjectName("OviSuite");
                id = (int) QmUSBMode::OviSuite;
                break;
            case BUTTON_ALWAYS_ASK:
            default:
                m_buttons[i]->setObjectName("AlwaysAsk");
                id = (int) QmUSBMode::Ask;
                break;
        }

        m_btn_group->addButton (m_buttons[i], id);
    }

    layout->addItem (buttonsLayout);

    int current_setting = (int) m_logic->getDefaultMode ();

    if (m_btn_group->button (current_setting) == 0)
        m_btn_group->button ((int) QmUSBMode::Ask)->setChecked (true);
    else
        m_btn_group->button (current_setting)->setChecked (true);

    connect (m_btn_group, SIGNAL (buttonClicked (int)),
             this, SLOT (selectionChanged (int)));

    layout->addStretch ();

    setLayout (layout);

    layout->invalidate ();

    retranslateUi ();
    #endif
}

void
UsbView::selectionChanged (int id)
{
    #ifdef HAVE_QMSYSTEM
    QmUSBMode::Mode newmode = (QmUSBMode::Mode) id;
    QmUSBMode::Mode active = m_logic->getMode ();

    /*
     * Do nothing if we just tapped on the
     * currently selected one...
     */
    if ((int) active == id)
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
        m_btn_group->blockSignals (true);

        /*
         * Set checked on the previously active button
         */
        int current_setting = (int) m_logic->getDefaultMode ();

        if (m_btn_group->button (current_setting) == 0)
            m_btn_group->button ((int) QmUSBMode::Ask)->setChecked (true);
        else
            m_btn_group->button (current_setting)->setChecked (true);

        m_btn_group->blockSignals (false);

        /*
         * Create the error notification
         */
        m_error = new MNotification (
            MNotification::DeviceErrorEvent,
//% "You cannot change USB mode while USB is connecting.<br/>Eject USB device first, and then change setting."
            "", QString ("<p>%1</p>").arg (qtTrId ("qtn_usb_change_incorrect")));

        /*
         * And show it
         */
        m_error->publish ();

        return;
    }

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
    m_info_label->setText (qtTrId ("qtn_usb_default_info"));

    //% "Always ask"
    m_buttons[BUTTON_ALWAYS_ASK]->setText (qtTrId ("qtn_usb_always_ask"));
    //% "Mass Storage mode"
    m_buttons[BUTTON_MASS_STORAGE]->setText (qtTrId ("qtn_usb_mass_storage"));
    //% "Ovi Suite mode"
    m_buttons[BUTTON_OVI_SUITE]->setText (qtTrId ("qtn_usb_ovi_suite"));
}

