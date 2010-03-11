/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef TIMECONTAINER_H
#define TIMECONTAINER_H

#include <DuiContainer>

class DuiImageWidget;
class DuiLabel;

/*!
 * A simple widget with two labels, one is the title/meaning of the widget, the
 * other is a time value. This widget is used to show the remaining battery time
 * in stand-by and active mode.
 * This widget also holds an image, but does not handle its changes.
 */
class TimeContainer : public DuiContainer
{
public:
    TimeContainer(const QString &text, DuiImageWidget *image,
                  DuiWidget *parent = 0);

    void updateTimeLabel (const QString &text);
    void setText (const QString &text);

private:
    void setLayout();

private:
    DuiImageWidget  *m_Image;
    DuiLabel        *m_TextLabel;
    DuiLabel        *m_TimeLabel;
};

#endif // STANDBYTIMECONTAINER_H
