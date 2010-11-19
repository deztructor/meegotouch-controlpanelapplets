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

#ifndef _Q_TRACKED_VARIANT_H_
#define _Q_TRACKED_VARIANT_H_

#include <QObject>
#include <QString>
#include <QVariant>

/*!
 * A simple class that is used as a variable that have a constant name (it is
 * called the 'key' here) and a value with an arbitrary type. The class has a
 * signal to be emitted when the value is changed. 
 */
class QTrackedVariant : public QObject
{
	Q_OBJECT

public:
	QTrackedVariant(const QString &key);

	void set(const QVariant &newValue);
	QVariant value();

	QString key() const;
    const char *keyChar () const;

Q_SIGNALS:
	void changed();

protected:
	virtual void emit_changed();
	virtual void realSetValue(const QVariant &newValue);
	virtual void fetchFromBackend();

	QVariant m_val;

private:
	/*
     * This tag is and should be a constant, since it is not private only
     * protected and the inherited classes are not handling the case when the
     * key has been changed. And indeed the key is never changed. 
     */
    const QString m_key;

#ifdef UNIT_TEST
    friend class Ut_TrackedVariantTests;
    friend class Ut_AlertToneDefaultsTests;
    friend class Ut_AlertTonePreviewTests;
    friend class Ut_AlertToneVolumeTests;
    friend class Ut_AlertToneWidgetTests;
    friend class Ut_ProfileintComboTests;
#endif
};

#endif /* !_Q_TRACKED_VARIANT_H_ */
