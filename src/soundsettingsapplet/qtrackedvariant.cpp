/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
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
#include <QDebug>
#include "qtrackedvariant.h"

//#define DEBUG
#define WARNING 
#include "debug.h"

QTrackedVariant::QTrackedVariant (
        const QString &key) :
	m_key(key)
{
}

void
QTrackedVariant::emit_changed()
{
	emit changed();
}

void
QTrackedVariant::set (
        const QVariant &newValue)
{
	realSetValue (newValue);
}

void
QTrackedVariant::realSetValue (
        const QVariant &newValue)
{
	m_val = newValue;
}


QVariant
QTrackedVariant::value()
{
    SYS_DEBUG ("*** m_val.isValid() = %s",  SYS_BOOL(m_val.isValid()));
    SYS_DEBUG ("*** m_val.isNull()  = %s",  SYS_BOOL(m_val.isNull()));
	if (!m_val.isValid() || m_val.isNull())
		fetchFromBackend();

	return m_val;
}

void
QTrackedVariant::fetchFromBackend()
{
    SYS_WARNING ("This virtual function does nothing!");
}

QString
QTrackedVariant::key() const
{
	return m_key;
}

const char *
QTrackedVariant::keyChar () const
{
    return m_key.toUtf8().constData();
}

