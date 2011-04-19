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
#include "displaywidget.h"
#include "displaybusinesslogic.h"
#include "dcpdisplay.h"
#include "../styles.h"

#undef DEBUG
#define WARNING
#include "../debug.h"

// Define this if you want handle-label for brightness-slider
// XXX: for now it looks very ugly... it is visible over the dialogs :-S
//#define WANT_HANDLE_LABEL

#include <MButton>
#include <MContainer>
#include <MComboBox>
#include <MLabel>
#include <MLayout>
#include <MSlider>
#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MSeparator>

DisplayWidget::DisplayWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        m_logic (0),
        m_MainLayout (0),
        m_TitleLabel (0),
        m_SubTitleLabel (0),
        m_brightnessSlider (0),
        m_screenTimeout (0),
        m_screenlightLabel (0),
        m_lowPowerSwitch (0),
        m_DoubleTapSwitch (0)
{
    setReferer (DcpDisplay::None);
    setContentsMargins (0, 0, 0, 0);
    m_logic = new DisplayBusinessLogic;

    connect (m_logic, SIGNAL(lowPowerModeChanged(bool)),
            this, SLOT(lowPowerModeChanged(bool)));
    connect (m_logic, SIGNAL(doubleTapModeChanged(bool)),
            this, SLOT(doubleTapModeChanged(bool)));

    initWidget ();
}

DisplayWidget::~DisplayWidget ()
{
}

void DisplayWidget::initWidget ()
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
     * FIXME: I removed these widgets because the layout guide has been changed
     * (what an unexpected thing). If the layout guide remains we will have to
     * remove the methods themselves.
     */
    // Row 1: The title label
    addHeaderContainer ();
    #ifdef APP_TITLE_USES_SPACER
    addStretcher (APP_TITLE_DIVIDER_STYLE_NAME);
    #endif
    //addSecHeaderContainer ();
    //addSliderContainer ();
    //addStretcher ("CommonItemDivider");
    addScreenTimeoutContainer ();
    //addStretcher ("CommonItemDivider");
    addLowPowerContainer ();
    addDoubleTapContainer ();
    //addStretcher ("CommonSmallSpacerInverted");
    m_MainLayout->addStretch();
}

void
DisplayWidget::addHeaderContainer ()
{
    QGraphicsLinearLayout *layout;

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);

    /*
     * The label that we use as title.
     */
    //% "Display"
    m_TitleLabel = new MLabel (qtTrId ("qtn_disp_display"));
    m_TitleLabel->setObjectName ("TitleLabel");
    m_TitleLabel->setStyleName (APP_TITLE_LABEL_STYLE_NAME);
    layout->addItem (m_TitleLabel);
    layout->setAlignment (m_TitleLabel, Qt::AlignLeft);
    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (layout);
    m_MainLayout->setStretchFactor (layout, 0);
}


void 
DisplayWidget::addSecHeaderContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonHeaderPanelInverted");

    container->setHeaderVisible (false);
    container->setContentsMargins (0,0,0,0);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);
    layout->setContentsMargins (0,0,0,0);

    /*
     * The label that we use as title.
     */
    //% "Brightness"
    m_SubTitleLabel = new MLabel (qtTrId("qtn_disp_bright"));
    m_SubTitleLabel->setObjectName ("SubTitleLabel");
    m_SubTitleLabel->setStyleName ("CommonGroupHeaderInverted");
    layout->addItem (m_SubTitleLabel);
    layout->setAlignment (m_SubTitleLabel, Qt::AlignLeft);
    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}

void 
DisplayWidget::addSliderContainer ()
{
#if 0
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setContentsMargins (0,0,0,0);
    container->setStyleName ("CommonLargePanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);
    container->centralWidget()->setLayout (layout);

    /*
     * The slider to set the brighness of the display.
     */
    m_brightnessSlider = new MSlider;
    m_brightnessSlider->setContentsMargins (0,0,0,0);
    m_brightnessSlider->setStyleName ("CommonSliderInverted");
    m_brightnessSlider->setObjectName ("BrightnessSlider");
    m_brightnessSlider->setMinLabelIconID ("icon-s-image-edit-low-brightness");
    m_brightnessSlider->setMinLabelVisible (true);
    m_brightnessSlider->setMaxLabelIconID ("icon-s-image-edit-high-brightness");
    m_brightnessSlider->setMaxLabelVisible (true);
    m_brightness_vals = m_logic->brightnessValues ();
    m_brightnessSlider->setRange (0, m_brightness_vals.size () - 1);
    m_brightnessSlider->setValue (m_logic->selectedBrightnessValueIndex ());

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             SLOT (sliderUpdated (int)));

#ifdef WANT_HANDLE_LABEL
    m_brightnessSlider->setHandleLabelVisible (true);
#endif

    /*
     * Adding the slider to the panel. Please note that the theme might set the
     * size of the slider, so we need stretchers to move the short slider to the
     * center and make the panel as wide as possible.
     */
    layout->addStretch ();
    layout->addItem (m_brightnessSlider);
    layout->addStretch ();
    //layout->setAlignment (m_brightnessSlider, Qt::AlignHCenter);
    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
#endif
}

void 
DisplayWidget::addScreenTimeoutContainer ()
{
    /*
     * Get the values from the business-logic
     */
    m_screenlight_vals = m_logic->screenLightsValues ();

    // Screen dim timeout selector
    m_screenTimeout = new MComboBox;

    //% "Backlight time out"
    m_screenTimeout->setTitle (qtTrId ("qtn_disp_screenoff"));
    m_screenTimeout->setStyleName ("CommonComboBoxInverted");
    m_screenTimeout->setLayoutPosition (M::VerticalCenterPosition);

    updateScreenTimeoutCombo ();

    connect (m_screenTimeout, SIGNAL (currentIndexChanged (int)),
             this, SLOT (screenTimeoutChanged (int)));

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (m_screenTimeout);
    m_MainLayout->setStretchFactor (m_screenTimeout, 0);
}

