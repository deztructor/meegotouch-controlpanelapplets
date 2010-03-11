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
    //% "%a hours %b minutes"
    const QString TimeValueText = qtTrId ("qtn_ener_remtime");

    if (m_TimeLabel == NULL)
        return;

    if (value.toInt() == 0) {// a text, not minute value
        m_TimeLabel->setText(value);
        return;
    }
    int minutes = value.toInt();

    const QString minutesPrefix =
        TimeValueText.section("%b", 1, 1).trimmed();
    const QString hoursPrefix =
        (TimeValueText.section("%b", 0, 0)).section("%a", 1, 1).trimmed();

    QString time;

    if (minutes < 60)
        time = QString("%1 %2").arg(minutes).arg(minutesPrefix);
    else {
        QVariant minsVar = minutes % 60;
        minsVar = (minsVar.toInt() == 0) ? "00" : minsVar;
        time = QString("%1:%2 %3").arg(minutes / 60).arg(minsVar.toString()).arg(hoursPrefix);
    }
    
    m_TimeLabel->setText(time);
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

