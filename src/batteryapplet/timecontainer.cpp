/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "timecontainer.h"

#include <DuiGridLayoutPolicy>
#include <DuiImageWidget>
#include <DuiLabel>
#include <DuiLayout>

TimeContainer::TimeContainer(
		const QString  &text, 
		DuiImageWidget *image, 
		DuiWidget      *parent) :
	DuiContainer (parent),
        m_Image (image),
        m_TextLabel (0),
        m_TimeLabel (0)
{
    m_TimeLabel = new DuiLabel ("N/A");
    m_TimeLabel->setObjectName ("batteryTimeLabel");

    m_TextLabel = new DuiLabel (text);
    m_TextLabel->setObjectName ("batteryLabel");

    setHeaderVisible (false);
    setLayout ();
}


void 
TimeContainer::setText (
		const QString &text)
{
    m_TextLabel->setText (text);
}

void TimeContainer::updateTimeLabel (const QString &value)
{
    if (m_TimeLabel == NULL)
        return;

    if (value.toInt() == 0) { // a text, not minute value
        m_TimeLabel->setText (value);
        return;
    }

    int minutes = value.toInt();

    QString time;

    if (minutes < 15)
    {
        //% "Less than 15 minutes"
        time = qtTrId ("qtn_ener_stest_15min");
    }
    else if (minutes < 60)
    {
        //% "Less than an hour"
        time = qtTrId ("qtn_ener_stest_1h");
    }
    else
    {
        //% "%1 hours"
        time = qtTrId ("qtn_ener_stest_hours").arg (minutes / 60);
    }
    
    m_TimeLabel->setText (time);
}

void TimeContainer::setLayout()
{
    // set the layout
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    centralWidget()->setLayout(layout);

    // add the widgets
    layoutPolicy->addItem (m_Image, 0, 0, 2, 1);
    layoutPolicy->addItem (m_TextLabel, 0, 1, 1, 1);
    layoutPolicy->addItem (m_TimeLabel, 1, 1, 1, 1);
}

