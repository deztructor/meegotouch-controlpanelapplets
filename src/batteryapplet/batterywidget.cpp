/****************************************************************************+
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

#include "batterywidget.h"
#include "batteryimage.h"
#include "batterybusinesslogic.h"
#include "dcpbattery.h"
#include "slidercontainer.h"
#include "percentagecontainer.h"

#include <QGraphicsLinearLayout>
#include <QTimer>

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MContainer>
#include <MLabel>
#include <MSlider>
#include <MBasicLayoutAnimation>

#ifdef HAVE_USERGUIDE
#include <HelpButton>
#endif

#undef DEBUG
#define WARNING
#include "../debug.h"

BatteryWidget::BatteryWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        m_logic (0),
        m_UILocked (false),
        batteryImage (0),
        PSMButton (0),
        m_MainLayout (0),
        sliderContainer (0),
        remainingCapacityContainer (0)
{
    SYS_DEBUG ("Starting in %p", this);
    /*
     * One can assume, that when the applet is started the power save mode is
     * not active.
     */
    m_PSMButtonToggle = false;

    setContentsMargins (0., 0., 0., 0.);
    initWidget ();
}

BatteryWidget::~BatteryWidget ()
{
    SYS_DEBUG ("Destroying %p", this);

    if (m_logic)
    {
        delete m_logic;
        m_logic = 0;
    }
}

bool BatteryWidget::back ()
{
    return true; // back is handled by main window by default
}

