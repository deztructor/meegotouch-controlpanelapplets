/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
** Authors: David Kedves <dkedves@blumsoft.eu>
**          Laszlo Pere <lpere@blumsoft.eu>
**
****************************************************************************/
#include "wallpaperbusinesslogic.h"
#include "wallpaperutils.h"
#include "wallpaperconfiguration.h"

#include "wallpapergconf.h"
#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"
#include "wallpapereditorsheet.h"
#include "wallpaperworkerthread.h"

#include <QString>
#include <QStringList>
#include <QProcessEnvironment>
#include <QPainter>
#include <QDBusConnection>
#include <QDBusError>
#include <QFile>
#include <MTheme>
#include <MGConfItem>
#include <MApplication>
#include <MApplicationWindow>

#define DEBUG
#define WARNING
#include "../debug.h"

WallpaperBusinessLogic::WallpaperBusinessLogic (
        QObject     *parent) :
    QObject (parent),
    m_EditRequested (false),
    m_WorkerThread (0)
{
    m_PPItem = new MGConfItem (Wallpaper::CurrentPortraitKey, this);
    m_POItem = new MGConfItem (Wallpaper::OriginalPortraitKey, this);
    m_PHItem = new MGConfItem (Wallpaper::PortraitHistoryKey, this);

    connect (m_PPItem, SIGNAL(valueChanged ()),
            this, SLOT(portraitGConfChanged()));

    connect (m_POItem, SIGNAL(valueChanged ()),
            this, SLOT(portraitGConfChanged()));
    
    connect (m_PHItem, SIGNAL(valueChanged ()),
            this, SLOT(portraitHistoryChanged()));
   
    /*
     * A timer to handle multiple GConf keys as one by filtering multiple value
     * change events.
     */
    m_GConfTimer.setInterval (200);
    m_GConfTimer.setSingleShot (true);
    connect (&m_GConfTimer, SIGNAL(timeout()),
            this, SLOT(gconfTimerSlot()));
    
    /*
     * Reading and caching the history.
     */
    portraitHistoryChanged ();
}

WallpaperBusinessLogic::~WallpaperBusinessLogic()
{
}



void
WallpaperBusinessLogic::startEdit (
        WallpaperDescriptor desc)
{
    SYS_DEBUG ("");
    if (!m_EditedImage.isNull()) {
        SYS_DEBUG ("We already have an edited image, ignoring this request.");
        return;
    }

    m_EditedImage   = desc;
    m_EditRequested = true;
    loadImage (m_EditedImage);
}

void
WallpaperBusinessLogic::endEdit ()
{
    m_EditedImage = WallpaperDescriptor ();
}

bool
WallpaperBusinessLogic::setWallpaper ()
{
    QString  filePath = m_EditedImage.filePath();
    QString  basename = Wallpaper::baseName (filePath);
    QFile    origFile (filePath);
    QString  targetPath;
    bool     success;

    /*
     * Checking if our output directory exists, creating if not.
     */
    targetPath = Wallpaper::constructPath (Wallpaper::ImagesSaveDir);
    success = Wallpaper::ensureHasDirectory (targetPath);
    if (!success)
        goto finalize;

    /*
     *
     */
    targetPath = Wallpaper::constructPath (targetPath, basename);
    
    if (filePath == targetPath) {
        SYS_WARNING ("This is the same file!");
        targetPath = Wallpaper::setFileVariant (targetPath);
    }
    
    SYS_DEBUG ("*** basename   = %s", SYS_STR(basename));
    SYS_DEBUG ("*** targetPath = %s", SYS_STR(targetPath));
    SYS_DEBUG ("*** filePath   = %s", SYS_STR(filePath));

    m_WorkerThread = new WallpaperWorkerThread (filePath, targetPath);
    connect (m_WorkerThread, SIGNAL(finished()), 
            this, SLOT(workerThreadFinishedSave()), Qt::QueuedConnection);
    success = true;
    m_WorkerThread->start();

finalize:
    return success;
}

bool
WallpaperBusinessLogic::setWallpaper (
        QPixmap   &pixmap)
{
    QString  filePath = m_EditedImage.filePath();
    QString  basename = Wallpaper::baseName (filePath);
    QString  targetPath;
    bool     success = false;

    if (m_WorkerThread) {
        SYS_WARNING ("Worker thread is already there, giving up.");
        goto finalize;
    }

    /*
     * Checking if our output directory exists, creating if not.
     */
    targetPath = Wallpaper::constructPath (Wallpaper::ImagesSaveDir);
    success = Wallpaper::ensureHasDirectory (targetPath);
    if (!success)
        goto finalize;

    basename = Wallpaper::setFileExtension (basename, Wallpaper::OutImgExt);
    targetPath = Wallpaper::constructPath (targetPath, basename);

    m_WorkerThread = new WallpaperWorkerThread (pixmap, filePath, targetPath);
    connect (m_WorkerThread, SIGNAL(finished()), 
            this, SLOT(workerThreadFinishedSave()), Qt::QueuedConnection);
    success = true;
    m_WorkerThread->start();

finalize:
    return success;
}

