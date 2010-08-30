/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "percentagecontainer.h"

#include <QGraphicsLinearLayout>
#include <MImageWidget>
#include <MLabel>

//#define DEBUG
#include "../debug.h"

PercentageContainer::PercentageContainer(
    const QString  &text,
    MImageWidget *image,
    MWidget      *parent) :
    MContainer (parent),
        m_Image (image),
        m_TextLabel (0)
{
    SYS_DEBUG ("*** text = %s", SYS_STR(text));
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
    text = qtTrId ("qtn_ener_battery_level").arg (value);
    SYS_DEBUG ("*** text = %s", SYS_STR(text));
    m_TextLabel->setText(text);
}

void PercentageContainer::setLayout()
{
    QGraphicsLinearLayout *layout =
        new QGraphicsLinearLayout (Qt::Horizontal);

    // add the widgets
    layout->addItem (m_Image);
    layout->addItem (m_TextLabel);
    layout->addStretch ();

    // set the layout
    centralWidget ()->setLayout (layout);
}

