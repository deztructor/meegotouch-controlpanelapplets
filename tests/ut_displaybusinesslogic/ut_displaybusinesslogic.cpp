/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_displaybusinesslogic.h"
#include "displaybusinesslogic.h"

#include <DuiApplication>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Ut_DisplayBusinessLogic implementation. 
 */
void 
Ut_DisplayBusinessLogic::init()
{
}

void 
Ut_DisplayBusinessLogic::cleanup()
{
}


void 
Ut_DisplayBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_DisplayBusinessLogic";

    m_App = new DuiApplication (argc, &app_name);
    m_Api = new DisplayBusinessLogic;
}

void 
Ut_DisplayBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
}


QTEST_APPLESS_MAIN(Ut_DisplayBusinessLogic)
