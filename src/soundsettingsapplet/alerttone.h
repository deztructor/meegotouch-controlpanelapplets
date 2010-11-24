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

#ifndef _ALERT_TONE_H_
#define _ALERT_TONE_H_

#include "qprofilevalue.h"

class AlertTone: public QProfileValue
{
    Q_OBJECT

public:
	AlertTone(const QString &key);

	static QList<AlertTone *> alertTones();

	QString fileName();
	QString niceName();
	QString trackerId();

protected:
	virtual void fetchFromBackend();
	virtual void realSetValue(const QVariant &newValue);

signals:
    void refreshed();
    
private slots:
    void dataReceived (
            const QString   &filename, 
            const QString   &title,
            const QString   &trackerId);

private:
	void maybeUpdate();
	QString m_niceName;
	QString m_trackerId;

#ifdef UNIT_TEST
    friend class Ut_AlertToneTests;
#endif
};

#endif /* !_ALERT_TONE_H_ */
