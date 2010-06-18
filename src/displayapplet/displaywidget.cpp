/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaywidget.h"
#include "displaybusinesslogic.h"
#include "dcpdisplay.h"

#undef DEBUG
#include "../debug.h"

#include <MButton>
#include <MComboBox>
#include <MContainer>
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
    mainLayout->setSpacing (0.); 

    m_logic = new DisplayBusinessLogic;

    // Brightness
    MWidget *brightness = new MStylableWidget;
    MLayout *brightnessLayout = new MLayout;

    MLinearLayoutPolicy *brightnessLandscape =
        new MLinearLayoutPolicy (brightnessLayout, Qt::Horizontal);

    MLinearLayoutPolicy *brightnessPortrait =
        new MLinearLayoutPolicy (brightnessLayout, Qt::Vertical);

    //% "Brightness"
    m_brightnessLabel = new MLabel (qtTrId ("qtn_disp_bright"));
    m_brightnessLabel->setObjectName ("CommonSingleTitle");
    brightnessLandscape->addItem (m_brightnessLabel);
    brightnessPortrait->addItem (m_brightnessLabel);

    m_brightnessSlider = new MSlider;
    m_brightnessSlider->setObjectName("BrightnessSlider");
    brightnessLandscape->addItem (m_brightnessSlider);
    brightnessLandscape->setAlignment (m_brightnessSlider, Qt::AlignVCenter);
    brightnessPortrait->addItem (m_brightnessSlider);

    m_brightness_vals = m_logic->brightnessValues ();
    m_brightnessSlider->setRange (0, m_brightness_vals.size () - 1);
    m_brightnessSlider->setValue (m_logic->selectedBrightnessValue ());

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setBrightnessValue (int)));

    brightnessLayout->setLandscapePolicy (brightnessLandscape);
    brightnessLayout->setPortraitPolicy (brightnessPortrait);

    brightness->setObjectName ("CommonPanel");
    brightness->setLayout (brightnessLayout);
    mainLayout->addItem (brightness);

    // Screen light
    m_screenlightCombo = new MComboBox;
    m_screenlightCombo->setObjectName ("ScreenlightCombo");
    m_screenlightCombo->setContentsMargins (0, 0, 0, 0);
    //% "Backlight time out"
    m_screenlightCombo->setTitle (qtTrId ("qtn_disp_screenoff"));

    m_screenlight_vals = m_logic->screenLightsValues ();

    for (int i = 0; i < m_screenlight_vals.size (); i++)
    {
        int value = m_screenlight_vals.at (i);
        QString str = (value < 60) ?
                      //% "%1 seconds"
                      qtTrId ("qtn_comm_time_second") :
                      //% "%1 minutes"
                      qtTrId ("qtn_comm_time_minute");
        if (value >= 60)
            value /= 60;
        SYS_DEBUG ("ADD: NUM = %d, STR = %s", value, SYS_STR (str.arg (value)));
        m_screenlightCombo->insertItem (i, str.arg (value));
    }
    m_screenlightCombo->setCurrentIndex (m_logic->selectedScreenLightsValue ());

    connect (m_screenlightCombo, SIGNAL (currentIndexChanged (int)),
             m_logic, SLOT (setScreenLightTimeouts (int)));

    mainLayout->addItem (m_screenlightCombo);

    MWidget *displayon = new MStylableWidget;
    QGraphicsLinearLayout *blankinhibitLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    //% "Display stays lit when charging"
    m_blankInhibitLabel = new MLabel (qtTrId ("qtn_disp_screenon"));
    m_blankInhibitLabel->setObjectName("CommonSingleTitle");
    blankinhibitLayout->addItem (m_blankInhibitLabel);

    // Blank inhibit
    MButton *blankInhibitButton = new MButton;
    blankInhibitButton->setObjectName("BlankInhibitButton");
    blankInhibitButton->setCheckable (true);
    blankInhibitButton->setViewType (MButton::switchType);

    blankinhibitLayout->addItem (blankInhibitButton);
    blankinhibitLayout->setAlignment (blankInhibitButton, Qt::AlignVCenter);

    connect (blankInhibitButton, SIGNAL (toggled (bool)),
             m_logic, SLOT (setBlankInhibitValue (bool)));

    blankInhibitButton->setChecked (m_logic->blankInhibitValue ());

    displayon->setObjectName ("CommonPanel");
    displayon->setLayout (blankinhibitLayout);
    mainLayout->addItem (displayon);

    mainLayout->addStretch ();

    setLayout (mainLayout);
}

void
DisplayWidget::retranslateUi ()
{
    for (int i = 0; i < m_screenlight_vals.size (); i++)
    {
        int value = m_screenlight_vals.at (i);
        QString str = (value < 60) ?
                      //% "%1 seconds"
                      qtTrId ("qtn_comm_time_second") :
                      //% "%1 minutes"
                      qtTrId ("qtn_comm_time_minute");
        if (value >= 60)
            value /= 60;
        m_screenlightCombo->setItemText (i, str.arg (value));
    }

    m_brightnessLabel->setText (qtTrId ("qtn_disp_bright"));
    m_blankInhibitLabel->setText (qtTrId ("qtn_disp_screenon"));
}

