/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_profilebuttons.h"
#include "profilebuttons.h"

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_ProfileButtons implementation. 
 */
void 
Ut_ProfileButtons::init()
{
}

void 
Ut_ProfileButtons::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ProfileButtons";

void 
Ut_ProfileButtons::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Buttons = new ProfileButtons;
}

void 
Ut_ProfileButtons::cleanupTestCase()
{
    delete m_Buttons;
    m_App->deleteLater ();
}


QTEST_APPLESS_MAIN(Ut_ProfileButtons)




