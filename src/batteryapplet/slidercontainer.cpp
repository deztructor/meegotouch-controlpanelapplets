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

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MSlider>

#include "percentagecontainer.h"

//#define DEBUG
//#define WARNING
#include "../debug.h"

#include <QDebug>

SliderContainer::SliderContainer (MWidget *parent) :
        QObject (parent),
        m_LabelContainer (0),
        m_SliderContainer (0),
        m_PSMSlider (0),
        m_SliderValue (-1),
        m_SliderExists (false)
{
    SYS_DEBUG ("");
    createWidgets ();
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

void 
SliderContainer::createWidgets ()
{
    QGraphicsLinearLayout *labelLayout;
    QGraphicsLinearLayout *sliderLayout;
    

    SYS_DEBUG ("");

    /*
     * A container for the two labels.
     */
    m_LabelContainer = new MContainer;
    m_LabelContainer->setContentsMargins (0,0,0,0);
    m_LabelContainer->setStyleName ("CommonPanelInverted");
    m_LabelContainer->setHeaderVisible (false);

    labelLayout = new QGraphicsLinearLayout (Qt::Vertical);
    labelLayout->setContentsMargins (0,0,0,0);
    m_LabelContainer->centralWidget()->setLayout (labelLayout);

    /*
     * "Auto activate power save" label
     */
    m_AutoPSMLabel = new MLabel;
    m_AutoPSMLabel->setStyleName ("CommonTitleInverted");
    labelLayout->addItem (m_AutoPSMLabel);
    labelLayout->setAlignment (m_AutoPSMLabel, Qt::AlignLeft);

    /*
     * A subtitle that shows the current value of the slider.
     */
    m_PsmValueLabel = new MLabel;
    m_PsmValueLabel->setStyleName ("CommonSubTitleInverted");
    labelLayout->addItem (m_PsmValueLabel);
    labelLayout->setAlignment (m_PsmValueLabel, Qt::AlignLeft);

    labelLayout->addItem (m_AutoPSMLabel);
    labelLayout->addItem (m_PsmValueLabel);
    
    /*
     * A container to hold the slider.
     */
    m_SliderContainer = new MContainer;
    m_SliderContainer->setContentsMargins (0,0,0,0);
    m_SliderContainer->setStyleName ("CommonPanelInverted");
    m_SliderContainer->setHeaderVisible (false);

    sliderLayout = new QGraphicsLinearLayout (Qt::Horizontal);
    sliderLayout->setContentsMargins (0,0,0,0);
    m_SliderContainer->centralWidget()->setLayout (sliderLayout);

    /*
     * Power save mode auto activation slider
     */
    m_PSMSlider = new MSlider;
    m_PSMSlider->setObjectName ("PSMSlider");
    m_PSMSlider->setStyleName ("CommonSliderInverted");
    m_PSMSlider->setOrientation (Qt::Horizontal);
    m_PSMSlider->setHandleLabelVisible (true);
    m_PSMSlider->setRange (0, m_SliderValues.size () - 1);
    
    /*
     * Adding the slider to its panel. Please note that the slider size might be
     * set in the theme, so we need to add stretchers to move the short slider
     * into the middle of the box.
     */
    sliderLayout->addStretch ();
    sliderLayout->addItem (m_PSMSlider);
    sliderLayout->addStretch ();
    sliderLayout->setAlignment (m_PSMSlider, Qt::AlignHCenter);

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

    if (m_SliderValue >= 0) {
        m_PSMSlider->setValue (m_SliderValue);
    } else {
        SYS_WARNING ("ERROR: got an invalid PSM value: %d", value);
        foreach (QString str, m_SliderValues)
            SYS_DEBUG ("Available slider value: %s", SYS_STR (str));
        /*
         * This should not happen, but we show some label anyway.
         */
        m_SliderValue = 0;
    }

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
 * Updates the label that shows the auto PSM slider value or shows the 'off'
 * string when the auto power save mode is disabled.
 */
void
SliderContainer::updateSliderValueLabel ()
{
    int index = m_SliderValue;

    /*
     * We want to show something, but this should never happen.
     */
    if (index < 0 || index >= m_SliderValues.size())
        index = 0;

    if (index >= 0 && index < m_SliderValues.size()) {
        m_PsmValueLabel->setText (QString ("%1%").arg (
                    m_SliderValues[index]));
        m_PSMSlider->setHandleLabel(QString ("%1%").arg (
                m_SliderValues[index]));
    }
}

