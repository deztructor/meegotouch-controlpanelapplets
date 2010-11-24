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

#include <QTimer>
#include <QFile>

#include "trackerconnection.h"
#include "alerttonedefaultsmodel.h"
#include "alerttone.h"
#include "static.h"

/*
 * Define this macro to add items asynchronously.
 */
#define USE_ASYNC_LOADING

//#define DEBUG
#define WARNING
#include "debug.h"

AlertToneDefaultsModel::AlertToneDefaultsModel() : QStandardItemModel(),
	m_isFinished(false)
{
	insertColumn(NiceNameColumn);
	insertColumn(FullPathColumn);
	insertColumn(ForcedColumn);

	m_dirStack.push(QDir(DEFAULT_RINGTONE_PATH1));
	m_dirIdx.push(0);
	m_dirStack.push(QDir(DEFAULT_RINGTONE_PATH2));
	m_dirIdx.push(0);

    /*
     * Creatig a file system watcher.
     */
    m_FileWatcher = new QFileSystemWatcher (this);
    connect (m_FileWatcher, SIGNAL(fileChanged(const QString &)),
            this, SLOT(fileChanged(const QString &)));

    /*
     * TrackerConnection might send a signal about the tracker answer.
     */
    connect (TrackerConnection::instance(), 
            SIGNAL(dataReady(QString,QString,QString)),
            this, 
            SLOT(dataReceived(QString,QString,QString)));

    #ifdef USE_ASYNC_LOADING
    /*
     * We need to give the UI a chance to process the tap events, but we also
     * want to be as fast as possible... this is not a perfect solution though.
     */
	QTimer::singleShot(20, this, SLOT(addSingleItem()));
    #else 
	while (!m_isFinished)
        addSingleItem();
    #endif
}

void
AlertToneDefaultsModel::addSingleItem()
{
    SYS_DEBUG ("*** m_isFinished = %s", SYS_BOOL(m_isFinished));
	if (m_isFinished) 
        return;

    while (m_dirStack.count() > 0) {
        if (m_dirIdx.top() >= m_dirStack.top().count()) {
            m_dirIdx.pop();
            m_dirStack.pop();
        } else {
            break;
        }
    }

    if (m_dirStack.count() <= 0) {
        m_isFinished = true;
        emit finished();
        return;
	} 

    #if 1
    SYS_DEBUG ("*** path        = %s", 
            SYS_STR(m_dirStack.top().absolutePath()));
    SYS_DEBUG ("***             = %s", 
            SYS_STR(m_dirStack.top()[m_dirIdx.top()]));
    #endif
    int currentLevel = m_dirIdx.count() - 1;
    if (m_dirStack.top()[m_dirIdx.top()] != "." &&
            m_dirStack.top()[m_dirIdx.top()] != "..") {
        QString fullPath = m_dirStack.top().absolutePath() + 
                QDir::separator() + m_dirStack.top()[m_dirIdx.top()];

		QDir subdir(fullPath);

		if (subdir.count() > 0) {
			m_dirStack.push(subdir);
			m_dirIdx.push(0);
		} else if (m_dirStack.top()[m_dirIdx.top()].right(4) == ".aac" &&
					m_dirStack.top()[m_dirIdx.top()].left(1) == m_dirStack.top()[m_dirIdx.top()].left(1).toUpper()) {
            QString niceName = 
                TrackerConnection::instance()->niceNameFromFileName (fullPath);
            SYS_DEBUG ("calling niceNameFromFileName()");
            addSingleItem (niceName, fullPath);
        }
    }

    m_dirIdx[currentLevel]++;

    #ifdef USE_ASYNC_LOADING
    QTimer::singleShot(20, this, SLOT(addSingleItem()));
    #endif
}

/*!
 * \param niceName The human readable name for the new item
 * \param fileName The full path file name for the new entry
 * \param forced Should be true when the item is not found in the filesystem but
 *   inserted for some other reasons (maybe coming from the content picker).
 *
 * This is the method that should be used to add a new item into the model once
 * the file name and the human readable name is known. No other function should
 * be created to add new row to the model.
 */
int
AlertToneDefaultsModel::addSingleItem (
            const QString &niceName, 
            const QString &fileName,
            bool           forced)
{
    int      row = rowCount();
    /*
     *
     */
    emit loading();

    /*
     * Adding the file to the watcher: we should not get duplications added to
     * the model, so we don't need to check if the file is already added to the
     * watcher.
     */
    m_FileWatcher->addPath (fileName);    


    /**************************************************************************
     * Adding the new row.
     */
    insertRow(row);

    /*
     * Last minute checking, just to be sure.
     */
    if (!niceName.isEmpty()) {
        setData (index(row, NiceNameColumn), QVariant(niceName));
    } else {
        QString myNiceName = 
            TrackerConnection::instance()->niceNameFromFileName (fileName);
        setData (index(row, NiceNameColumn), QVariant(myNiceName));
    }
    
    setData (index(row, FullPathColumn), QVariant(fileName));
    setData (index(row, ForcedColumn), QVariant(forced));

    return row;
}

