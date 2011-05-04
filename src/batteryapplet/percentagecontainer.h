/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef PERCENTAGECONTAINER_H
#define PERCENTAGECONTAINER_H

#include <MContainer>

class QGraphicsLinearLayout;
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
    PercentageContainer(
            const QString &text, 
            MImageWidget *image,
            MWidget *parent = 0);

    void updateCapacity (const int value);
    void updateRemainingChargingTime (int ChTime);
    void updateRemainingTime (int talk, int idle);
    void setText (const QString &text);

public slots:
    void charging (int);

private:
    void setLayout ();
    QString formatTime (int remTime);

private:
    QGraphicsLinearLayout  *m_MainLayout;
    MImageWidget           *m_Image;
    MLabel                 *m_TextLabel;
    MLabel                 *m_SubTextLabel;
};

#endif // PERCENTAGECONTAINER_H
