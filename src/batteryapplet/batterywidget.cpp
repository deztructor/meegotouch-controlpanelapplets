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

#define DEBUG
#define WARNING
#include "../debug.h"

static const int ActivationContainerPosition = 2;
static const int SliderContainerPosition = 3;

/******************************************************************************
 * BatteryWidget implementation.
 */
BatteryWidget::BatteryWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        m_logic (0),
        m_MainLayout (0),
        m_RemainingContainer (0),
        m_ActivationContainer (0),
        m_SliderContainer (0),
        m_PSMAutoButton (0),
        PSMButton (0),
        m_BatteryImage (0),
        m_TitleLabel (0),
        m_UILocked (false)
{
    SYS_DEBUG ("Starting in %p", this);
    /*
     * One can assume, that when the applet is started the power save mode is
     * not active.
     */
    m_PSMButtonToggle = false;
    setContentsMargins (0., 0., 0., 0.);

    // instantiate the batterybusinesslogic
    m_logic = new BatteryBusinessLogic;
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

void 
BatteryWidget::initWidget ()
{
    MLayout     *layout;
   
    /*
     * Creating a layout that holds the rows of the internal widgets.
     */
    layout = new MLayout (this);
    m_MainLayout = new MLinearLayoutPolicy (layout, Qt::Vertical);
    m_MainLayout->setContentsMargins (0., 0., 0., 0.);
    m_MainLayout->setSpacing (0.);
    setLayout (layout);

    /*
     * Adding the rows of widgets.
     */
    addHeaderContainer ();
    addRemainingCapacityWidget ();
    addAutoActivationWidget ();
    addSliderContainer ();
    addPowerSaveButton ();

    /*
     * Initializing the wigets and connecting the signals.
     */
    // SliderContainer signals and slots, and initialization
    m_SliderContainer->initSlider (m_logic->PSMThresholdValues ());
    m_SliderContainer->updateSlider (m_logic->PSMThresholdValue ());
    connect (m_SliderContainer, SIGNAL (PSMThresholdValueChanged (int)),
             m_logic, SLOT (setPSMThresholdValue (int)),
             Qt::DirectConnection);
    
    // connect the value receive signals
    connect (m_logic, SIGNAL(remainingBatteryCapacityChanged(int)),
             this, SLOT(remainingBatteryCapacityReceived(int)));

    // Connect the batteryImage slots.
    connect (m_logic, SIGNAL (batteryCharging (int)),
             m_BatteryImage, SLOT (startCharging (int)));
    connect (m_logic, SIGNAL (batteryBarValueReceived (int)),
             m_BatteryImage, SLOT (updateBatteryLevel (int)));
    connect (m_logic, SIGNAL (PSMValueReceived (bool)),
             m_BatteryImage, SLOT (setPSMValue (bool)));

    // Connecting the signals of the businesslogic.
    connect (m_logic, SIGNAL (PSMValueReceived (bool)),
             this, SLOT (PSMValueReceived (bool)));
    connect (m_logic, SIGNAL(batteryFull()),
             m_BatteryImage, SLOT(chargeComplete()));
    connect (m_logic, SIGNAL(batteryFull()),
             this, SLOT(chargeComplete()));
    
    // Initialize the values from the business logic
    m_logic->requestValues ();
}

void 
BatteryWidget::addHeaderContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonXLargeGroupHeaderPanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * The label that we use as title.
     */
    //% "Battery"
    m_TitleLabel = new MLabel (qtTrId("qtn_ener_battery"));
    m_TitleLabel->setStyleName ("CommonXLargeGroupHeaderInverted");
    layout->addItem (m_TitleLabel);
    layout->setAlignment (m_TitleLabel, Qt::AlignLeft);
    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);

}

void 
BatteryWidget::addRemainingCapacityWidget ()
{
    Q_ASSERT (m_MainLayout);
    
    m_BatteryImage = new BatteryImage (this);
    m_BatteryImage->setStyleName ("commonIconInverted");

    //% "Battery level \%L1\%"
    m_RemainingContainer = new PercentageContainer (
            qtTrId ("qtn_ener_battery_level"), m_BatteryImage);

    m_MainLayout->addItem (m_RemainingContainer);
    m_MainLayout->setStretchFactor (m_RemainingContainer, 0);
    
    connect (m_logic, SIGNAL(batteryCharging (int)),
             this, SLOT(charging(int)));
    m_logic->remainingCapacityRequired();

}

