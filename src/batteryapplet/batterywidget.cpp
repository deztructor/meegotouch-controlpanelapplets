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

#include <MStylableWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MContainer>
#include <MLabel>
#include <MSlider>
#include <MHelpButton>
#include <MBasicLayoutAnimation>

#define DEBUG
#define WARNING
#include "../debug.h"

#define USE_SPACERS

#ifdef USE_SPACERS
#include <MSeparator>
static const int ActivationContainerPosition = 4;
static const int LabelContainerPosition = 6;
static const int SliderContainerPosition = 7;
#else
static const int ActivationContainerPosition = 2;
static const int LabelContainerPosition = 3;
static const int SliderContainerPosition = 4;
#endif

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
     * We currently have 5 separators
     */
    for (int n = 0; n < 5; ++n)
        m_Separators[n] = 0;

    m_SeparatorPlacement[0] = 1;
    m_SeparatorPlacement[1] = 3;
    m_SeparatorPlacement[2] = 5;
    m_SeparatorPlacement[3] = 8;
    m_SeparatorPlacement[4] = 10;

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

    if (m_logic) {
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
    #ifdef USE_SPACERS
    // Row 0: The title label
    addHeaderContainer ();
    // Row 1: A spacer
    m_Separators[0] = addSpacer (
            "CommonSmallSpacerInverted",
            m_SeparatorPlacement[0]);
    // Row 2: Remaining capacity widget
    addRemainingCapacityWidget ();
    // Row 3: A spacer
    m_Separators[1] = addSpacer (
            "CommonItemDivider",
            m_SeparatorPlacement[1]);
    // Row 4: PSM Auto activation switch
    addAutoActivationWidget ();
    // Row 5: A spacer
    m_Separators[2] = addSpacer (
            "CommonItemDivider",
            m_SeparatorPlacement[2]);
    // Row 6-7: PSM Auto activation text and slider
    addSliderContainer ();
    // Row 8: A spacer
    m_Separators[3] = addSpacer (
            "CommonItemDivider",
            m_SeparatorPlacement[3]);
    // Row 9: PSM forced activation button.
    addPowerSaveButton ();
    // Row 10: another divider
    m_Separators[4] = addSpacer (
            "CommonSmallSpacerInverted",
            m_SeparatorPlacement[4]);
    // Row 11: Battery condition info.
    addBatteryConditionWidget ();
    #else
    // Row 1: The title label
    addHeaderContainer ();
    // Row 2: Remaining capacity widget
    addRemainingCapacityWidget ();
    // Row 3: PSM Auto activation switch
    addAutoActivationWidget ();
    // Row 4-5: PSM Auto activation text and slider
    addSliderContainer ();
    // Row 6: PSM forced activation button.
    addPowerSaveButton ();
    // Row 7: Battery condition info.
    addBatteryConditionWidget ();
    #endif
    
    m_MainLayout->addStretch();

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

    /*
     * And finally set the PSMAutoValue based on backend value,
     * this possibly triggers the slider container visibility also,
     * that is why we should call this after layout is set,
     * !! Only when we aren't in power save mode...
     */
    if (m_logic->PSMValue () == false)
        m_PSMAutoButton->setChecked (m_logic->PSMAutoValue());
}

void 
BatteryWidget::addHeaderContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonXLargeHeaderPanelInverted");
    container->setHeaderVisible (false);
    container->setContentsMargins (0,0,0,0);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);
    container->centralWidget()->setLayout (layout);

    /*
     * The label that we use as title.
     */
    //% "Battery"
    m_TitleLabel = new MLabel (qtTrId("qtn_ener_battery"));
    m_TitleLabel->setStyleName ("CommonXLargeHeaderInverted");
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
    
    m_BatteryImage = new BatteryImage;
    m_BatteryImage->setStyleName ("CommonMainIcon");

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
    m_ActivationContainer->setContentsMargins (0,0,0,0);
    m_ActivationContainer->setStyleName ("CommonPanelInverted");
    m_ActivationContainer->setHeaderVisible (false);
    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);
    layout->setSpacing (0.);

    m_ActivationContainer->centralWidget()->setLayout (layout);

    /*
     * A label for the PSM auto activation.
     */
    //% "Auto activate power save"
    activationLevelLabel = new MLabel(qtTrId ("qtn_ener_autops"));
    activationLevelLabel->setStyleName ("CommonSingleTitleInverted");
    activationLevelLabel->setObjectName ("activationLevel");

    /*
     * A help button for the PSM auto activation.
     */
    MHelpButton* helpButton = new MHelpButton ("IDUG_MEEGO_BATTERY.html");
    helpButton->setViewType(MButton::iconType);
    helpButton->setIconID ("icon-m-content-description");

    /*
     * A switch that turns the auto PSM mode on and off
     */
    m_PSMAutoButton = new MButton;
    m_PSMAutoButton->setStyleName ("CommonSwitchInverted");
    m_PSMAutoButton->setObjectName ("AutoActivatePowerSaveButton");
    m_PSMAutoButton->setCheckable (true);
    m_PSMAutoButton->setViewType (MButton::switchType);

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

    m_SliderContainer = new SliderContainer (this);
    showSlider (m_PSMAutoButton->isChecked ());
}

