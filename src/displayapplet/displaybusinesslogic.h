/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef DISPLAYBUSINESSLOGIC_H
#define DISPLAYBUSINESSLOGIC_H

/*
 * In the functional tests we use the real thing, in the unit tests we use the
 * stubbed version. 
 */
#if defined(UNIT_TEST) && !defined(FUNCTIONAL_TEST)
#  include "qmdisplaystatestub.h"
#else
#  include <qmsystem/qmdisplaystate.h>
#endif

#include <QObject>

class MGConfItem;

using namespace Maemo;

class DisplayBusinessLogic : public QObject
{
    Q_OBJECT

public:
    DisplayBusinessLogic (QObject* parent = 0);
    virtual ~DisplayBusinessLogic();

    QList<int> brightnessValues();
    int selectedBrightnessValue();
    QList<int> screenLightsValues();
    int selectedScreenLightsValue();
    bool blankInhibitValue();

public slots:
    void setBrightnessValue(int value);
    void setScreenLightTimeouts (int index);
    void setBlankInhibitValue(bool value);
    void toggleDisplay(bool toggle);

private: 
    QmDisplayState *m_Display;
    MGConfItem     *m_possibleDimValues;

};

#endif
