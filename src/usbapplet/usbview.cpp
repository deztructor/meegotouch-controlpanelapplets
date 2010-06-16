/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "usbview.h"

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MButtonGroup>
#include <MNotification>
#include <MLocale>

#define BUTTON_ALWAYS_ASK   0
#define BUTTON_MASS_STORAGE 1
#define BUTTON_OVI_SUITE    2

using namespace Maemo;

UsbView::UsbView (Maemo::QmUSBMode *logic) :
    m_logic (logic)
{
    initWidget ();
}

void
UsbView::initWidget ()
{
    QGraphicsLinearLayout   *layout;

// Creating the main layout
    layout = new QGraphicsLinearLayout (Qt::Vertical);

// Creating & adding the info-label

    m_info_label = new MLabel;
    m_info_label->setObjectName ("info_label");

    layout->addItem (m_info_label);

// Creating, filling and adding the mode-list
    m_btn_group = new MButtonGroup ();
    m_btn_group->setExclusive (true);

    for (int i = 0; i < 3; i++)
    {
        int id;
        m_buttons[i] = new MButton;
        m_buttons[i]->setCheckable (true);
        layout->addItem (m_buttons[i]);

        switch (i)
        {
            case BUTTON_MASS_STORAGE:
                id = (int) QmUSBMode::MassStorage;
                break;
            case BUTTON_OVI_SUITE:
                id = (int) QmUSBMode::OviSuite;
                break;
            case BUTTON_ALWAYS_ASK:
            default:
                id = (int) QmUSBMode::Ask;
                break;
        }

        m_btn_group->addButton (m_buttons[i], id);
    }

    int current_setting = (int) m_logic->getDefaultMode ();

    if (m_btn_group->button (current_setting) == 0)
        m_btn_group->button ((int) QmUSBMode::Ask)->setChecked (true);
    else
        m_btn_group->button (current_setting)->setChecked (true);

    connect (m_btn_group, SIGNAL (buttonClicked (int)),
             this, SLOT (selectionChanged (int)));

    layout->addStretch ();

    setLayout (layout);

    retranslateUi ();
}

void
UsbView::selectionChanged (int id)
{
    QmUSBMode::Mode newmode = (QmUSBMode::Mode) id;
    QmUSBMode::Mode active = m_logic->getMode ();

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
        MNotification error (MNotification::DeviceErrorEvent,
                             "",
//% "You cannot change USB mode while USB is connecting.<br/>Eject USB device first, and then change setting."
                             qtTrId ("qtn_usb_change_incorrect"));
        /*
         * And show it
         */
        error.publish ();

        return;
    }

    m_logic->setDefaultMode (newmode);

    /*
     * If we are connected, and we've changed the default
     * mode lets activate the selected mode...
     */
    if (m_logic->getMode () == QmUSBMode::ChargingOnly)
        m_logic->setMode (newmode);

    emit settingsChanged ();
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