void BatteryWidget::initWidget ()
{
    SYS_DEBUG ("Start");
    // instantiate the batterybusinesslogic
    m_logic = new BatteryBusinessLogic;

    // battery image
    batteryImage = new BatteryImage;

    // batteryRemainingCapacityPercentage
    //% "Battery level \%L1\%"
    remainingCapacityContainer = new PercentageContainer (
            qtTrId ("qtn_ener_battery_level"), batteryImage);

    connect (m_logic, SIGNAL(batteryCharging (int)),
             this, SLOT(charging(int)));

    m_logic->remainingCapacityRequired();

    /*
     * PSMButton is used to immediately turn the power save mode on/off.
     */
    PSMButton = new MButton;
    PSMButton->setObjectName ("CommonSingleButton");
    updatePSMButton ();

    connect (PSMButton, SIGNAL (released ()),
             this, SLOT (PSMButtonReleased ()));

    /*
     * sliderContainer
     */
    sliderContainer = new SliderContainer;

    /*
     * SliderContainer signals and slots,
     * and initialization
     */
    sliderContainer->initSlider (m_logic->PSMThresholdValues ());
    sliderContainer->updateSlider (m_logic->PSMThresholdValue ());

    connect (sliderContainer, SIGNAL (PSMThresholdValueChanged (int)),
             m_logic, SLOT (setPSMThresholdValue (int)),
             Qt::DirectConnection);

    MContainer            *buttonContainer;
    QGraphicsLinearLayout *buttonLayout;

    buttonContainer = new MContainer;
    buttonContainer->setObjectName ("CommonPanel");
    buttonContainer->setHeaderVisible (false);

    buttonLayout = new QGraphicsLinearLayout (Qt::Horizontal);

    buttonLayout->addStretch ();
    buttonLayout->addItem (PSMButton);
    buttonLayout->addStretch ();
    buttonLayout->setAlignment (PSMButton, Qt::AlignHCenter);

    buttonContainer->centralWidget()->setLayout (buttonLayout);

    MLayout               *activationLevelLabelLayout;
    MLabel                *activationLevelLabel;

    //% "Auto activate power save"
    activationLevelLabel = new MLabel(qtTrId ("qtn_ener_autops"));
    activationLevelLabelLayout = new MLayout;
    MLinearLayoutPolicy *hpolicy = new MLinearLayoutPolicy (activationLevelLabelLayout, Qt::Horizontal);

#ifdef HAVE_USERGUIDE
    HelpButton* helpButton = new HelpButton ("IDUG_MEEGO_BATTERY.html");
    helpButton->setViewType(MButton::iconType);
    helpButton->setIconID ("icon-m-content-description");
#endif

    m_PSMAutoButton = new MButton;
    m_PSMAutoButton->setStyleName ("CommonSwitch");
    m_PSMAutoButton->setObjectName ("AutoActivatePowerSaveButton");
    connect (m_PSMAutoButton, SIGNAL (toggled (bool)),
             this, SLOT (PSMAutoToggled (bool)));
    m_PSMAutoButton->setCheckable (true);
    m_PSMAutoButton->setViewType (MButton::switchType);
    m_PSMAutoButton->setChecked (m_logic->PSMAutoValue ());

    activationLevelLabelContainer = new MContainer;
    activationLevelLabelContainer->setObjectName("CommonPanel");
    activationLevelLabelContainer->setHeaderVisible (false);

    hpolicy->addItem (activationLevelLabel, Qt::AlignLeft | Qt::AlignVCenter);
#ifdef HAVE_USERGUIDE
    hpolicy->addItem(helpButton);
#endif
    hpolicy->addItem (m_PSMAutoButton, Qt::AlignRight | Qt::AlignVCenter);

    activationLevelLabelContainer->centralWidget()->setLayout (activationLevelLabelLayout);

    MLayout *layout = new MLayout;
    // mainContainer_
    m_MainLayout = new MLinearLayoutPolicy (layout, Qt::Vertical);
    m_MainLayout->setContentsMargins (0., 0., 0., 0.);
    m_MainLayout->setSpacing (0.);

    m_MainLayout->addItem (remainingCapacityContainer);
    m_MainLayout->setStretchFactor (remainingCapacityContainer, 0);

    m_MainLayout->addItem (activationLevelLabelContainer);
    m_MainLayout->setStretchFactor (activationLevelLabelContainer, 0);

    if(m_PSMAutoButton->isChecked ())
    {
        m_MainLayout->addItem (sliderContainer);
        m_MainLayout->setStretchFactor (sliderContainer, 0);
    }

    m_MainLayout->addItem (buttonContainer);
    m_MainLayout->setStretchFactor (buttonContainer, 0);

    MContainer *mainContainer = new MContainer;
    mainContainer->setHeaderVisible (false);
    mainContainer->setLayout (layout);

    // connect the value receive signals
    connect (m_logic, SIGNAL(remainingBatteryCapacityChanged(int)),
             this, SLOT(remainingBatteryCapacityReceived(int)));

    /*
     * Connect the batteryImage slots.
     */
    connect (m_logic, SIGNAL (batteryCharging (int)),
             batteryImage, SLOT (startCharging (int)));
    connect (m_logic, SIGNAL (batteryBarValueReceived (int)),
             batteryImage, SLOT (updateBatteryLevel (int)));
    connect (m_logic, SIGNAL (PSMValueReceived (bool)),
             batteryImage, SLOT (setPSMValue (bool)));

    connect (m_logic, SIGNAL (PSMValueReceived (bool)),
             this, SLOT (PSMValueReceived (bool)));

    connect (m_logic, SIGNAL(batteryFull()),
             batteryImage, SLOT(chargeComplete()));
    connect (m_logic, SIGNAL(batteryFull()),
             this, SLOT(chargeComplete()));
    // mainLayout
    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);

    mainLayout->setContentsMargins (0, 0, 0, 0);
    mainLayout->addItem (mainContainer);
    mainLayout->addStretch ();
    setLayout (mainLayout);

    // Initialize the values from the business logic
    m_logic->requestValues ();

    SYS_DEBUG ("End");
}

/*!
 * This function is called when the user clicked on the 'power save mode' button
 * that activates and disactivates the power saving mode. The function will call
 * the battery interface, but the UI will be changed only when the power save
 * mode really changed...
 */
void 
BatteryWidget::PSMButtonReleased ()
{
    bool newPSMValue = !m_PSMButtonToggle;
    SYS_DEBUG ("Setting PSMvalue to %s", SYS_BOOL (newPSMValue));

    /*
     * To 'deactivate' the power-save move, we need to turn
     * off the automatic power-saving...
     */
    if (newPSMValue == false)
    {
        m_logic->setPSMAutoValue (false);
        sliderContainer->initPSMAutoButton (false);
    }

    // UI will change only in PSMValueReceived slot...
    m_logic->setPSMValue (newPSMValue);
}

/*!
 * This slot is called when the psm auto switch is toggled.
 */
