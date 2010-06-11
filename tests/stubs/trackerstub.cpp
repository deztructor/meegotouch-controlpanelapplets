/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
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
#include "trackerstub.h"

#include <QString>
#include <QStringList>

#define DEBUG
#include "../../src/debug.h"

Tracker::Tracker ()
{
    SYS_DEBUG ("Constructor: %p", this);
}

Tracker::~Tracker ()
{
    SYS_DEBUG ("Destructor : %p", this);
}

QVector<QStringList>
Tracker::rawSparqlQuery (
        const QString &query)
{
    QVector<QStringList> retval;
    
    SYS_DEBUG ("*** query = %s", SYS_STR(query));
    return retval;
}

Tracker *instance = NULL;

Tracker *
tracker ()
{
    if (instance == NULL)
        instance = new Tracker;

    return instance;
}


