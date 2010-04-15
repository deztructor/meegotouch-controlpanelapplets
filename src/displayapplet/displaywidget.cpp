/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaywidget.h"
#include "displaybusinesslogic.h"
#include "dcpdisplay.h"

#include <QDebug>

#include <MButton>
#include <MContainer>
#include <MLabel>
#include <MLayout>
#include <MSlider>
#include <MLinearLayoutPolicy>

DisplayWidget::DisplayWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        m_logic(NULL)
{
    setReferer(DcpDisplay::None);
    initWidget();
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::initWidget()
{
    MLayout               *mainLayout;
    MLinearLayoutPolicy   *policy;

    mainLayout = new MLayout (this);
    policy     = new MLinearLayoutPolicy (mainLayout, Qt::Vertical);

    policy->setSpacing (10);
    mainLayout->setPolicy (policy);
   
    m_logic = new DisplayBusinessLogic();

    // Brightness
    MLayout* brightnessLayout = new MLayout;

    MLinearLayoutPolicy* brightnessLayoutPolicy =
        new MLinearLayoutPolicy (brightnessLayout, Qt::Horizontal);
    brightnessLayoutPolicy->setSpacing (0);
    brightnessLayoutPolicy->setContentsMargins (0, 0, 0, 0);

    m_brightnessSlider = new MSlider;
    brightnessLayoutPolicy->addItem (m_brightnessSlider);

    m_brightness_vals = m_logic->brightnessValues ();
    m_brightnessSlider->setRange (0, m_brightness_vals.size () - 1);
    m_brightnessSlider->setValue (m_logic->selectedBrightnessValue ());

// FIXME: now its casues crash on the device :-S <dkedves>
#if 0
    m_brightnessSlider->setMinLabelVisible (true);
    m_brightnessSlider->setMaxLabelVisible (true);
    m_brightnessSlider->setMinLabelIconID ("icon-l-brightness-min");
    m_brightnessSlider->setMaxLabelIconID ("icon-l-brightness-max");
#endif

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setBrightnessValue (int)));

    m_brightnessContainer = new MContainer;
    //% "Brightness"
    m_brightnessContainer->setTitle (qtTrId ("qtn_disp_bright"));
    m_brightnessContainer->centralWidget ()->setLayout (brightnessLayout);

    policy->addItem (m_brightnessContainer);

    // Screen light
    MLayout* screenlightLayout = new MLayout;
    MLinearLayoutPolicy* screenlightLayoutPolicy =
        new MLinearLayoutPolicy(screenlightLayout, Qt::Vertical);
    screenlightLayoutPolicy->setSpacing(0);
    screenlightLayoutPolicy->setContentsMargins(0, 0, 0, 0);

    m_screenlightSlider = new MSlider;
    screenlightLayoutPolicy->addItem (m_screenlightSlider);

    m_screenlight_vals = m_logic->screenLightsValues ();
    m_screenlightSlider->setRange (0, m_screenlight_vals.size () - 1);
    m_screenlightSlider->setValue (m_logic->selectedScreenLightsValue ()); 

#if 0
    m_screenlightSlider->setMinLabelVisible (true);
    m_screenlightSlider->setMaxLabelVisible (true);
    m_screenlightSlider->setMinLabelIconID ("icon-l-screenlight-min?");
    m_screenlightSlider->setMaxLabelIconID ("icon-l-screenlight-max?");
#endif

    connect (m_screenlightSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setScreenLightTimeouts (int)));
    connect (m_screenlightSlider, SIGNAL (valueChanged (int)),
             this, SLOT (modify_screenlight_handle (int)));

    m_screenlightContainer = new MContainer;
    //% "Backlight time-out" 
    m_screenlightContainer->setTitle (qtTrId ("qtn_disp_screenoff"));
    m_screenlightContainer->centralWidget ()->setLayout (screenlightLayout);

    modify_screenlight_handle (m_logic->selectedScreenLightsValue ());
    m_screenlightSlider->setHandleLabelVisible (true);

    policy->addItem (m_screenlightContainer);

    // Blank inhibit
    MButton *blankInhibitButton = new MButton;
    blankInhibitButton->setCheckable (true);
    blankInhibitButton->setViewType (MButton::switchType);

    MLayout *blankInhibitLayout = new MLayout;
    MLinearLayoutPolicy *blankInhibitLayoutPolicy =
        new MLinearLayoutPolicy (blankInhibitLayout, Qt::Horizontal);

    m_OnOffLabel = new MLabel;
    m_OnOffLabel->setObjectName("BlankInhibitStateLabel");

    blankInhibitLayoutPolicy->addItem (m_OnOffLabel, Qt::AlignLeft);
    blankInhibitLayoutPolicy->addItem (blankInhibitButton,
                                       Qt::AlignRight | Qt::AlignVCenter);

    m_blankInhibitContainer = new MContainer;
    //% "Display stays lit when charging"
    m_blankInhibitContainer->setTitle (qtTrId ("qtn_disp_screenon"));
    m_blankInhibitContainer->centralWidget ()->setLayout (blankInhibitLayout);

    connect (blankInhibitButton, SIGNAL (toggled (bool)),
             m_logic, SLOT (setBlankInhibitValue (bool)));
    connect (blankInhibitButton, SIGNAL (toggled (bool)),
             this, SLOT (update_onoff_label (bool)));

    blankInhibitButton->setChecked (m_logic->blankInhibitValue());
    update_onoff_label(m_logic->blankInhibitValue());

    policy->addItem(m_blankInhibitContainer, Qt::AlignLeft);
}

void
DisplayWidget::update_onoff_label (bool value)
{
    QString text = value ?
        //% "On"
        qtTrId ("qtn_comm_settings_on") :
        //% "Off"
        qtTrId ("qtn_comm_settings_off");

    m_OnOffLabel->setText (text);
}

void
DisplayWidget::modify_screenlight_handle (int newValue)
{
    int value = m_screenlight_vals.at (newValue);
    QString str = (value < 60) ?
                  //% "%1 seconds"
                  qtTrId ("qtn_comm_time_second") :
                  //% "%1 minutes"
                  qtTrId ("qtn_comm_time_minute");

    if (value >= 60)
        value /= 60;

    m_screenlightSlider->setHandleLabel (str.arg (value));
}

void
DisplayWidget::retranslateUi ()
{
    modify_screenlight_handle (m_screenlightSlider->value ());

    m_brightnessContainer->setTitle (qtTrId ("qtn_disp_bright"));
    m_blankInhibitContainer->setTitle (qtTrId ("qtn_disp_screenon"));
}

