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

#include <MSeparator>
#include <MLabel>
#include <MLayout>
#include <QGraphicsLinearLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MButtonGroup>
#include <MBanner>
#include <MApplication>
#include <MImageWidget>
#include <MContainer>
#include <MWidgetController>


#include <QTimer>
#include <QStringList>

#include "../styles.h"

#undef DEBUG
#define WARNING
#include "../debug.h"

#ifdef HAVE_QMSYSTEM
using namespace MeeGo;

#define qtTrIdShort(id) qtTrId(id).split(QChar(0x9c)).last()
#define VALID_USB_MODE(modeType) ((modeType >= 0 && modeType <= 2))

static QmUSBMode::Mode usbModes[3] = { 
    QmUSBMode::Ask,
    QmUSBMode::MassStorage,
    QmUSBMode::OviSuite 
};

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
    MLayout          *mainLayout;
    MSeparator       *separator;

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
            this, m_policy, APP_TITLE_LABEL_STYLE_NAME);
    //% "USB"
    title->setText (qtTrId ("qtn_usb_title"));
    m_infoOrder++;
#endif

    /*
     * A subtitle and then a separator. The separator right below the infotext
     * is going to be added by the infotext when it is necessary. The banner
     * below the subtitle is always there.
     */
    addSubTitle (this, m_policy, 
            qtTrId("qtn_usb_default_info"));

    separator = new MSeparator;
    separator->setStyleName ("CommonLargeSpacer");
    m_policy->addItem (separator);

    /*
     * The buttons to change the USB mode.
     */
    addButtons ();

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
    if (m_logic->getDefaultMode() == usbModes[idx])
        return;

    QmUSBMode::Mode active = m_logic->getMode ();

    /*
     * If we are connected and some mode active, then show an error message and
     * set the mode back to original
     */
    if ((active == QmUSBMode::MassStorage) ||
        (active == QmUSBMode::OviSuite))
    {
        /*
         * Set checked on the previously active button
         */
        setSelectedButtonIndex (usbModeIndex(m_logic->getDefaultMode()));

        MBanner *infoBanner = new MBanner;
        infoBanner->setStyleName ("InformationBanner");
        infoBanner->setObjectName ("InfoBanner");

        //% "Cannot change the USB mode when it is connected"
        infoBanner->setTitle (qtTrId ("qtn_usb_change_incorrect"));

        infoBanner->appear (MApplication::instance ()->activeWindow (),
                            MSceneWindow::DestroyWhenDone);
        return;
    }

    QmUSBMode::Mode newmode = usbModes[idx];

    SYS_DEBUG ("Setting USB mode to %d", newmode);
    m_logic->setDefaultMode (newmode);

    /*
     * If we are connected, and we've changed the default mode lets activate the
     * selected mode...
     */
    if (active == QmUSBMode::ChargingOnly)
        m_logic->setMode (newmode);
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

void
UsbView::addSubTitle (
        QGraphicsWidget     *parent,
        MLinearLayoutPolicy *targetPolicy,
        const QString       &subTitle)
{
    MContainer              *container;
    QGraphicsLinearLayout   *layout;
    MLabel                  *label;
    MSeparator              *separator;

    container = new MContainer (parent);
    container->setContentsMargins (0., 0., 0., 0.);
    container->setStyleName (SUBTITLE_PANEL_STYLE_NAME);
    container->setHeaderVisible (false);
    
    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);
    /*
     *
     */
    separator = new MSeparator;
    separator->setStyleName (SUBTITLE_DIVIDER_STYLE_NAME);

    /*
     *
     */
    label = new MLabel (subTitle);
    label->setStyleName (SUBTITLE_LABEL_STYLE_NAME);
    

    layout->addItem (separator);
    layout->setStretchFactor (separator, 2);

    layout->addItem (label);
    layout->setAlignment (label, Qt::AlignLeft);
    layout->setStretchFactor (label, 0);

    container->centralWidget()->setLayout (layout);
    targetPolicy->addItem (container);
}

void
UsbView::addButtons ()
{
    MButtonGroup *group = new MButtonGroup();
    int           currentModeIndex = -1;

    #ifdef HAVE_QMSYSTEM
    currentModeIndex = usbModeIndex (m_logic->getDefaultMode ());
    #endif
    
    for (int n = 0; n < UsbModeLastMode; ++n) {
        MButton *button;

        SYS_DEBUG ("*** n = %d", n);
        button = new MButton (usbModeUIString(UsbModeType(n)));
            
        group->addButton (button);
        button->setStyleName (usbModeButtonStyle(UsbModeType(n)));
        button->setViewType (MButton::groupType);
        button->setCheckable (true);
        m_policy->addItem (button);
        m_policy->setAlignment (button, Qt::AlignCenter);

        m_Buttons.append (button);

        if (n == currentModeIndex) {
            button->setChecked (true);
        }

        connect (button, SIGNAL (toggled(bool)),
                    this, SLOT(buttonToggled (bool)));
    }
}

