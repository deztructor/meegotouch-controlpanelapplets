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

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <dcpwidget.h>

class MButton;
class MSlider;
class MLabel;
class MContainer;
class MBasicListItem;
class DisplayBusinessLogic;

class DisplayWidget : public DcpWidget
{
    Q_OBJECT

public:
    DisplayWidget(QGraphicsWidget *parent = 0);
    virtual ~DisplayWidget();

protected:
    void initWidget();

private slots:
    void screenTimeoutClicked ();
    void sliderUpdated (int val);

private:
    void retranslateUi ();

    DisplayBusinessLogic   *m_logic;
    QList<int>              m_brightness_vals;
    QList<int>              m_screenlight_vals;
    MSlider                *m_brightnessSlider;
    MBasicListItem         *m_screenTimeout;
    MButton                *m_blankInhibitButton;
    MLabel                 *m_brightnessLabel;
    MLabel                 *m_screenlightLabel;
    MLabel                 *m_blankInhibitLabel;
    #ifdef UNIT_TEST
    friend class Ut_DisplayWidget;
    #endif
};

#endif // DISPLAYWIDGET_H

