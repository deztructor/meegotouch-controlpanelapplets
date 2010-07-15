/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef PERCENTAGECONTAINER_H
#define PERCENTAGECONTAINER_H

#include <MContainer>

class MImageWidget;
class MLabel;

/*!
 * A simple widget with two labels, one is the title/meaning of the widget, the
 * other is a time value. This widget is used to show the remaining battery time
 * in stand-by and active mode.
 * This widget also holds an image, but does not handle its changes.
 */
class PercentageContainer : public MContainer
{
public:
    PercentageContainer(const QString &text, MImageWidget *image,
                  MWidget *parent = 0);

    void updateCapacity (const int value);
    void setText (const QString &text);

public slots:
    void charging(int);

private:
    void setLayout();

private:
    MImageWidget  *m_Image;
    MLabel        *m_TextLabel;
    MLabel        *m_TimeLabel;
};

#endif // PERCENTAGECONTAINER_H
