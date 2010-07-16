/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "percentagecontainer.h"

#include <MGridLayoutPolicy>
#include <MImageWidget>
#include <MLabel>
#include <MLayout>

#define DEBUG 
#include "../debug.h"

PercentageContainer::PercentageContainer(
		const QString  &text, 
		MImageWidget *image, 
		MWidget      *parent) :
	MContainer (parent),
        m_Image (image),
        m_TextLabel (0)
{
    m_TextLabel = new MLabel (text);
    m_TextLabel->setObjectName ("batteryLabel");

    setHeaderVisible (false);
    setLayout ();
}


void 
PercentageContainer::setText (
		const QString &text)
{
    m_TextLabel->setText (text);
}

void PercentageContainer::updateCapacity(
        const int value)
{
    QString text;
    //% "Battery level \%L1\%"
    text = qtTrId ("qtn_ener_battery_level", value).arg (value);
    m_TextLabel->setText(text);
}

void PercentageContainer::setLayout()
{
    // set the layout
    MLayout *layout = new MLayout();
    MGridLayoutPolicy *layoutPolicy = new MGridLayoutPolicy(layout);
    centralWidget()->setLayout(layout);

    // add the widgets
    layoutPolicy->addItem (m_Image, 0, 0, 1, 1);
    layoutPolicy->addItem (m_TextLabel, 0, 1, 1, 1,Qt::AlignVCenter);
}


