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
#include "displaywidget.h"
#include "displaybusinesslogic.h"
#include "dcpdisplay.h"

#undef DEBUG
#define WARNING
#include "../debug.h"

// Define this if you want handle-label for brightness-slider
// XXX: for now it looks very ugly... it is visible over the dialogs :-S
#define WANT_HANDLE_LABEL

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
        m_screenlightLabel (0)
{
    setReferer (DcpDisplay::None);
    setContentsMargins (0, 0, 0, 0);
    m_logic = new DisplayBusinessLogic;

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

    // Row 1: The title label
    addHeaderContainer ();
    addStretcher ("CommonSmallSpacerInverted");
    addSecHeaderContainer ();
    addSliderContainer ();
    addStretcher ("CommonItemDivider");
    addScreenTimeoutContainer ();
    addStretcher ("CommonSmallSpacerInverted");
    m_MainLayout->addStretch();
}

void 
DisplayWidget::addHeaderContainer ()
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
    container->centralWidget()->setLayout (layout);
    layout->setContentsMargins (0,0,0,0);

    /*
     * The label that we use as title.
     */
    //% "Display"
    m_TitleLabel = new MLabel (qtTrId("qtn_disp_display"));
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
DisplayWidget::addSecHeaderContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonGroupHeaderPanelInverted");

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
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonLargePanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * The slider to set the brighness of the display.
     */
    m_brightnessSlider = new MSlider;
    m_brightnessSlider->setStyleName ("CommonSliderInverted");
    m_brightnessSlider->setObjectName ("BrightnessSlider");
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
}

void 
DisplayWidget::addScreenTimeoutContainer ()
{
    Q_ASSERT (m_MainLayout);

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
DisplayWidget::updateScreenTimeoutCombo ()
{
    bool fillNeeded = (m_screenTimeout->count () == 0);

    for (int i = 0; i < m_screenlight_vals.size (); i++)
    {
        int value = m_screenlight_vals.at (i);
        QString str = (value < 60) ?
                      //% "%1 seconds"
                      qtTrId ("qtn_comm_time_second", value) :
                      //% "%1 minutes"
                      qtTrId ("qtn_comm_time_minute", value / 60);
        if (value >= 60)
            value /= 60;

        if (fillNeeded)
            m_screenTimeout->insertItem (i, str.arg (value));
        else
            m_screenTimeout->setItemText (i, str.arg (value));
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

    Q_ASSERT (m_MainLayout);

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

    m_TitleLabel->setText (qtTrId("qtn_disp_display"));
    m_SubTitleLabel->setText (qtTrId("qtn_disp_bright"));
    m_screenTimeout->setTitle (qtTrId ("qtn_disp_screenoff"));
}

