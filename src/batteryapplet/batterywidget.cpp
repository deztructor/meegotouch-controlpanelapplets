/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batterywidget.h"
#include "batteryimage.h"
#include "batterydbusinterface.h"
#include "dcpbattery.h"
#include "slidercontainer.h"
#include "timecontainer.h"

#include <QGraphicsLinearLayout>

#include <MButton>
#include <MContainer>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MLinearLayoutPolicy>

#define DEBUG 
#include "../debug.h"

/*
TODO list:
1) what is the correct interval for updating the battery image when charging? Is there a difference between
   USB and normal charging?

*/

BatteryWidget::BatteryWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        batteryIf (NULL),
        m_UILocked (false),
        batteryImage (NULL),
        PSMButton (NULL),
        sliderContainer (NULL),
        standByTimeContainer (NULL),
        talkTimeContainer (NULL)
{
    SYS_DEBUG ("Starting in %p", this);
    /*
     * One can assume, that when the applet is started the power save mode is
     * not active. This way we show a value that makes sense even if the DBus
     * interface to the sysuid is not usable.
     */
    m_PSMButtonToggle = false;
    initWidget();
}

BatteryWidget::~BatteryWidget ()
{
    SYS_DEBUG ("Destroying %p", this);
    if (batteryIf) {
        delete batteryIf;
        batteryIf = NULL;
    }
}

bool BatteryWidget::back ()
{
    return true; // back is handled by main window by default
}

void BatteryWidget::initWidget ()
{
    SYS_DEBUG ("Start");
    // proxy for dbus interface on remote object
    batteryIf = new BatteryDBusInterface;

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
    connect (sliderContainer, SIGNAL (PSMThresholdValueChanged (QString)),
             batteryIf, SLOT (setPSMThresholdValue (QString)));

    // mainContainer
    MLayout *orientationLayout = new MLayout;

    MGridLayoutPolicy *landscapeLayoutPolicy =
        new MGridLayoutPolicy (orientationLayout);
    landscapeLayoutPolicy->addItem (talkTimeContainer, 0, 0);
    landscapeLayoutPolicy->addItem (standByTimeContainer, 0, 1);
    landscapeLayoutPolicy->setColumnStretchFactor (0, 2);
    landscapeLayoutPolicy->setColumnStretchFactor (1, 2);
    landscapeLayoutPolicy->addItem (sliderContainer, 1, 0, 1, 2);
    landscapeLayoutPolicy->addItem (PSMButton, 2, 0, 1, 2);
    landscapeLayoutPolicy->setSpacing (10);
    orientationLayout->setLandscapePolicy (landscapeLayoutPolicy);

    MLinearLayoutPolicy *portraitLayoutPolicy =
        new MLinearLayoutPolicy (orientationLayout, Qt::Vertical);
    portraitLayoutPolicy->addItem (talkTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem (standByTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->setStretchFactor (talkTimeContainer, 2);
    portraitLayoutPolicy->setStretchFactor (standByTimeContainer, 2);
    portraitLayoutPolicy->addItem (sliderContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem (PSMButton, Qt::AlignCenter);
    portraitLayoutPolicy->setSpacing (10);
    orientationLayout->setPortraitPolicy (portraitLayoutPolicy);

    MContainer *mainContainer = new MContainer;
    mainContainer->setHeaderVisible (false);
    mainContainer->centralWidget ()->setLayout (orientationLayout);

    // connect the value receive signals
    connect (batteryIf, SIGNAL (remainingTimeValuesReceived (QStringList)),
             this, SLOT (remainingTimeValuesReceived (QStringList)));

    /*
     * Connect the batteryImage slots.
     */
    connect (batteryIf, SIGNAL (batteryCharging (int)),
             batteryImage, SLOT (startCharging (int)));
    connect (batteryIf, SIGNAL (batteryNotCharging ()),
             batteryImage, SLOT (stopCharging ()));
    connect (batteryIf, SIGNAL (batteryBarValueReceived (int)),
             batteryImage, SLOT (updateBatteryLevel (int)));
    connect (batteryIf, SIGNAL (PSMValueReceived (bool)),
             batteryImage, SLOT (setPSMValue(bool)));

    connect (batteryIf, SIGNAL (batteryNotCharging ()),
             batteryIf, SLOT (batteryBarValueRequired ()));
    connect (batteryIf, SIGNAL (PSMValueReceived (bool)),
             this, SLOT (PSMValueReceived (bool)));
    
    /*
     * SliderContainer signals and slots.
     */
    connect (batteryIf, SIGNAL (PSMAutoValueReceived (bool)),
             sliderContainer, SLOT (initPSMAutoButton (bool)));
    connect (batteryIf, SIGNAL (PSMAutoDisabled ()),
             sliderContainer, SLOT (PSMAutoDisabled ()));
    connect (batteryIf, SIGNAL (PSMThresholdValuesReceived (QStringList)),
             sliderContainer, SLOT (initSlider (QStringList)));
    connect (batteryIf, SIGNAL (PSMThresholdValueReceived (QString)),
             sliderContainer, SLOT (updateSlider (QString)));
    
    connect (batteryIf, SIGNAL (PSMThresholdValuesReceived (QStringList)),
             batteryIf, SLOT (PSMThresholdValueRequired ()));

    // send value requests over dbus
    batteryIf->remainingTimeValuesRequired ();
    batteryIf->batteryBarValueRequired ();
    batteryIf->batteryChargingStateRequired ();
    batteryIf->PSMValueRequired ();
    batteryIf->PSMAutoValueRequired ();
    batteryIf->PSMThresholdValuesRequired ();

    // mainLayout
    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->setContentsMargins (0, 0, 0, 0);
    mainLayout->addItem (mainContainer);
    this->setLayout (mainLayout);

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

    batteryIf->setPSMValue (newPSMValue);
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
        batteryIf->setPSMAutoValue (PSMAutoEnabled);
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

    // This call will reload timelabels on timercontainers
    batteryIf->remainingTimeValuesRequired ();
}

