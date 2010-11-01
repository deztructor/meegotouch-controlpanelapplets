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

#include "warrantybusinesslogic.h"

#ifdef HAVE_QMSYSTEM
#  include <qmsystemstate.h>
#endif

// TODO: TBD
#define WARRANTY_DAYS 365

#undef DEBUG
#include "../debug.h"


WarrantyBusinessLogic::WarrantyBusinessLogic() 
{
}

WarrantyBusinessLogic::~WarrantyBusinessLogic()
{
}

int
WarrantyBusinessLogic::getExpirationDays ()
{
    int retVal = -1;

#ifdef HAVE_QMSYSTEM
    MeeGo::QmSystemState  systemState;
    int elapsedDays = systemState.getPowerOnTimeInSeconds () / (60 * 60 * 24);

    retVal = WARRANTY_DAYS - elapsedDays;

    if (retVal < 0)
        retVal = 0;
#else
    /*
     * FIXME: To implement a version that does not use the QmSystem.
     */
#endif

    return retVal;
}

