/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _ALERT_TONE_APPLET_MAPS_H_
#define _ALERT_TONE_APPLET_MAPS_H_

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <meegocontrolexport.h>

class MC_EXPORT AlertToneAppletMaps
{
public:
	static const AlertToneAppletMaps &instance();
	AlertToneAppletMaps();


    static QVariant mapToUiString (const QString &str);
    static QVariant mapToUiString (int integer);

private:
	QString map(const QString &str) const;
	QString map(const int &i) const;

	static const AlertToneAppletMaps maps;
	QMap<QString, const char *> m_map;
    #ifdef UNIT_TEST
    friend class Ut_AlertToneAppletMapsTests;
    #endif
};

#endif /* !_ALERT_TONE_APPLET_MAPS_H_ */
