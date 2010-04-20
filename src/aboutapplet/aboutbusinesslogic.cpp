/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "aboutbusinesslogic.h"

#include <QString>

#define DEBUG
#include "../debug.h"

AboutBusinessLogic::AboutBusinessLogic()
{
}

AboutBusinessLogic::~AboutBusinessLogic()
{
}

/*!
 * Returns the version number of the operating system (Maemo version).
 */
QString 
AboutBusinessLogic::osVersion ()
{
    return "version number";
}

QString 
AboutBusinessLogic::WiFiAddress ()
{
    return "address";
}

QString 
AboutBusinessLogic::BluetoothAddress ()
{
    return "address";
}

QString 
AboutBusinessLogic::IMEI ()
{
    return "value";
}