void
BatteryWidget::PSMAutoToggled (
        bool PSMAutoEnabled)
{
    SYS_DEBUG ("*** PSMAutoEnabled = %s", SYS_BOOL(PSMAutoEnabled));

    if (m_UILocked) {
        SYS_WARNING ("The UI is locked.");
    } else {
        m_logic->setPSMAutoValue (PSMAutoEnabled);

        if (PSMAutoEnabled)
        {
            /*
             * QmSystem returns 0 when PSMAuto is disabled,
             * so when we're enabling it, we've to re-query
             * the proper value
             */
            sliderContainer->updateSlider (m_logic->PSMThresholdValue ());
            if(m_MainLayout)
                if(m_MainLayout->indexOf(sliderContainer) == -1)
                {
                    m_MainLayout->insertItem (/*m_MainLayout->indexOf(activationLevelLabelContainer)+1*/2, sliderContainer);
                    m_MainLayout->setStretchFactor (sliderContainer, 0);
                }
        }
        else
        {
            m_MainLayout->removeAt(m_MainLayout->indexOf(sliderContainer));
        }
    }
}

void 
BatteryWidget::updatePSMButton ()
{
    if (m_PSMButtonToggle) {
        //% "Deactivate power save now"
        PSMButton->setText (qtTrId ("qtn_ener_dps"));
    } else {
        //% "Activate power save now"
        PSMButton->setText (qtTrId ("qtn_ener_aps"));
    }
}

void BatteryWidget::remainingBatteryCapacityReceived(const   int pct)
{
    SYS_DEBUG ("percentage = %d", pct);
    if(!(m_logic->isCharging()))
    {
        if(!m_PSMButtonToggle)
        {
            remainingCapacityContainer->updateCapacity (pct);
        }
        else
        {
            //% "Power save mode"
            remainingCapacityContainer->setText (qtTrId ("qtn_ener_power_save_mode"));
        }
    }
    else
    {
        if(!m_PSMButtonToggle)
        {
            //% "Charging"
            remainingCapacityContainer->setText(qtTrId ("qtn_ener_charging"));
        }
        else
        {
            //% "Power save mode"
            remainingCapacityContainer->setText (qtTrId ("qtn_ener_power_save_mode"));
        }
    }
}

void 
BatteryWidget::PSMValueReceived (
        bool PSMEnabled)
{
    SYS_DEBUG ("*** PSMEnabled = %s", SYS_BOOL (PSMEnabled));

    if (m_PSMButtonToggle == PSMEnabled) {
        SYS_DEBUG ("toggle already set");
        return;
    }
    m_PSMButtonToggle = PSMEnabled;

    updatePSMButton ();
    m_UILocked = true;
    if(m_MainLayout)
    {
        if (!PSMEnabled)
        {
            if(m_MainLayout->indexOf(activationLevelLabelContainer) == -1)
            {
                m_PSMAutoButton->setChecked(false);
                m_MainLayout->insertItem (/*m_MainLayout->indexOf(activationLevelLabelContainer)+1*/1, activationLevelLabelContainer);
                m_MainLayout->setStretchFactor (activationLevelLabelContainer, 0);
            }
            m_logic->remainingCapacityRequired();
        }
        else
        {
            m_MainLayout->removeAt(m_MainLayout->indexOf(sliderContainer));
            m_MainLayout->removeAt(m_MainLayout->indexOf(activationLevelLabelContainer));
            //% "Power save mode"
            remainingCapacityContainer->setText (qtTrId ("qtn_ener_power_save_mode"));
        }
    }

    m_UILocked = false;
}

void
BatteryWidget::retranslateUi ()
{
    // This call will reload the translated text on PSButton
    updatePSMButton ();

    // This call will retranslate the label (infoText)
    sliderContainer->retranslate ();

    m_logic->remainingCapacityRequired();
}

void BatteryWidget::charging(int animation_rate)
{
    SYS_DEBUG("Charging rate: %d", animation_rate);
    if(animation_rate > 0)
    {
        //% "Charging"
        remainingCapacityContainer->setText(qtTrId ("qtn_ener_charging"));
    }
}

void BatteryWidget::chargeComplete()
{
    //% "Charging complete"
    remainingCapacityContainer->setText(qtTrId ("qtn_ener_charcomp"));
}
