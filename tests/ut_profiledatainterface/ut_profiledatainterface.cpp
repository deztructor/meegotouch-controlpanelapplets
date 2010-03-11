/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_profiledatainterface.h"
#include "profiledatainterface.h"

#include <DuiApplication>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Ut_ProfileDataInterface implementation. 
 */
void 
Ut_ProfileDataInterface::init()
{
}

void 
Ut_ProfileDataInterface::cleanup()
{
}

DuiApplication *app;

void 
Ut_ProfileDataInterface::initTestCase()
{

    int argc = 1;
    char* app_name = (char*) "./Ut_ProfileDataInterface";
    app = new DuiApplication(argc, &app_name);
}

void 
Ut_ProfileDataInterface::cleanupTestCase()
{
    delete app;
}

QTEST_APPLESS_MAIN(Ut_ProfileDataInterface)
