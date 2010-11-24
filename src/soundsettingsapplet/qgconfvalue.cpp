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
#include "qgconfvalue.h"
#include "qgconfdirmanager.h"

//#define DEBUG
#define WARNING
#include "../debug.h"


QGConfValue::QGConfValue(const QString &key) :
	QTrackedVariant(key),
	m_notifyId(0)
{
    SYS_DEBUG ("*** key = %s", SYS_STR(key));

	m_lsDir = key.split("/", QString::SkipEmptyParts);
	m_lsDir.removeLast();
	addNotify();
}

QGConfValue::~QGConfValue()
{
	delNotify();
}

void
QGConfValue::addNotify()
{
	GError *err = NULL;

	QGConfDirManager::instance().addDir(m_lsDir);

	m_notifyId = gconf_client_notify_add(
		gconf_client_get_default(),
		keyChar(),
		(GConfClientNotifyFunc)notifyValue,
		this, NULL, &err);

	if (err) {
		g_error_free(err);
		err = NULL;
	}
}

void
QGConfValue::delNotify()
{
	if (m_notifyId != 0)
		gconf_client_notify_remove(gconf_client_get_default(), m_notifyId);
	QGConfDirManager::instance().rmDir(m_lsDir);
}

void
QGConfValue::realSetValue(const QVariant &newValue)
{
	m_val.clear();

	GError *err = NULL;
	GConfClient *cli = gconf_client_get_default();

    SYS_DEBUG ("");
	if (QVariant::Bool == newValue.type())
		gconf_client_set_bool(cli, keyChar(), newValue.toBool(), &err);
	else if (QVariant::String == newValue.type()) {
        SYS_DEBUG ("*** key() = %s", SYS_STR(key()));
		gconf_client_set_string(cli, keyChar(), newValue.toString().toUtf8().constData(), &err);
    } else
	if (QVariant::Int == newValue.type())
		gconf_client_set_int(cli, keyChar(), newValue.toInt(), &err);
	else
	if (QVariant::Double == newValue.type())
		gconf_client_set_float(cli, keyChar(), newValue.toFloat(), &err);
	else
		qWarning() << "QGConfValue::realSetValue:" << key() << ": unimplemented QGConfValue data type" << newValue;

	if (err) {
		g_error_free(err);
		err = NULL;
	}
}

void
QGConfValue::fetchFromBackend()
{
	GError *err = NULL;
	GConfValue *val;
	QVariant var;

    SYS_DEBUG ("");
	val = gconf_client_get(
		gconf_client_get_default(),
		keyChar(),
		&err);

	if (err) {
		g_error_free(err);
		err = NULL;
	}

	if (val) {
		if (GCONF_VALUE_BOOL == val->type)
			var = QVariant((bool)(gconf_value_get_bool((const GConfValue *)val)));
		else if (GCONF_VALUE_STRING == val->type) {
            SYS_DEBUG ("STRING: %s", gconf_value_get_string(val));
			var = QVariant(QString(gconf_value_get_string((const GConfValue *)val)));
        }
		else
		if (GCONF_VALUE_INT == val->type)
			var = QVariant(gconf_value_get_int((const GConfValue *)val));
		else
		if (GCONF_VALUE_FLOAT == val->type)
			var = QVariant(gconf_value_get_float((const GConfValue *)val));
		else
			qWarning() << "QGConfValue::value: unimplemented QGConfValue data type" << val->type;
		gconf_value_free(val);
	}

	if (!var.isNull())
		m_val = var;

    // FIXME: Here val is leaking out!!
}

void
QGConfValue::notifyValue(
        GConfClient *client, 
        guint connection_id, 
        GConfEntry *entry, 
        QGConfValue *val)
{
	Q_UNUSED(client)
	Q_UNUSED(connection_id)
	Q_UNUSED(entry)

	val->m_val.clear();
	val->emit_changed();
}
