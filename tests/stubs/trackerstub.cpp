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

#include "trackerstub.h"

#include <QString>
#include <QStringList>

#undef DEBUG
#include "../../src/debug.h"

Tracker::Tracker ()
{
    SYS_DEBUG ("Constructor: %p", this);
}

Tracker::~Tracker ()
{
    SYS_DEBUG ("Destructor : %p", this);
}

/*!
 * Returns a list of images in a simulated Tracker query, used in the wallpaper
 * applet to enumerate a list of possible wallpapers.
 *
 * Please note that this method should return at least two items, so there will
 * be 3 item in the list together with the current wallpaper. This way we will
 * have a first item, a last item and also a middle item that has the 
 * MContentItem::SingleColumnCenter visual style.
 */
QVector<QStringList>
Tracker::rawSparqlQuery (
        const QString &query)
{
    QVector<QStringList> retval;
    
    SYS_DEBUG ("*** query = %s", SYS_STR(query));
    Q_UNUSED (query);

    QStringList list1;
    list1 << 
        "file:///NoDir/NoSuchFile.png" <<
        "The image title1" <<
        "image/png" <<
        "300" << "200";
    
    QStringList list2;
    list2 << 
        "file:///AnotherSimulatedDir/WithASimulatedFile.gif" <<
        "The image title2" <<
        "image/gif" <<
        "864" << "480";

    retval << 
        list1 <<
        list2;
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


