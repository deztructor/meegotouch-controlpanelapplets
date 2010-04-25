/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaywidget.h"
#include "displaybusinesslogic.h"
#include "dcpdisplay.h"

#include <QDebug>

#include <MButton>
#include <MComboBox>
#include <MContainer>
#include <MLabel>
#include <MLayout>
#include <MSlider>
#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>

DisplayWidget::DisplayWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        m_logic (NULL)
{
    setReferer (DcpDisplay::None);
    initWidget ();
}

DisplayWidget::~DisplayWidget ()
{

}

void DisplayWidget::initWidget ()
{
    MLayout               *mainLayout;
    MLinearLayoutPolicy   *policy;

    mainLayout = new MLayout;
    policy     = new MLinearLayoutPolicy (mainLayout, Qt::Vertical);
    mainLayout->setPolicy (policy);

    m_logic = new DisplayBusinessLogic();

    // Brightness
    QGraphicsLinearLayout *brightnessLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    //% "Brightness"
    m_brightnessLabel = new MLabel (qtTrId ("qtn_disp_bright"));
    brightnessLayout->addItem (m_brightnessLabel);
    brightnessLayout->setStretchFactor (m_brightnessLabel, 2);

    m_brightnessSlider = new MSlider;
    brightnessLayout->addItem (m_brightnessSlider);
    brightnessLayout->setStretchFactor (m_brightnessSlider, 2);

    m_brightness_vals = m_logic->brightnessValues ();
    m_brightnessSlider->setRange (0, m_brightness_vals.size () - 1);
    m_brightnessSlider->setValue (m_logic->selectedBrightnessValue ());

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setBrightnessValue (int)));

    policy->addItem (brightnessLayout);

    // Screen light
    m_screenlightCombo = new MComboBox;
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
        m_screenlightCombo->insertItem (i, str);
    }
    m_screenlightCombo->setCurrentIndex (m_logic->selectedScreenLightsValue ());

    connect (m_screenlightCombo, SIGNAL (currentIndexChanged (int)),
             this, SLOT (setScreenLightTimeouts (int)));

    policy->addItem (m_screenlightCombo);

    QGraphicsLinearLayout *blankinhibitLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    //% "Display stays lit when charging"
    m_blankInhibitLabel = new MLabel (qtTrId ("qtn_disp_screenon"));
    blankinhibitLayout->addItem (m_blankInhibitLabel);

    // Blank inhibit
    MButton *blankInhibitButton = new MButton;
    blankInhibitButton->setCheckable (true);
    blankInhibitButton->setViewType (MButton::switchType);

    blankinhibitLayout->addItem (blankInhibitButton);

    connect (blankInhibitButton, SIGNAL (toggled (bool)),
             m_logic, SLOT (setBlankInhibitValue (bool)));

    blankInhibitButton->setChecked (m_logic->blankInhibitValue ());

    policy->addItem (blankinhibitLayout);

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
        if (value => 60)
            value /= 60;
        m_screenlightCombo->setItemText (i, str.arg (value));
    }

    m_brightnessLabel->setText (qtTrId ("qtn_disp_bright"));
    m_blankInhibitLabel->setText (qtTrId ("qtn_disp_screenon"));
}

