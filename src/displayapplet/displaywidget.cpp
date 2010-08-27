/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaywidget.h"
#include "displaybusinesslogic.h"
#include "dcpdisplay.h"

#define DEBUG
#include "../debug.h"

#include <MButton>
#include <MContainer>
#include <MContentItem>
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

    // Brightness
    MWidget *brightness = new MStylableWidget;
    MLayout *brightnessLayout = new MLayout;

    MLinearLayoutPolicy *brightnessLandscape =
        new MLinearLayoutPolicy (brightnessLayout, Qt::Horizontal);

    MLinearLayoutPolicy *brightnessPortrait =
        new MLinearLayoutPolicy (brightnessLayout, Qt::Vertical);

    //% "Brightness"
    m_brightnessLabel = new MLabel (qtTrId ("qtn_disp_bright"));
    m_brightnessLabel->setObjectName ("ContentItemTitle");
    brightnessLandscape->addItem (m_brightnessLabel);
    brightnessPortrait->addItem (m_brightnessLabel);

    m_brightnessSlider = new MSlider;
    m_brightnessSlider->setObjectName("BrightnessSlider");
    brightnessLandscape->addItem (m_brightnessSlider);
    brightnessLandscape->setAlignment (m_brightnessSlider, Qt::AlignVCenter);
    brightnessPortrait->addItem (m_brightnessSlider);

    m_brightness_vals = m_logic->brightnessValues ();
    m_brightnessSlider->setRange (0, m_brightness_vals.size () - 1);
    m_brightnessSlider->setValue (m_logic->selectedBrightnessValueIndex ());

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setBrightnessValue (int)));

    brightnessLayout->setLandscapePolicy (brightnessLandscape);
    brightnessLayout->setPortraitPolicy (brightnessPortrait);

    brightness->setObjectName ("CommonPanel");
    brightness->setLayout (brightnessLayout);
    mainLayout->addItem (brightness);

    m_screenlight_vals = m_logic->screenLightsValues ();

    // Screen dim timeout selector
    m_screenTimeout = new MContentItem (MContentItem::TwoTextLabels);
    m_screenTimeout->setObjectName ("ScreenTimeout");
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

    MWidget *displayon = new MStylableWidget;
    QGraphicsLinearLayout *blankinhibitLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    //% "Display stays lit when charging"
    m_blankInhibitLabel = new MLabel (qtTrId ("qtn_disp_screenon"));
    m_blankInhibitLabel->setObjectName("ContentItemTitle");
    blankinhibitLayout->addItem (m_blankInhibitLabel);

    // Blank inhibit
    m_blankInhibitButton = new MButton;
    m_blankInhibitButton->setObjectName("BlankInhibitButton");
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
DisplayWidget::screenTimeoutClicked ()
{
    MPopupList *popuplist = new MPopupList;
    QStringListModel *model = new QStringListModel;
    QStringList timeoutVals;

    popuplist->setTitle (qtTrId ("qtn_disp_screenoff"));

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

