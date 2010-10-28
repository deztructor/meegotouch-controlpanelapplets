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

#include "slidercontainer.h"

#include <MButton>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MLayout>
#include <MSlider>

#include "percentagecontainer.h"


#undef DEBUG
#include "../debug.h"

#include <QDebug>

SliderContainer::SliderContainer (MWidget *parent) :
        MContainer (parent),
        m_PSMAutoButton (0),
        m_PSMSlider (0),
        m_SliderValue (-1),
        m_SliderExists (false)
{
    SYS_DEBUG ("");
    setStyleName("CommonPanel");
    setObjectName ("SliderContainer");
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

    //% "Activation battery level"
    m_AutoPSMLabel->setText (qtTrId("qtn_ener_activation"));
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
    m_AutoPSMLabel->setObjectName ("CommonTitle");
    labelLayoutPolicy->addItem (m_AutoPSMLabel, Qt::AlignLeft);

    /*
     *
     */
    m_PsmValueLabel = new MLabel;
    m_PsmValueLabel->setObjectName ("CommonSubTitle");
    labelLayoutPolicy->addItem (m_PsmValueLabel, Qt::AlignLeft);

    /*
     * Adding the labels to the upper horizontal part.
     */
    hpolicy->addItem (labelLayout);

    m_LayoutPolicy->addItem (hlayout);
    
    /*
     * PSM Slider
     */
    m_PSMSlider = new MSlider;
    SYS_DEBUG ("Connecting %p->valueChanged", m_PSMSlider);
    SYS_DEBUG ("m_SliderValue = %d", m_SliderValue);

    m_PSMSlider->setObjectName ("PSMSlider");
    m_PSMSlider->setOrientation (Qt::Horizontal);
    m_PSMSlider->setHandleLabelVisible (true);
    m_PSMSlider->setRange (0, m_SliderValues.size () - 1);
    m_LayoutPolicy->addItem(m_PSMSlider);

    /*
     * Set the slider value if available...
     */
    if (m_SliderValue >= 0)
        m_PSMSlider->setValue (m_SliderValue);

    /*
     * .. and after connect the slidervalue changed signal
     */
    connect (m_PSMSlider, SIGNAL (valueChanged (int)),
            this, SLOT (sliderValueChanged (int)),
            Qt::DirectConnection);
    connect (m_PSMSlider, SIGNAL (valueChanged (int)),
            this, SLOT (updateSliderHandleLabel(int)),
            Qt::DirectConnection);

    centralWidget ()->setLayout (layout);

    retranslate ();
}

/*!
 * This is in fact not an initialization function, the backend calls it to set
 * the slider values.
 * Must be called before updateSlider!
 */
void
SliderContainer::initSlider (
        const QStringList &values)
{
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
    SYS_DEBUG ("*** value = %d", value);

    // Store the actual value for later
    // (eg for the case when slider isn't ready yet...)
    m_SliderValue = m_SliderValues.indexOf (QString ("%1").arg (value));

    if (m_SliderValue >= 0)
        m_PSMSlider->setValue (m_SliderValue);
#ifdef DEBUG
    else
    {
        SYS_WARNING ("ERROR: got an invalid PSM value: %d", value);
        foreach (QString str, m_SliderValues)
            SYS_DEBUG ("Available slider value: %s", SYS_STR (str));
    }
#endif

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

    if (m_SliderValue >= 0)
        m_PSMSlider->setValue (m_SliderValue);
}

/*!
 * Updates the label that shows the auto PSM slider value or shows the 'off'
 * string when the auto power save mode is disabled.
 */
void
SliderContainer::updateSliderValueLabel ()
{
    if ( m_SliderValue >= 0 &&
         m_SliderValue < m_SliderValues.size()) {

        m_PsmValueLabel->setText (QString ("%1%").arg (
                    m_SliderValues[m_SliderValue]));
        m_PSMSlider->setHandleLabel(QString ("%1%").arg (
                m_SliderValues[m_SliderValue]));
    }
}

