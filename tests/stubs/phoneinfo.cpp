/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "phoneinfo.h"

#include <QString>

#define DEBUG
#define WARNING
#include "../../src/debug.h"

using namespace Cellular;

PhoneInfo::PhoneInfo ()
{
    SYS_DEBUG ("Creating %p", this);
}

PhoneInfo::~PhoneInfo ()
{
    SYS_DEBUG ("Destroying %p", this);
}

QString 
PhoneInfo::imeiNumber() const
{
    return FAKE_IMEI_NUMBER;
}