void 
BatteryWidget::addAutoActivationWidget ()
{
    QGraphicsLinearLayout *layout;
    
    MLabel                *activationLevelLabel;

    Q_ASSERT (m_MainLayout);
    Q_ASSERT (m_logic);

    /*
     * Creating the container and the layout.
     */
    m_ActivationContainer = new MContainer (this);
    m_ActivationContainer->setStyleName ("CommonPanelInverted");
    m_ActivationContainer->setHeaderVisible (false);
    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    
    m_ActivationContainer->centralWidget()->setLayout (layout);

    /*
     * A label for the PSM auto activation.
     */
    //% "Auto activate power save"
    activationLevelLabel = new MLabel(qtTrId ("qtn_ener_autops"));
    activationLevelLabel->setStyleName ("CommonSingleTitleInverted");
    
    /*
     * A help button for the PSM auto activation.
     */
    #ifdef HAVE_USERGUIDE
    HelpButton* helpButton = new HelpButton ("IDUG_MEEGO_BATTERY.html");
    helpButton->setViewType(MButton::iconType);
    helpButton->setIconID ("icon-m-content-description");
    #endif
    
    /*
     * A switch that turns the auto PSM mode on and off
     */
    m_PSMAutoButton = new MButton;
    m_PSMAutoButton->setStyleName ("CommonSwitchInverted");
    m_PSMAutoButton->setObjectName ("AutoActivatePowerSaveButton");
    m_PSMAutoButton->setCheckable (true);
    m_PSMAutoButton->setViewType (MButton::switchType);
    //m_PSMAutoButton->setChecked (m_logic->PSMAutoValue());
    
    connect (m_PSMAutoButton, SIGNAL (toggled (bool)),
             this, SLOT (PSMAutoToggled (bool)));
   
    /*
     * Adding the widgets to the layout.
     */
    layout->addItem (activationLevelLabel);
    layout->setAlignment(activationLevelLabel, Qt::AlignLeft|Qt::AlignVCenter);
    #ifdef HAVE_USERGUIDE
    layout->addItem(helpButton);
    #endif
    layout->addItem (m_PSMAutoButton);
    layout->setAlignment(m_PSMAutoButton, Qt::AlignRight | Qt::AlignVCenter);

    /*
     * Adding the new row to the main layout.
     */
    m_MainLayout->addItem (m_ActivationContainer);
    m_MainLayout->setStretchFactor (m_ActivationContainer, 0);
}

void 
BatteryWidget::addSliderContainer ()
{
    Q_ASSERT (m_MainLayout);

    m_SliderContainer = new SliderContainer;

    /*
     *
     */
    if(m_PSMAutoButton->isChecked ()) {
        m_MainLayout->addItem (m_SliderContainer);
        m_MainLayout->setStretchFactor (m_SliderContainer, 0);
    }
}

void 
BatteryWidget::addPowerSaveButton ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonPanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * PSMButton is used to immediately turn the power save mode on/off.
     */
    PSMButton = new MButton;
    PSMButton->setStyleName ("CommonSingleButtonInverted");
    updatePSMButton ();
    // FIXME: why here?
    connect (PSMButton, SIGNAL (released ()),
             this, SLOT (PSMButtonReleased ()));

    /*
     * Adding the button to our layout.
     */
    layout->addStretch ();
    layout->addItem (PSMButton);
    layout->addStretch ();
    layout->setAlignment (PSMButton, Qt::AlignHCenter);

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
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
        m_SliderContainer->initPSMAutoButton (false);
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
            m_SliderContainer->updateSlider (m_logic->PSMThresholdValue ());
            if(m_MainLayout)
                if(m_MainLayout->indexOf(m_SliderContainer) == -1)
                {
                    m_MainLayout->insertItem (
                            SliderContainerPosition, m_SliderContainer);
                    m_MainLayout->setStretchFactor (m_SliderContainer, 0);
                }
        }
        else
        {
            m_MainLayout->removeAt(m_MainLayout->indexOf(m_SliderContainer));
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

    /*
     * A little extra check just to be sure the widget is fully created.
     */
    if (!m_RemainingContainer)
        return;

    if (!(m_logic->isCharging())) {
        if (!m_PSMButtonToggle) {
            m_RemainingContainer->updateCapacity (pct);
        } else {
            //% "Power save mode"
            m_RemainingContainer->setText (qtTrId ("qtn_ener_power_save_mode"));
        }
    } else {
        if (!m_PSMButtonToggle) {
            //% "Charging"
            m_RemainingContainer->setText(qtTrId ("qtn_ener_charging"));
        } else {
            //% "Power save mode"
            m_RemainingContainer->setText (qtTrId ("qtn_ener_power_save_mode"));
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

    if (m_MainLayout && m_ActivationContainer) {
        if (!PSMEnabled) {
            if (m_MainLayout->indexOf(m_ActivationContainer) == -1) {
                m_PSMAutoButton->setChecked(false);
                m_MainLayout->insertItem (
                        ActivationContainerPosition, m_ActivationContainer);
                m_MainLayout->setStretchFactor (m_ActivationContainer, 0);
            }
            m_logic->remainingCapacityRequired();
        } else {
            m_MainLayout->removeAt(m_MainLayout->indexOf(m_SliderContainer));
            m_MainLayout->removeAt(m_MainLayout->indexOf(m_ActivationContainer));
            //% "Power save mode"
            m_RemainingContainer->setText (qtTrId ("qtn_ener_power_save_mode"));
        }
    }

    m_UILocked = false;
}

void
BatteryWidget::retranslateUi ()
{
    if (m_TitleLabel)
        //% "Battery"
        m_TitleLabel->setText (qtTrId("qtn_ener_battery"));

    // This call will reload the translated text on PSButton
    updatePSMButton ();

    // This call will retranslate the label (infoText)
    m_SliderContainer->retranslate ();

    // FIXME: Why do we need this?
    m_logic->remainingCapacityRequired();
}

void BatteryWidget::charging(int animation_rate)
{
    SYS_DEBUG("Charging rate: %d", animation_rate);
    if(animation_rate > 0) {
        //% "Charging"
        m_RemainingContainer->setText(qtTrId ("qtn_ener_charging"));
    }
}

void BatteryWidget::chargeComplete()
{
    //% "Charging complete"
    m_RemainingContainer->setText(qtTrId ("qtn_ener_charcomp"));
}
