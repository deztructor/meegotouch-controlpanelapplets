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
#ifndef TRACKERCONNECTION_H
#define TRACKERCONNECTION_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QtTracker/Tracker>

/*!
 * Singleton class to implement an asynchronous tracker connection with 
 * caching.
 *
 * Please note that it is important not to emit the dataReady() signal unless it
 * is necessary for there might be objects that process the signal slowli (e.g.
 * the AlertToneDefaultsModel() that is doing a sequential search for every
 * signal. This means we only send at most one signal for every filename!
 */
class TrackerConnection : public QObject
{
	Q_OBJECT

public:
    ~TrackerConnection ();
    static TrackerConnection *instance ();

    QString niceNameFromFileName (const QString &fileName);
    QString trackerIdFromFileName (const QString &fileName);

protected:
    TrackerConnection ();
    QString poorNiceName (const QString &filename);

signals:
    void dataReady (
            const QString   &filename, 
            const QString   &title,
            const QString   &trackerId);

private slots:
    void processPendingRequests();
    void processRequest (const QString &fileName);

private:
    static TrackerConnection    *s_Instance;
    QHash<QString, QString>      m_NiceNameCache;
    QHash<QString, QString>      m_TrackerIdCache;
    QStringList                  m_PendingRequests;
};
#endif

