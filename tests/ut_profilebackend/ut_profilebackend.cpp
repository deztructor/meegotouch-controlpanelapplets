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
#include "ut_profilebackend.h"
#include "profilebackend.h"

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Ut_ProfileBackend implementation. 
 */
void 
Ut_ProfileBackend::init()
{
     m_backend = new ProfileBackend;
}

void 
Ut_ProfileBackend::cleanup()
{
    delete m_backend;
    m_backend = 0;
}

void 
Ut_ProfileBackend::initTestCase()
{
    // run once before all test cases
}

void 
Ut_ProfileBackend::cleanupTestCase()
{
    // run once after all test cases or failure
}

void
Ut_ProfileBackend::testCase1 ()
{

}

QTEST_MAIN(Ut_ProfileBackend)