void 
BatteryWidget::showSlider (
        bool   show)
{
    MContainer *container;

    if (!m_SliderContainer || !m_MainLayout)
        return;

    if (show) {
        container = m_SliderContainer->labelContainer();

        if (m_MainLayout->indexOf(container) == -1) {
            m_MainLayout->insertItem (LabelContainerPosition, container);
            m_MainLayout->setStretchFactor (container, 0);
            container->show ();
        }
        
        container = m_SliderContainer->sliderContainer();

        if (m_MainLayout->indexOf(container) == -1) {
            m_MainLayout->insertItem (SliderContainerPosition, container);
            m_MainLayout->setStretchFactor (container, 0);
            container->show ();
        }
    } else {
        container = m_SliderContainer->labelContainer();

        if (m_MainLayout->indexOf(container) != -1) {
            container->hide ();
            m_MainLayout->removeAt(m_MainLayout->indexOf(container));
        }
        
        container = m_SliderContainer->sliderContainer();

        if (m_MainLayout->indexOf(container) != -1) {
            container->hide ();
            m_MainLayout->removeAt(m_MainLayout->indexOf(container));
        }
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
    container->setContentsMargins (0,0,0,0);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);
    container->centralWidget()->setLayout (layout);

    /*
     * PSMButton is used to immediately turn the power save mode on/off.
     */
    PSMButton = new MButton;
    PSMButton->setStyleName ("CommonSingleButtonInverted");
    PSMButton->setObjectName ("PSMButton");
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

void 
BatteryWidget::addBatteryConditionWidget ()
{
    MContainer            *container;
    QGraphicsLinearLayout *containerLayout =
        new QGraphicsLinearLayout (Qt::Vertical);
    containerLayout->setContentsMargins (0,0,0,0);
    containerLayout->setSpacing (0);

    /*
     * Creating a container and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonPanelInverted");
    container->setHeaderVisible (false);
    container->setContentsMargins (0,0,0,0);
    container->centralWidget()->setLayout (containerLayout);

    QGraphicsLinearLayout *layout;
    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);
    layout->setSpacing (0);

    containerLayout->addItem (layout);

    //% "Battery condition"
    MLabel *keyLabel = new MLabel (qtTrId ("qtn_ener_battery_condition"));
    keyLabel->setStyleName ("CommonSingleTitleInverted");
    keyLabel->setAlignment (Qt::AlignLeft | Qt::AlignVCenter);
    layout->addItem (keyLabel);
    layout->setStretchFactor (keyLabel, 1);

    MLabel *valueLabel = new MLabel;
    valueLabel->setStyleName ("CommonSubTitleInverted");
    valueLabel->setAlignment (Qt::AlignLeft | Qt::AlignVCenter);
    layout->addItem (valueLabel);
    layout->setStretchFactor (valueLabel, 2);

    switch (m_logic->getCondition ())
    {
        case BatteryBusinessLogic::BGood:
        case BatteryBusinessLogic::BUnknown:
            //% "Good"
            valueLabel->setText (qtTrId ("qtn_ener_condition_good"));
            break;
        case BatteryBusinessLogic::BPoor:
            //% "Poor"
            valueLabel->setText (qtTrId ("qtn_ener_condition_poor"));
            break;
    }

    if (m_logic->getCondition () == BatteryBusinessLogic::BPoor)
    {
        QGraphicsLinearLayout *poorLayout =
            new QGraphicsLinearLayout (Qt::Horizontal);
        poorLayout->setContentsMargins (0,0,0,0);
        poorLayout->setSpacing (0);

        containerLayout->addItem (poorLayout);

        MImageWidget *poorIcon = new MImageWidget;
        poorIcon->setStyleName ("CommonMainIcon");
        poorIcon->setImage ("icon-m-energy-management-insufficient-power");
        poorLayout->addItem (poorIcon);

        MLabel *poorInfo = new MLabel;
        //% "The battery capacity has decreased. You can
        // improve battery life by purchasing a new battery."
        poorInfo->setText (qtTrId ("qtn_ener_condition_poor_info"));
        poorInfo->setAlignment (Qt::AlignLeft);
        poorInfo->setStyleName ("CommonSubTextInverted");
        poorInfo->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);
        poorInfo->setWordWrap (true);
        poorLayout->addItem (poorInfo);
    }

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}


MSeparator *
BatteryWidget::addSpacer (
        const QString &styleName, 
        int            index)
{
    MSeparator   *spacer;

    Q_ASSERT (m_MainLayout);

    spacer = new MSeparator;
    spacer->setStyleName (styleName + QString::number(index));
    m_MainLayout->insertItem (index, spacer);
    m_MainLayout->setStretchFactor (spacer, 0);

    return spacer;
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

        if (PSMAutoEnabled) {
            /*
             * QmSystem returns 0 when PSMAuto is disabled,
             * so when we're enabling it, we've to re-query
             * the proper value
             */
            m_SliderContainer->updateSlider (m_logic->PSMThresholdValue ());
            showSlider (true);
        } else {
            showSlider (false);
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
                m_PSMAutoButton->setChecked (m_logic->PSMAutoValue());
                m_MainLayout->insertItem (
                        ActivationContainerPosition, m_ActivationContainer);
                m_MainLayout->setStretchFactor (m_ActivationContainer, 0);
            }
            m_logic->remainingCapacityRequired();
        } else {
            m_PSMAutoButton->setChecked(false);
            showSlider (false);
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
