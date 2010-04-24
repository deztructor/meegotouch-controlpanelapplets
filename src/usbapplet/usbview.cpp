/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <QGraphicsLinearLayout>
#include <MButtonGroup>
#include <MLocale>

#include "usbview.h"

#define BUTTON_ALWAYS_ASK   0
#define BUTTON_MASS_STORAGE 1
#define BUTTON_OVI_SUITE    2

UsbView::UsbView (UsbSettingsLogic *logic) : DcpWidget (),
    m_logic (logic)
{
    initWidget ();
}

void
UsbView::initWidget ()
{
    QGraphicsLinearLayout   *layout;
    MButtonGroup            *button_group;

// Creating the main layout
    layout = new QGraphicsLinearLayout (Qt::Vertical);

// Creating & adding the info-label

    m_info_label = new MLabel;
    m_info_label->setObjectName ("info_label");

    layout->addItem (m_info_label);

// Creating, filling and adding the mode-list
    button_group = new MButtonGroup ();
    button_group->setExclusive (true);

    for (int i = 0; i < 3; i++)
    {
        int id;
        m_buttons[i] = new MButton;
        m_buttons[i]->setCheckable (true);
        layout->addItem (m_buttons[i]);

        switch (i)
        {
            case BUTTON_MASS_STORAGE:
                id = USB_MASS_STORAGE;
                break;
            case BUTTON_OVI_SUITE:
                id = USB_OVI_SUITE;
                break;
            case BUTTON_ALWAYS_ASK:
            default:
                id = USB_AUTO;
                break;
        }

        button_group->addButton (m_buttons[i], id);
    }

    int current_setting = m_logic->getUsbSetting ();

    if (button_group->button (current_setting) == 0)
        button_group->button (USB_AUTO)->setChecked (true);
    else
        button_group->button (current_setting)->setChecked (true);

    connect (button_group, SIGNAL (buttonClicked (int)),
             this, SLOT (selectionChanged (int)));

    setLayout (layout);

    retranslateUi ();
}

void
UsbView::selectionChanged (int id)
{
    m_logic->setUsbSetting ((usb_modes) id);

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

