/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LEDBUSINESSLOGIC_H
#define LEDBUSINESSLOGIC_H

#include <QObject>
#include <qmsystem/qmled.h>

#include "systemuigconf.h"

using namespace Maemo;

class LedBusinessLogic : public QObject
{
    Q_OBJECT

public:
    LedBusinessLogic (SystemUIGConf *systemUIGConf, QObject *parent = 0);
    ~LedBusinessLogic ();

    bool ledsEnabled ();
    void setLedsEnabled (bool enabled);
    
private:
    SystemUIGConf      *m_SystemUIGConf;
    QmLED              *m_Led;
};

#endif