bool
WallpaperBusinessLogic::loadImage (
        WallpaperDescriptor &desc)
{
    bool       success;

    if (m_WorkerThread) {
        SYS_WARNING ("Worker thread is already there, giving up.");
        goto finalize;
    }

    /*
     * The worker thread might want to open the original file so we don't get
     * the black frames around the image while editing.
     */
    if (desc.selected()) {
        QString currentFilePath;
        QString originalFilePath;

        currentWallpaper (currentFilePath, originalFilePath);
        if (currentFilePath == desc.filePath())
            desc.setOriginalFilePath (originalFilePath);
    } else {
        desc.setOriginalFilePath (QString());
    }

    emit workerStarted ();
    m_WorkerThread = new WallpaperWorkerThread (desc, sceneSize());
    connect (m_WorkerThread, SIGNAL(finished()), 
            this, SLOT(workerThreadFinishedLoad()), Qt::QueuedConnection);
    success = true;
    m_WorkerThread->start();

finalize:
    return success;
}

QSize
WallpaperBusinessLogic::sceneSize () const
{
    MWindow   *win;
    QSize      retval;

    win = MApplication::activeWindow ();
    if (win) {
        retval = win->visibleSceneSize (M::Portrait);
    } else {
        SYS_WARNING ("Using default size.");
        retval = QSize (480, 854);
    }

    return retval;
}


/*!
 * Returns the wallpaper that is being edited.
 */
WallpaperDescriptor 
WallpaperBusinessLogic::editedImage () const
{
    return m_EditedImage;
}

bool
WallpaperBusinessLogic::hasEditedImage () const
{
    return !m_EditedImage.isNull();
}


/******************************************************************************
 * Handling the GConf database.
 */
void
WallpaperBusinessLogic::gconfTimerSlot ()
{
    SYS_DEBUG ("\n\n\n\n-----------------------------------------------");
    emit wallpaperChanged ();
}

void
WallpaperBusinessLogic::portraitGConfChanged ()
{
    if (!m_GConfTimer.isActive()) {
        m_GConfTimer.start();
    }
}

void
WallpaperBusinessLogic::currentWallpaper (
        QString   &currentFilePath,
        QString   &originalFilePath)
{
    QStringList origList = m_POItem->value().toStringList ();

    /*
     * The current file path.
     */
    currentFilePath  = m_PPItem->value().toString ();
    if (currentFilePath.isEmpty())
        currentFilePath = Wallpaper::CurrentPortraitDefault;

    currentFilePath  = Wallpaper::logicalIdToFilePath (currentFilePath);

    /*
     * The original file path.
     */
    if (origList.size() == 2 && origList[0] == currentFilePath) {
        originalFilePath = Wallpaper::logicalIdToFilePath (origList[1]);
    } else {
        originalFilePath = QString();
    }

    SYS_DEBUG ("*** currentFilePath  = %s", SYS_STR(currentFilePath));
    SYS_DEBUG ("*** originalFilePath = %s", SYS_STR(originalFilePath));
}

void 
WallpaperBusinessLogic::workerThreadFinishedSave ()
{
    bool success = m_WorkerThread->success();
    SYS_DEBUG ("*** success  = %s", SYS_BOOL(success));

    if (success) {
        QString origFile = m_WorkerThread->originalFileName ();
        QString outputFile = m_WorkerThread->outputFileName ();
        QStringList origList;

        origList << outputFile << origFile;
        SYS_DEBUG ("*** original = %s", SYS_STR(origFile));
        SYS_DEBUG ("*** output   = %s", SYS_STR(outputFile));

        /*
         * If we save the same value the lockscreen will not recognize the
         * change...
         */
        if (m_PPItem->value().toString() == outputFile)
            m_PPItem->set ("");

        m_POItem->set (origList);
        prependFileToHistory (origFile);
        m_PPItem->set (outputFile);
    }

    emit wallpaperSaved ();

    delete m_WorkerThread;
    m_WorkerThread = 0;
}

void 
WallpaperBusinessLogic::workerThreadFinishedLoad ()
{
    bool success = m_WorkerThread->success();

    SYS_DEBUG ("*** success  = %s", SYS_BOOL(success));

    if (m_EditRequested) {
        if (success)
            emit editWallpaper (m_EditedImage);
        else 
            endEdit ();

        m_EditRequested = false;
    }

    if (success)
        emit wallpaperLoaded (
                m_WorkerThread->image(),
                m_WorkerThread->size());

    delete m_WorkerThread;
    m_WorkerThread = 0;

    emit workerEnded ();
}

/******************************************************************************
 * Methods to handle the history.
 */
void
WallpaperBusinessLogic::prependFileToHistory (
        const QString &path)
{
    QStringList  historyList;

    historyList = m_PHItem->value().toStringList();

    if (historyList.contains(path))
        historyList.removeAt (historyList.indexOf(path));

    while (historyList.size() >= Wallpaper::historyMaxLength)
        historyList.takeLast();

    historyList.prepend (path);

    m_PHItem->set (historyList);
}

void
WallpaperBusinessLogic::portraitHistoryChanged ()
{
    m_PortraitHistory =  m_PHItem->value().toStringList();
}

QStringList
WallpaperBusinessLogic::history () const
{
    return m_PortraitHistory;
}