QString
UsbView::currentText () const
{
    SYS_DEBUG ("");

#ifdef HAVE_QMSYSTEM
    QmUSBMode::Mode active = m_logic->getMode ();

    /* XXX */
    active = QmUSBMode::MassStorage;

    switch (active) {
        case QmUSBMode::MassStorage:
            SYS_DEBUG ("QmUSBMode::MassStorage");
            //% "%1 active"
            return qtTrId ("qtn_usb_active_mode").arg (
            //% "Mass storage mode"
                   qtTrId ("qtn_usb_mass_storage"));
            break;

        case QmUSBMode::OviSuite:
            SYS_DEBUG ("QmUSBMode::OviSuite");
            //% "%1 active"
            return qtTrId ("qtn_usb_active_mode").arg (
            //% "Mass storage mode"
                   qtTrId ("qtn_usb_ovi_suite"));
            break;

        case QmUSBMode::ModeRequest:
            SYS_DEBUG ("QmUSBMode::ModeRequest");
            // Seems that the ChargingOnly comes late (30 sec or so), and we can
            // show the chargingonly early.
            //% "Current state: Charging only"
            return qtTrId ("qtn_usb_charging");
            break;
        
        case QmUSBMode::Ask:
            SYS_DEBUG ("QmUSBMode::Ask");
            // Seems that the ChargingOnly comes late (30 sec or so), and we can
            // show the chargingonly early.
            //% "Current state: Charging only"
            return qtTrId ("qtn_usb_charging");
            break;

        case QmUSBMode::ChargingOnly:
            SYS_DEBUG ("QmUSBMode::ChargingOnly");
            //% "Current state: Charging only"
            return qtTrId ("qtn_usb_charging");
            break;

        case QmUSBMode::Disconnected:
            SYS_DEBUG ("QmUSBMode::Disconnected");
            // Nothing to do.
            break;
        
        case QmUSBMode::Undefined:
            SYS_DEBUG ("QmUSBMode::Undefined");
            // Nothing to do.
            break;

        default:
            SYS_DEBUG ("UNHANDLED MODE: %d", active);
            return "";
            break;
    }
#endif
    return "";
}

void
UsbView::updateInfoLabel ()
{
    SYS_DEBUG ("");
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

    if (infoText.isEmpty ()) {
        /*
         * Label needs to be removed if it is there...
         */
        if (! m_infoLabel)
            return;

        /*
         * Delete the separator, and the infoWidget
         */
        delete m_policy->itemAt (m_infoOrder + 1);
        delete m_policy->itemAt (m_infoOrder);
        m_infoLabel = 0;

        return;
    }

    SYS_DEBUG ("creating the info-widget");
    
    /*
     * When we shown no text we didn't need the separator, now we need one.
     */
    MSeparator       *separator;
    separator = new MSeparator;
    separator->setStyleName ("CommonLargeSpacer");

    /*
     * Create the info label widget, and initialize its contents...
     */
    MWidgetController *infoWidget = new MWidgetController;
    infoWidget->setStyleName ("CommonTextFrameInverted");
    infoWidget->setContentsMargins (0,0,0,0);

    QGraphicsLinearLayout *iwLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);
    iwLayout->setContentsMargins (0,0,0,0);
    iwLayout->setSpacing (0);

    MImageWidget *iwIcon = new MImageWidget;
    iwIcon->setStyleName ("CommonMainIcon");
    iwIcon->setImage ("icon-m-common-usb");
    SYS_DEBUG ("iwIcon->setImage (\"icon-m-common-usb\");");
    iwLayout->addItem (iwIcon);

    m_infoLabel = new MLabel;
    m_infoLabel->setStyleName ("CommonSingleTitleInverted");
    m_infoLabel->setText (infoText);
    iwLayout->addItem (m_infoLabel);

    infoWidget->setLayout (iwLayout);

    /*
     * Insert it to the proper place... after the separator.
     */
    m_policy->insertItem (m_infoOrder, infoWidget);
    m_policy->insertItem (m_infoOrder + 1, separator);
}

void 
UsbView::buttonToggled (
        bool state)
{
    int index;

    /*
     * For every click we got two signals: one buttons is going to be un-toggled
     * and one is going to be toggled. To handle the event we only handle one of
     * these.
     */
    if (!state)
        return;
    
    index = selectedButtonIndex ();
    if (VALID_USB_MODE(index))
        usbModeActivated (index);

    SYS_DEBUG ("*** state = %s", SYS_BOOL(state));
}

QString 
UsbView::usbModeUIString (
        UsbModeType type) const
{
    switch (type) {
        case UsbModeAsk:
            return qtTrId ("qtn_usb_always_ask");

        case UsbModeMassStorage:
            return qtTrId("qtn_usb_mass_storage");

        case UsbModeOviSuite:
            return qtTrId("qtn_usb_ovi_suite");

        default:
            SYS_WARNING ("Invalid USB mode: %d", type);
    }

    return QString();
}

QString 
UsbView::usbModeButtonStyle (
        UsbModeType type) const
{
    switch (type) {
        case UsbModeAsk:
            return "CommonTopSplitButtonInverted";

        case UsbModeMassStorage:
            return "CommonVerticalCenterSplitButtonInverted";

        case UsbModeOviSuite:
            return "CommonBottomSplitButtonInverted";

        default:
            SYS_WARNING ("Invalid USB mode: %d", type);
    }

    return QString();
}

int 
UsbView::selectedButtonIndex () const
{
    int index = -1;

    for (int n = 0; n < m_Buttons.size(); ++n) {
        if (m_Buttons[n]->isChecked())
            index = n;
    }

    SYS_DEBUG ("returning %d", index);
    return index;
}

void
UsbView::setSelectedButtonIndex (
        int index)
{
    for (int n = 0; n < m_Buttons.size(); ++n) {
        if (n == index) {
            if (!m_Buttons[n]->isChecked())
                m_Buttons[n]->setChecked(true);
            break;
        }
    }
}
