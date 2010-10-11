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
#undef WANT_HANDLE_LABEL

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

DisplayWidget::DisplayWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        m_logic (NULL)
{
    setReferer (DcpDisplay::None);
    setContentsMargins (0, 0, 0, 0);
    initWidget ();
}

DisplayWidget::~DisplayWidget ()
{

}

void DisplayWidget::initWidget ()
{
    SYS_DEBUG ("");

    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->setContentsMargins (0., 0., 0., 0.);
    mainLayout->setSpacing (0.); 

    m_logic = new DisplayBusinessLogic;

    /*
     * First row: The brighness label and slider.
     */
    // Brightness
    MWidget *brightness = new MStylableWidget;
    QGraphicsLinearLayout *brightnessLayout =
        new QGraphicsLinearLayout (Qt::Vertical);
    brightnessLayout->setContentsMargins (0., 0., 0., 0.);

    //% "Brightness"
    m_brightnessLabel = new MLabel (qtTrId ("qtn_disp_bright"));
    m_brightnessLabel->setObjectName ("CommonTitle");

    m_brightnessSlider = new MSlider;
    m_brightnessSlider->setObjectName("CommonSlider");
#ifdef WANT_HANDLE_LABEL
    m_brightnessSlider->setHandleLabelVisible (true);
#endif
    m_brightnessSlider->setSizePolicy (QSizePolicy::Ignored, QSizePolicy::Preferred);

    brightnessLayout->addItem (m_brightnessLabel);
    brightnessLayout->setAlignment (m_brightnessLabel, Qt::AlignVCenter);

    brightnessLayout->addItem (m_brightnessSlider);
    brightnessLayout->setAlignment (m_brightnessSlider, Qt::AlignVCenter);

    m_brightness_vals = m_logic->brightnessValues ();
    m_brightnessSlider->setRange (0, m_brightness_vals.size () - 1);
    m_brightnessSlider->setValue (m_logic->selectedBrightnessValueIndex ());

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             SLOT (sliderUpdated (int)));

    brightness->setObjectName ("CommonPanel");
    brightness->setLayout (brightnessLayout);
    mainLayout->addItem (brightness);

    /*
     * Second row: The available backlight time out values.
     */
    m_screenlight_vals = m_logic->screenLightsValues ();

    // Screen dim timeout selector
    m_screenTimeout = new MBasicListItem (MBasicListItem::TitleWithSubtitle);
    m_screenTimeout->setObjectName ("CommonPanel");
    m_screenTimeout->setLayoutPosition (M::VerticalCenterPosition);
    //% "Backlight time out"
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

    mainLayout->addItem (m_screenTimeout);

    /*
     * The third row: stays lit when charging label and button.
     */
    MWidget *displayon = new MStylableWidget;
    QGraphicsLinearLayout *blankinhibitLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    blankinhibitLayout->setContentsMargins (0., 0., 0., 0.);

    //% "Display stays lit when charging"
    m_blankInhibitLabel = new MLabel (qtTrId ("qtn_disp_screenon"));
    m_blankInhibitLabel->setObjectName("CommonSingleTitle");
    blankinhibitLayout->addItem (m_blankInhibitLabel);

    // Blank inhibit
    m_blankInhibitButton = new MButton;
    m_blankInhibitButton->setObjectName("CommonSwitch");
    m_blankInhibitButton->setCheckable (true);
    m_blankInhibitButton->setViewType (MButton::switchType);

    blankinhibitLayout->addItem (m_blankInhibitButton);
    blankinhibitLayout->setAlignment (m_blankInhibitButton, Qt::AlignVCenter);

    connect (m_blankInhibitButton, SIGNAL (toggled (bool)),
             m_logic, SLOT (setBlankInhibitValue (bool)));

    m_blankInhibitButton->setChecked (m_logic->blankInhibitValue ());

    displayon->setObjectName ("CommonPanel");
    displayon->setLayout (blankinhibitLayout);
    mainLayout->addItem (displayon);

    mainLayout->addStretch ();

    setLayout (mainLayout);
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

    m_screenTimeout->setTitle (qtTrId ("qtn_disp_screenoff"));
    m_brightnessLabel->setText (qtTrId ("qtn_disp_bright"));
    m_blankInhibitLabel->setText (qtTrId ("qtn_disp_screenon"));
}