/*!
 * This slot is called when the tracker connection sends a signal about a
 * tracker answer to send the nince name (title9 of a particular item.
 */
void 
AlertToneDefaultsModel::dataReceived (
            const QString   &filename, 
            const QString   &title,
            const QString   &trackerId)
{
    Q_UNUSED (trackerId);
    int Idx = findItemByFileName (filename);

    SYS_DEBUG ("*** filename = %s", SYS_STR(filename));
    SYS_DEBUG ("*** Idx      = %d", Idx);
    if (Idx >= 0)
        setData (index(Idx, NiceNameColumn), QVariant(title));
}

/*!
 * \returns The item index for the given file name or -1 if not found.
 */
int 
AlertToneDefaultsModel::findItemByFileName (
        const QString &FileName) const
{
    for (int n = 0; n < rowCount(); ++n) {
        if (fileName(n) == FileName)
            return n;
    }

    return -1;
}

/*!
 * \returns The item index for the given nice name or -1 if not found.
 */
int 
AlertToneDefaultsModel::findItemByNiceName (
        const QString &NiceName) const
{
    for (int n = 0; n < rowCount(); ++n) {
        if (niceName(n) == NiceName)
            return n;
    }

    return -1;
}

/*
 * We call this function when the whole model needs to be refreshed on the
 * screen, for example when the highlight is changed because the user typed a
 * search string.
 */
void
AlertToneDefaultsModel::refresh ()
{
    int rows = rowCount();

    if (rows != 0) {
        QModelIndex first = index (0, 0);
        QModelIndex last  = index (rows - 1, 0);

        emit dataChanged (first, last);
    }
}

/*!
 * Helper function to return the nice name (human readable name) for a given
 * item.
 */
QString
AlertToneDefaultsModel::niceName (
        const QModelIndex &item) const
{
    QModelIndex myItem = index (item.row(), NiceNameColumn);
    return data(myItem).toString ();
}



/*!
 * Helper function to return the filename (full path name) for a given item.
 */
QString
AlertToneDefaultsModel::fileName (
        const QModelIndex &item) const
{
    QModelIndex myItem = index (item.row(), FullPathColumn);
    return data(myItem).toString ();
}

/*!
 * Helper function to return the filename (full path name) for a given item.
 */
QString
AlertToneDefaultsModel::fileName (
        int indexRow) const
{
    QModelIndex myItem = index (indexRow, FullPathColumn);
    return data(myItem).toString ();
}

/*!
 * Helper function to return the nice name (human readable name) for a given
 * item.
 */
QString
AlertToneDefaultsModel::niceName (
        int indexRow) const
{
    QModelIndex myItem = index (indexRow, NiceNameColumn);
    return data(myItem).toString ();
}

bool 
AlertToneDefaultsModel::isFinished () const
{
    return m_isFinished;
}

/*!
 * This slot is called when the filesystem watcher reports a file to be changed.
 * In this case we are going to remove the referencing row from the model, so
 * the file will disappear from the list widget.
 * Fortunately this file system change is also monitored by the QProfileValue
 * object, and that will handle the selection that will be changing to the
 * default item when the currently set sound file is removed.
 */
void
AlertToneDefaultsModel::fileChanged (
        const QString &filename)
{
    QFile thisFile (filename);
    bool  exists = thisFile.exists (filename);
    int   idx;

    SYS_DEBUG ("*** filename = %s", SYS_STR(filename));

    /*
     * If the file still exists we don't need to do nothing.
     */
    if (exists)
        return;

    /*
     * Removing the referencing row from the model.
     */
    idx = findItemByFileName (filename);
    if (idx >= 0)
        removeRows (idx, 1);

    /*
     * We are not watching the file any more.
     */
    m_FileWatcher->removePath (filename);
}


#ifdef ALERT_TONE_DEFAULTS_MODEL_RUN_STANDALONE
#include <MApplication>
#include "alerttonedefaultsmodel.h"

int
main(int argc, char **argv)
{
	MApplication app(argc, argv);

	AlertToneDefaultsModel atdm;

	return app.exec();
}
#endif /* ALERT_TONE_DEFAULTS_MODEL_RUN_STANDALONE */
