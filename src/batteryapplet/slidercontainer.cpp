/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "slidercontainer.h"

#include <MButton>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MLayout>
#include <MSlider>

#define DEBUG 
#include "../debug.h"

SliderContainer::SliderContainer (MWidget *parent) :
        MContainer (parent),
        m_PSMAutoButton (0),
        m_PSMSlider (0),
        m_SliderValue (-1),
        m_SliderExists (false)
{
    SYS_DEBUG ("");

    setHeaderVisible (false);
    setLayout ();
}

SliderContainer::~SliderContainer ()
{
    SYS_DEBUG ("Destroying %p", this);
}


void
SliderContainer::retranslate ()
{
    SYS_DEBUG ("");
    //% "Auto activate power save"
    m_AutoPSMLabel->setText (qtTrId ("qtn_ener_autops"));
    updateSliderValueLabel ();
}

void SliderContainer::setLayout()
{
    MLayout              *labelLayout;
    MLinearLayoutPolicy  *labelLayoutPolicy;

    SYS_DEBUG ("");

    labelLayout = new MLayout;
    labelLayoutPolicy = new MLinearLayoutPolicy (labelLayout, Qt::Vertical);

    MLayout *layout = new MLayout;
    m_LayoutPolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    MLayout *hlayout = new MLayout;
    MLinearLayoutPolicy *hpolicy = new MLinearLayoutPolicy (hlayout, Qt::Horizontal);

    /*
     * "Auto activate power save" label
     */
    m_AutoPSMLabel = new MLabel;
    m_AutoPSMLabel->setObjectName ("batteryLabel");
    labelLayoutPolicy->addItem (m_AutoPSMLabel, Qt::AlignLeft);

    /*
     *
     */
    m_PsmValueLabel = new MLabel;
    m_PsmValueLabel->setObjectName ("psmValueLabel");
    labelLayoutPolicy->addItem (m_PsmValueLabel, Qt::AlignLeft);
    
    /*
     * Adding the labels to the upper horizontal part.
     */
    hpolicy->addItem (labelLayout);

    // m_PSMAutoButton
    m_PSMAutoButton = new MButton;
    connect (m_PSMAutoButton, SIGNAL (toggled (bool)),
             this, SLOT (PSMAutoButtonToggled (bool)));
    m_PSMAutoButton->setCheckable (true);
    m_PSMAutoButton->setViewType (MButton::switchType);
    // m_PSMAutoButton->setObjectName ("PSMAutoButton");
    hpolicy->addItem (m_PSMAutoButton, Qt::AlignRight | Qt::AlignVCenter);

   
    m_LayoutPolicy->addItem (hlayout);

    centralWidget ()->setLayout (layout);
    
    retranslate ();
}

/*!
 * This is in fact not an initialization function, the backend calls it to set
 * the slider values.
 */
void 
SliderContainer::initSlider (
        const QStringList &values)
{
    SYS_DEBUG ("*** m_SliderValue = %d", m_SliderValue);
    m_SliderValues = values;

    if (m_PSMSlider)
        m_PSMSlider->setRange (0, m_SliderValues.size () - 1);

    updateSliderValueLabel ();
}

/*!
 * This slot is called when the backend returns the PSM value so we have to set
 * the slider accordingly.
 */
void 
SliderContainer::updateSlider (int value)
{
    SYS_DEBUG ("value = %d", value);

    // Store the actual value for later
    // (eg for the case when slider isn't ready yet...)
    m_SliderValue = m_SliderValues.indexOf (QString (value));

    // Slider not yet created:
    if (m_PSMSlider == 0)
        return;

    m_PSMSlider->setValue (m_SliderValue);

    updateSliderValueLabel ();
}

/*!
 * This function is called when the user drags the slider and when the slider
 * value has been changed by the applet to show the value that came from the
 * backend.
 */
void 
SliderContainer::sliderValueChanged (
        int value)
{
    SYS_DEBUG ("*** slider = %p", m_PSMSlider);
    SYS_DEBUG ("*** value  = %d", value);

    m_SliderValue = value;
    
    updateSliderValueLabel ();
    emit PSMThresholdValueChanged (m_SliderValues.at (value).toInt ());
}

void 
SliderContainer::toggleSliderExistence (
        bool toggle)
{
    SYS_DEBUG ("");
    
    if (toggle == m_SliderExists)
        return;
    m_SliderExists = toggle;

    if (toggle) {
        if ((m_LayoutPolicy->count () < 2) && (m_PSMSlider == 0)) {
            m_PSMSlider = new MSlider;
            SYS_DEBUG ("Connecting %p->valueChanged", m_PSMSlider);
            SYS_DEBUG ("m_SliderValue = %d", m_SliderValue);
            
            m_PSMSlider->setOrientation (Qt::Horizontal);
            m_PSMSlider->setHandleLabelVisible (false);
            m_PSMSlider->setRange (0, m_SliderValues.size () - 1);

            connect (m_PSMSlider, SIGNAL (valueChanged (int)),
                    this, SLOT (sliderValueChanged (int)));

            if (m_SliderValue < 0)
            {
                m_SliderValue = 0;
                /*
                 * Set some default value [the smaller one]
                 * if nothing set yet...
                 */
                sliderValueChanged (0);
            }

            m_PSMSlider->setValue (m_SliderValue);
            /*
             *
             */
            m_LayoutPolicy->addItem (m_PSMSlider);
        }
    } else {
        if ((m_PSMSlider) && (m_LayoutPolicy->count () > 1)) {
            m_LayoutPolicy->removeItem (m_PSMSlider);
            m_PSMSlider->deleteLater();
            m_PSMSlider = 0;
        }
    }

    updateSliderValueLabel ();
}

/*!
 * This function is called when the dackend decides if the automatic power save
 * mode is enabled or disabled, so we can toggle the switch widget. Everything
 * else is following the switch widget change.
 */
void
SliderContainer::initPSMAutoButton (
        bool toggle)
{
    SYS_DEBUG ("toggle = %s", SYS_BOOL (toggle));

    m_PSMAutoButton->setChecked (toggle);

    toggleSliderExistence (toggle);
}

void SliderContainer::PSMAutoButtonToggled (bool toggle)
{
    SYS_DEBUG ("toggle = %s", SYS_BOOL (toggle));

    toggleSliderExistence (toggle);
    emit PSMAutoToggled (toggle);
}

/*!
 * Updates the label that shows the auto PSM slider value or shows the 'off'
 * string when the auto power save mode is disabled.
 */
void
SliderContainer::updateSliderValueLabel ()
{
    if (!m_SliderExists) {
        //% "Off"
        m_PsmValueLabel->setText (qtTrId ("qtn_comm_settings_off"));
    }

    if (m_SliderExists && 
            m_SliderValue >= 0 && 
            m_SliderValue < m_SliderValues.size()) {
        m_PsmValueLabel->setText (QString ("%1%").arg (
                    m_SliderValues[m_SliderValue]));
    }

    SYS_DEBUG ("text = %s", SYS_STR (m_PsmValueLabel->text ()));
}

QSizeF
SliderContainer::sizeHint (Qt::SizeHint which,
                           const QSizeF &constraint) const
{
    // Why not doing this the Qt or the meegotouch by default?
    if (isVisible ())
        return MContainer::sizeHint (which, constraint);
    else
        return (QSizeF (0., 0.));
}


