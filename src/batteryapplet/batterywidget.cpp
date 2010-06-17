/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batterywidget.h"
#include "batteryimage.h"
#include "batterybusinesslogic.h"
#include "dcpbattery.h"
#include "slidercontainer.h"
#include "timecontainer.h"

#include <QGraphicsLinearLayout>
#include <QTimer>

#include <MButton>
#include <MContainer>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MLinearLayoutPolicy>

#define DEBUG
#include "../debug.h"

BatteryWidget::BatteryWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        m_logic (0),
        m_UILocked (false),
        batteryImage (0),
        m_MainLayout (0),
        PSMButton (0),
        sliderContainer (0),
        standByTimeContainer (0),
        talkTimeContainer (0)
{
    SYS_DEBUG ("Starting in %p", this);
    /*
     * One can assume, that when the applet is started the power save mode is
     * not active. This way we show a value that makes sense even if the DBus
     * interface to the sysuid is not usable.
     */
    m_PSMButtonToggle = false;

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

void BatteryWidget::initWidget ()
{
    SYS_DEBUG ("Start");
    // instantiate the batterybusinesslogic
    m_logic = new BatteryBusinessLogic;

    // battery image
    batteryImage = new BatteryImage;

    // talkTimeContainer
    //% "Estimated talk time:"
    talkTimeContainer = new TimeContainer (
            qtTrId ("qtn_ener_tt"), batteryImage);

    // standByTimeContainer
    //% "Estimated stand-by time:"
    standByTimeContainer = new TimeContainer (
            qtTrId ("qtn_ener_st"), new MImageWidget);
                                  //"qgn_ener_standby" ^

    /*
     * PSMButton is used to immediately turn the power save mode on/off.
     */
    PSMButton = new MButton;
    updatePSMButton ();

    connect (PSMButton, SIGNAL (released ()),
             this, SLOT (PSMButtonReleased ()));

    // sliderContainer
    sliderContainer = new SliderContainer;

    connect (sliderContainer, SIGNAL (PSMAutoToggled (bool)),
             this, SLOT (PSMAutoToggled (bool)));
    connect (sliderContainer, SIGNAL (PSMThresholdValueChanged (int)),
             m_logic, SLOT (setPSMThresholdValue (int)));

    // mainContainer
    m_MainLayout = new MLayout;

    MGridLayoutPolicy *landscapeLayoutPolicy =
        new MGridLayoutPolicy (m_MainLayout);
    landscapeLayoutPolicy->addItem (talkTimeContainer, 0, 0);
    landscapeLayoutPolicy->addItem (standByTimeContainer, 0, 1);
    landscapeLayoutPolicy->setColumnStretchFactor (0, 2);
    landscapeLayoutPolicy->setColumnStretchFactor (1, 2);
    landscapeLayoutPolicy->addItem (sliderContainer, 1, 0, 1, 2);
    landscapeLayoutPolicy->addItem (PSMButton, 2, 0, 1, 2);
    landscapeLayoutPolicy->setSpacing (10);
    m_MainLayout->setLandscapePolicy (landscapeLayoutPolicy);

    MLinearLayoutPolicy *portraitLayoutPolicy =
        new MLinearLayoutPolicy (m_MainLayout, Qt::Vertical);
    portraitLayoutPolicy->addItem (talkTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem (standByTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->setStretchFactor (talkTimeContainer, 2);
    portraitLayoutPolicy->setStretchFactor (standByTimeContainer, 2);
    portraitLayoutPolicy->addItem (sliderContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem (PSMButton, Qt::AlignCenter);
    portraitLayoutPolicy->setSpacing (10);
    m_MainLayout->setPortraitPolicy (portraitLayoutPolicy);

    MContainer *mainContainer = new MContainer;
    mainContainer->setHeaderVisible (false);
    mainContainer->centralWidget ()->setLayout (m_MainLayout);

    // connect the value receive signals
    connect (m_logic, SIGNAL (remainingTimeValuesChanged (QStringList)),
             this, SLOT (remainingTimeValuesReceived (QStringList)));

    /*
     * Connect the batteryImage slots.
     */
    connect (m_logic, SIGNAL (batteryCharging (int)),
             batteryImage, SLOT (startCharging (int)));
    connect (m_logic, SIGNAL (batteryBarValueReceived (int)),
             batteryImage, SLOT (updateBatteryLevel (int)));
    connect (m_logic, SIGNAL (PSMValueReceived (bool)),
             batteryImage, SLOT (setPSMValue (bool)));

    connect (m_logic, SIGNAL (PSMValueReceived (bool)),
             this, SLOT (PSMValueReceived (bool)));

    /*
     * SliderContainer signals and slots,
     * and initialization
     */
    sliderContainer->initPSMAutoButton (m_logic->PSMAutoValue ());
    sliderContainer->initSlider (m_logic->PSMThresholdValues ());
    sliderContainer->updateSlider (m_logic->PSMThresholdValue ());

    // mainLayout
    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);

    mainLayout->setContentsMargins (0, 0, 0, 0);
    mainLayout->addItem (mainContainer);
    mainLayout->addStretch ();
    setLayout (mainLayout);

    // Initialize the values from the business logic
    QTimer::singleShot (0, m_logic, SLOT (requestValues ()));

    SYS_DEBUG ("End");
}

/*!
 * This function is called when the user clicked on the 'power save mode' button
 * that activates and disactivates the power saving mode. The function will call
 * the battery interface, but the UI will be changed only when the sysuid
 * reports the change.
 */
void 
BatteryWidget::PSMButtonReleased ()
{
    bool newPSMValue = !m_PSMButtonToggle;

    SYS_DEBUG ("Setting PSMvalue to %s", SYS_BOOL(newPSMValue));
    if (newPSMValue) {
        m_UILocked = true;
        sliderContainer->setVisible (!m_PSMButtonToggle);
        m_UILocked = false;
    }

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
            sliderContainer->updateSlider (m_logic->PSMThresholdValue ());
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

void BatteryWidget::remainingTimeValuesReceived(const QStringList &timeValues)
{
    SYS_DEBUG ("timevalues = %s, %s",
               SYS_STR (timeValues.at (0)),
               SYS_STR (timeValues.at (1)));

    talkTimeContainer->updateTimeLabel (timeValues.at (0));
    standByTimeContainer->updateTimeLabel (timeValues.at (1));
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
    SYS_DEBUG ("Hiding sliderContainer");
    sliderContainer->setVisible (!m_PSMButtonToggle);
    m_UILocked = false;

    m_MainLayout->invalidate ();
}

void 
BatteryWidget::retranslateUi ()
{
    // This call will reload the translated text on PSButton
    updatePSMButton ();

    // This call will retranslate the label (infoText)
    sliderContainer->retranslate ();

    talkTimeContainer->setText(qtTrId ("qtn_ener_tt"));
    standByTimeContainer->setText (qtTrId ("qtn_ener_st"));

    m_logic->remainingTimeValuesRequired ();
}

