/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_batterydbusinterface.h"
#include "batterydbusinterface.h"
#include <DuiApplication>

void 
Ut_BatteryDbusInterface::init()
{
}

void 
Ut_BatteryDbusInterface::cleanup()
{
}

DuiApplication *app;
void 
Ut_BatteryDbusInterface::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_BatteryDbusInterface";
    app = new DuiApplication(argc, &app_name);
}

void 
Ut_BatteryDbusInterface::cleanupTestCase()
{
    delete app;
}

QTEST_APPLESS_MAIN(Ut_BatteryDbusInterface)
