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
#ifndef QGCONFVALUE_H
#define QGCONFVALUE_H

#include <QStringList>
#include "qtrackedvariant.h"

class QGConfValue : public QTrackedVariant
{
public:
	QGConfValue(const QString &key);
	~QGConfValue();

private:
	static void notifyValue(void *client, quint32 connection_id, void *entry, QGConfValue *val);

	void addNotify();
	void delNotify();
	void realSetValue(const QVariant &newValue);
	void fetchFromBackend();

	quint32 m_notifyId;
	QStringList m_lsDir;
#ifdef UNIT_TEST
    friend class Ut_QGConfValueTests;
    friend class Ut_GConfStringComboTests;
#endif
};
#endif