void 
DisplayWidget::addLowPowerContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setContentsMargins (0,0,0,0);
    container->setStyleName ("CommonLargePanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);
    container->centralWidget()->setLayout (layout);

    MLabel *lowPowerLabel = new MLabel;
    lowPowerLabel->setObjectName ("LowPowerLabel");
    lowPowerLabel->setStyleName ("CommonSingleTitleInverted");
    //% "Low power mode"
    lowPowerLabel->setText (qtTrId ("qtn_disp_lowpower"));
    layout->addItem (lowPowerLabel);
    layout->setAlignment (lowPowerLabel, Qt::AlignVCenter);

    m_lowPowerSwitch = new MButton;
    m_lowPowerSwitch->setCheckable (true);
    m_lowPowerSwitch->setViewType (MButton::switchType);
    m_lowPowerSwitch->setStyleName ("CommonRightSwitchInverted");
    m_lowPowerSwitch->setChecked (m_logic->getLowPowerMode ());

    connect (m_lowPowerSwitch, SIGNAL (toggled (bool)),
             m_logic, SLOT (setLowPowerMode (bool)));

    layout->addItem (m_lowPowerSwitch);
    layout->setAlignment (m_lowPowerSwitch, Qt::AlignVCenter | Qt::AlignRight);

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}

void 
DisplayWidget::addDoubleTapContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setContentsMargins (0,0,0,0);
    container->setStyleName ("CommonLargePanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0,0,0,0);
    container->centralWidget()->setLayout (layout);

    MLabel *lowPowerLabel = new MLabel;
    lowPowerLabel->setObjectName ("LowPowerLabel");
    lowPowerLabel->setStyleName ("CommonSingleTitleInverted");
    //% "Double tap wake-up"
    lowPowerLabel->setText (qtTrId ("qtn_disp_doubletap"));
    layout->addItem (lowPowerLabel);
    layout->setAlignment (lowPowerLabel, Qt::AlignVCenter);

    m_DoubleTapSwitch = new MButton;
    m_DoubleTapSwitch->setCheckable (true);
    m_DoubleTapSwitch->setViewType (MButton::switchType);
    m_DoubleTapSwitch->setStyleName ("CommonRightSwitchInverted");
    m_DoubleTapSwitch->setChecked (m_logic->getDoubleTapWakes());

    connect (m_DoubleTapSwitch, SIGNAL (toggled (bool)),
             m_logic, SLOT (setDoubleTapWakes(bool)));

    layout->addItem (m_DoubleTapSwitch);
    layout->setAlignment (m_DoubleTapSwitch, Qt::AlignVCenter | Qt::AlignRight);

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
}

void
DisplayWidget::updateScreenTimeoutCombo ()
{
    bool fillNeeded = (m_screenTimeout->count () == 0);

    for (int i = 0; i < m_screenlight_vals.size (); i++)
    {
        bool minute = false;
        int value = m_screenlight_vals.at (i);

        if (value >= 60)
        {
            minute = true;
            value /= 60;
        }

        QString itemText;
        if (minute)
        {
            //% "%1 minutes"
            itemText = qtTrId ("qtn_comm_time_minute", value).arg (value);
        }
        else
        {
            //% "%1 seconds"
            itemText = qtTrId ("qtn_comm_time_second", value).arg (value);
        }

        if (fillNeeded)
            m_screenTimeout->insertItem (i, itemText);
        else
            m_screenTimeout->setItemText (i, itemText);
    }

    if (fillNeeded)
    {
        /*
         * set the currently selected index
         */
        m_screenTimeout->setProperty ("currentIndex",
                                      m_logic->selectedScreenLightsValue ());
    }
}

void 
DisplayWidget::addStretcher (
        const QString &styleName)
{
    MSeparator *stretcher;

    stretcher = new MSeparator ();
    stretcher->setStyleName (styleName);
    m_MainLayout->addItem (stretcher);
}

void
DisplayWidget::sliderUpdated (int val)
{
    m_logic->setBrightnessValue (val);
#ifdef WANT_HANDLE_LABEL
    m_brightnessSlider->setHandleLabel (QString ("%1").arg (val));
#endif
}

void
DisplayWidget::screenTimeoutChanged (int index)
{
    SYS_DEBUG ("selected = %d (%s)", index, SYS_STR (m_screenTimeout->currentText ()));
    SYS_DEBUG ("new value = %d", m_screenlight_vals.at (index));

    m_logic->setScreenLightTimeouts (index);
}

void
DisplayWidget::retranslateUi ()
{
    updateScreenTimeoutCombo ();

    if (m_TitleLabel)
        m_TitleLabel->setText (qtTrId("qtn_disp_display"));

    if (m_SubTitleLabel)
        m_SubTitleLabel->setText (qtTrId("qtn_disp_bright"));

    if (m_screenTimeout)
        m_screenTimeout->setTitle (qtTrId ("qtn_disp_screenoff"));
}

void 
DisplayWidget::lowPowerModeChanged (
        bool lpm)
{
    SYS_DEBUG ("*** lpm = %s", SYS_BOOL(lpm));
    if (m_lowPowerSwitch)
        m_lowPowerSwitch->setChecked (lpm);
}

void 
DisplayWidget::doubleTapModeChanged (
        bool dt)
{
    SYS_DEBUG ("*** dt = %s", SYS_BOOL(dt));
    if (m_DoubleTapSwitch)
        m_DoubleTapSwitch->setChecked (dt);
}
