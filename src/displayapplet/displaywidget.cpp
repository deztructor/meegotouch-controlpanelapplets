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
#include "../debug.h"

// Define this if you want handle-label for brightness-slider
// XXX: for now it looks very ugly... it is visible over the dialogs :-S
#define WANT_HANDLE_LABEL

#include <MButton>
#include <MContainer>
#include <MBasicListItem>
#include <MPopupList>
#include <QStringList>
#include <QStringListModel>
#include <MLabel>
#include <MLayout>
#include <MSlider>
#include <MWidget>
#include <MStylableWidget>
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
        m_blankInhibitButton (0),
        m_screenlightLabel (0),
        m_blankInhibitLabel (0)
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
    addBlankInhibitContainer ();
    addStretcher ("CommonSmallSpacerInverted");
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

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

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

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

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
     * 
     */
    m_screenlight_vals = m_logic->screenLightsValues ();

    // Screen dim timeout selector
    m_screenTimeout = new MBasicListItem (MBasicListItem::TitleWithSubtitle);
    m_screenTimeout->setStyleName ("CommonPanelInverted");
    // FIXME: These are unfortunately protected, so I can't modify them!
    // MLabel * 	titleLabelWidget ()
    // MLabel * 	subtitleLabelWidget ()
    m_screenTimeout->setLayoutPosition (M::VerticalCenterPosition);
    //% "Backlight time out"
    // FIXME: This should go into a separate method.
    m_screenTimeout->setTitle (qtTrId ("qtn_disp_screenoff"));
    {
        int value = m_screenlight_vals.at (
                        m_logic->selectedScreenLightsValue ());
        QString str = (value < 60) ?
                      //% "%1 seconds"
                      qtTrId ("qtn_comm_time_second", value) :
                      //% "%1 minutes"
                      qtTrId ("qtn_comm_time_minute", value / 60);
        if (value >= 60)
            value /= 60;
        m_screenTimeout->setSubtitle (str.arg (value));
    }
    connect (m_screenTimeout, SIGNAL (clicked ()),
             this, SLOT (screenTimeoutClicked ()));
    
    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (m_screenTimeout);
    m_MainLayout->setStretchFactor (m_screenTimeout, 0);
}

void 
DisplayWidget::addBlankInhibitContainer ()
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (m_MainLayout);
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonPanelInverted");
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    container->centralWidget()->setLayout (layout);

    /*
     * 
     */
    //% "Display stays lit when charging"
    m_blankInhibitLabel = new MLabel (qtTrId ("qtn_disp_screenon"));
    m_blankInhibitLabel->setStyleName("CommonSingleTitleInverted");

    // Blank inhibit
    m_blankInhibitButton = new MButton;
    m_blankInhibitButton->setStyleName ("CommonSwitchInverted");
    m_blankInhibitButton->setObjectName ("BlankInhibitButton");
    m_blankInhibitButton->setCheckable (true);
    m_blankInhibitButton->setViewType (MButton::switchType);

    connect (m_blankInhibitButton, SIGNAL (toggled (bool)),
             m_logic, SLOT (setBlankInhibitValue (bool)));

    m_blankInhibitButton->setChecked (m_logic->blankInhibitValue ());

    /*
     *
     */
    layout->addItem (m_blankInhibitLabel);
    layout->addItem (m_blankInhibitButton);
    layout->setAlignment (m_blankInhibitButton, Qt::AlignVCenter);

    /*
     * Adding the whole row to the main container.
     */
    m_MainLayout->addItem (container);
    m_MainLayout->setStretchFactor (container, 0);
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
DisplayWidget::screenTimeoutClicked ()
{
    MPopupList *popuplist = new MPopupList;
    QStringListModel *model = new QStringListModel;
    QStringList timeoutVals;

    popuplist->setTitle (qtTrId ("qtn_disp_screenoff"));
    popuplist->setButtonBoxVisible (false);

    // FIXME: This should go into a separate method.
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

        timeoutVals << str.arg (value);
    }

    model->setStringList (timeoutVals);

    popuplist->setItemModel (model);

    const QModelIndex selected (
        model->index (m_logic->selectedScreenLightsValue ()));

    popuplist->setCurrentIndex (selected);
    popuplist->scrollTo (selected);

    if ((popuplist->exec () == MDialog::Accepted)
        && popuplist->currentIndex ().isValid ())
    {
        int newIndex = popuplist->currentIndex ().row ();
        SYS_DEBUG ("Selected: %d", newIndex);

        m_screenTimeout->setSubtitle (timeoutVals.at (newIndex));
        m_logic->setScreenLightTimeouts (newIndex);
    }

    popuplist->deleteLater ();
}

void
DisplayWidget::retranslateUi ()
{
    // FIXME: This should go into a separate method.
    {
        int value = m_screenlight_vals.at (
                        m_logic->selectedScreenLightsValue ());
        QString str = (value < 60) ?
                      //% "%1 seconds"
                      qtTrId ("qtn_comm_time_second", value) :
                      //% "%1 minutes"
                      qtTrId ("qtn_comm_time_minute", value / 60);
        if (value >= 60)
            value /= 60;
        m_screenTimeout->setSubtitle (str.arg (value));
    }

    m_TitleLabel->setText (qtTrId("qtn_disp_display"));
    m_SubTitleLabel->setText (qtTrId("qtn_disp_bright"));
    m_screenTimeout->setTitle (qtTrId ("qtn_disp_screenoff"));
    m_blankInhibitLabel->setText (qtTrId ("qtn_disp_screenon"));
}

