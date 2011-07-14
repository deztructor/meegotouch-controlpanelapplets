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
****************************************************************************/

#include "wallpapermodel.h"

#include <QUrl>
#include <QTimer>
#include <MImageWidget>
#include <MProgressIndicator>

#include "wallpapercellcreator.h"
#include "wallpaperconfiguration.h"
#include "wallpaperutils.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "gridimagewidget.h"

#define DEBUG
#define WARNING
#include <../debug.h>

/*
 * The intentional delay between the loading of two picture files. This gives a
 * little chance for other applications.
 */
static const int loadPictureDelay = 0;
static const int fileSystemReCheckDelay = 500;

/******************************************************************************
 * WallpaperModel implementation.
 */
WallpaperModel::WallpaperModel (
        WallpaperBusinessLogic *logic,
        QObject                *parent) :
    QAbstractTableModel (parent),
    m_BusinessLogic (logic),
    m_ThumbnailMagicNumber (1),
    m_OrderDirty (false)
{
    m_ImagesDir = Wallpaper::constructPath (Wallpaper::ImagesDir);
    m_SysImagesDir = Wallpaper::constructPath (Wallpaper::SysImagesDir);

    m_FileSystemTimer.setInterval (fileSystemReCheckDelay);
    m_FileSystemTimer.setSingleShot (true);
    connect (&m_FileSystemTimer, SIGNAL(timeout()),
            this, SLOT(loadFromDirectory()));

    loadFromDirectory ();
    startWatchFiles ();

    connect (m_BusinessLogic, SIGNAL(wallpaperChanged()),
            this, SLOT(wallpaperChanged()));
    connect (m_BusinessLogic, SIGNAL(workerEnded()),
            this, SLOT(hideProgress()));

    #ifdef HAVE_QMSYSTEM
    m_UsbMode = new MeeGo::QmUSBMode (this);
    connect (m_UsbMode, SIGNAL(modeChanged (MeeGo::QmUSBMode::Mode)),
            this, SLOT(usbModeChanged(MeeGo::QmUSBMode::Mode)));
    #endif
        
    ensureSelection ();
}

WallpaperModel::~WallpaperModel ()
{
    if (m_Thumbnailer) {
        SYS_WARNING ("Destroying thumbler.");
        m_Thumbnailer->deleteLater ();
    }
}

int 
WallpaperModel::rowCount(
		const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    
    return m_FilePathList.size();
}

/*!
 * Returns the data from a given row in the specified format. The supported
 * roles are:
 *   Qt::DisplayRole: The title of the wallpaper is returned.
 *   otherwise: the WallpaperDescriptor object pointer is returned.
 */
QVariant
WallpaperModel::data (
        const QModelIndex &index, 
        int                role) const
{
    QVariant             retval;
    WallpaperDescriptor  desc;

    Q_ASSERT (index.row() >= 0);
    Q_ASSERT (index.row() < m_FilePathList.size());

    switch (role) {
        case Qt::DisplayRole:
            if (m_OrderDirty)
                mySort ();
            /*
             * We use this role to sort the wallpapers in the list widget.
             */
            //SYS_DEBUG ("Qt::DisplayRole");
            if (!m_FilePathHash.contains(m_FilePathList[index.row()])) {
                SYS_WARNING ("MISSING DESCRIPTOR AT %d: %s", index.row(),
                        SYS_STR(m_FilePathList[index.row()]));
            }
            
            retval.setValue (m_OrderHash[m_FilePathList[index.row()]]);
            break;

        case WallpaperModel::WallpaperDescriptorRole:
            //SYS_DEBUG ("WallpaperModel::WallpaperDescriptorRole");
            if (!m_FilePathHash.contains(m_FilePathList[index.row()])) {
                SYS_WARNING ("MISSING DESCRIPTOR AT %d: %s", index.row(),
                        SYS_STR(m_FilePathList[index.row()]));
            }
            retval.setValue (m_FilePathHash[m_FilePathList[index.row()]]);
            break;

        default:
            SYS_WARNING ("Unsupported role");
            retval.setValue (QString("Unsupported role"));
    }

    return retval;
}

int 
WallpaperModel::columnCount (
        const QModelIndex&) const
{
    return 1;
}

/*
 * This slot is activated when the wallpaper has been changed. We emit a
 * dataChanged() signal here, so the UI can sense that and refresh itself.
 */
void 
WallpaperModel::wallpaperChanged ()
{
    /*
     * Boomer, the Gallery is saving the new image under the same file name.
     * Maybe we should check the file date here?
     */
    QString currentPath;
    QString originalPath;

    m_BusinessLogic->currentWallpaper (currentPath, originalPath);
    if (m_FilePathHash.contains(currentPath)) {
        SYS_DEBUG ("Unsetting thumbnail.");
        m_FilePathHash[currentPath].unsetThumbnail();
    }

    /*
     *
     */
    ensureSelection ();
    m_OrderDirty = true;
}

void 
WallpaperModel::loadFromDirectory ()
{
    QHash<QString, qint64>  entries;
    QStringList             toAdd;
    QStringList             removedFilePaths;
    QStringList             itemsToRemove;
    QStringList             directories;

    SYS_DEBUG ("");
    directories <<
        m_SysImagesDir <<
        m_ImagesDir;

    foreach (QString directory, directories) {
        /*
         * Getting the full list of the given directory. 
         */
        SYS_DEBUG ("Reading %s", SYS_STR(directory));
        entries = Wallpaper::readDir (
            directory, Wallpaper::imageNameFilter(), entries);
        SYS_DEBUG ("*** %d entries", entries.size());
        /*
         * Comparing the data we have with the file list we got. Removing from
         * the new list data all the files that are already stored in the model.
         * We also maintain a list of files that are 
         */
        foreach (QString oldPath, m_FilePathList) {
            if (!Wallpaper::isInDir (directory, oldPath))
                continue;

            if (entries.contains(oldPath)) {
                /*
                 * Here we found a file, that we already have. This one we don't
                 * need to worry about.
                 */
                 entries.remove(oldPath);
            } else {
                /*
                 * Here we have a path that we have, it was in the given
                 * directory, but it is not found any more.
                 */
                 itemsToRemove << oldPath;
            }
        }
    } // foreach in directories
    
    /*
     * Immediately removing all the disappeared files.
     */
    foreach (QString oldPath, itemsToRemove) {
        int idx = m_FilePathList.indexOf(oldPath);
        QModelIndex  parent;

        beginRemoveRows (parent, idx, idx);
        m_FilePathList.removeAt (idx);
        m_FilePathHash.remove (oldPath);
        endRemoveRows ();
    }

    /*
     * Let's see which files are changing. We have to postpone the changing
     * files and create a storage of the files that are finished copying.
     */
    foreach (QString newPath, entries.keys()) {
        //SYS_WARNING ("%s", SYS_STR(newPath));
        //SYS_WARNING ("%lld ? %lld", m_PendingFiles[newPath], entries[newPath]);
#if 0
        if (!Wallpaper::isInDir (m_ImagesDir, newPath)) {
            toAdd << newPath;
            m_PendingFiles.remove (newPath);
            continue;
        }
#endif

        if (m_PendingFiles.contains(newPath) &&
                m_PendingFiles[newPath] == entries[newPath]) {
            /*
             * Finished copying, we are going to add these right now.
             */
             toAdd << newPath;
             m_PendingFiles.remove (newPath);
        } else {
             /*
              * Otherwise we remember to add these later.
              */
             m_PendingFiles[newPath] = entries[newPath];
        }
    }

    /*
     * Adding the files that are not changing any more.
     */
    if (!toAdd.empty()) {
        QModelIndex  parent;
        /*
         * Inserting the new items.
         */
        beginInsertRows (parent, m_FilePathList.size(), 
                m_FilePathList.size() + toAdd.size() - 1);

        foreach (QString newPath, toAdd) {
            WallpaperDescriptor desc (newPath);

            //SYS_DEBUG ("Adding '%s'", SYS_STR(newPath));
            m_FilePathList << newPath;
            m_OrderDirty = true;
            m_FilePathHash[newPath] = desc;
        }

        endInsertRows ();
    }

    if (!m_PendingFiles.isEmpty()) {
        m_FileSystemTimer.start ();
    }
}

/******************************************************************************
 * Methods to handle thumbnails.
 */
/*!
 * This slot is called when the thumbnail has been generated.
 */
void 
WallpaperModel::thumbnailReady (
            QUrl         fileUri, 
            QUrl         thumbnailUri, 
            QPixmap      pixmap, 
            QString      flavor)
{
    QString             path;

    Q_UNUSED (flavor);
    Q_UNUSED (fileUri);
    Q_UNUSED (thumbnailUri);

    if (pixmap.isNull()) {
        SYS_WARNING ("ERROR: thumbnail is null for %s",
                SYS_STR(fileUri.toString()));
        return;
    }

    path = fileUri.toString();
    if (path.startsWith("file://"))
        path.remove (0, 7);

    //SYS_DEBUG ("*** flavor = %s", SYS_STR(flavor));
    //SYS_DEBUG ("*** size   = %dx%d", pixmap.width(), pixmap.height());
    //SYS_DEBUG ("*** path   = %s", SYS_STR(path));
    if (m_FilePathHash.contains(path)) {
        WallpaperDescriptor desc = m_FilePathHash[path];
        int                 idx = m_FilePathList.indexOf(path);

        desc.setThumbnail (pixmap);
        emitChangedSignal (idx);
    } else {
        SYS_WARNING ("Descriptor path not found: %s", SYS_STR(path));
    }
}

/*!
 * This slot is called when the thumbler has been detected some error. We could
 * do something, but until we find some smart thing we can do we just print an
 * error message.
 */
void
WallpaperModel::thumbnailError (
            QString      message,
            QUrl         fileUri)
{
    QString path = fileUri.toString();

    if (path.startsWith("file://"))
        path.remove (0, 7);

    Q_UNUSED (message);
    Q_UNUSED (fileUri);
    SYS_WARNING ("Failed thumbnailing: %s: %s", 
            SYS_STR(message),
            SYS_STR(path));

    if (m_FilePathHash.contains(path)) {
        m_FilePathHash[path].setThumbnailPending (false);
    } else {
        SYS_WARNING ("Path not found.");
    }
}

/*!
 * Called when the generation of the thumbnail is finished. This is the point
 * where we can destroy the thumbler object.
 */
void 
WallpaperModel::thumbnailLoadingFinished (
            int          left)
{
    SYS_DEBUG ("*** left = %d", left);
#if 0
    Q_UNUSED (left);

    if (!m_Thumbnailer.isNull() && left == 0) {
        SYS_WARNING ("DESTROYING THUMBNAILER");
        delete m_Thumbnailer;
        m_Thumbnailer = 0;
    }
#endif
}


void
WallpaperModel::loadThumbnails (
            const QModelIndex& firstVisibleRow, 
            const QModelIndex& lastVisibleRow)
{
    QList<QUrl>  uris;
    QStringList  requestedFiles;
    QStringList  mimeTypes;
    int          start, end;

    #if 0
    SYS_DEBUG ("Between %d, %d - %d, %d", 
            firstVisibleRow.column(),
            firstVisibleRow.row(), 
            lastVisibleRow.column(),
            lastVisibleRow.row());
    #endif

    if (!firstVisibleRow.model()) {
        SYS_WARNING ("No model for the index.");
        SYS_WARNING ("*** %p", firstVisibleRow.model());
        SYS_WARNING ("*** %p", lastVisibleRow.model());
        return;
    }

    /*
     * Creating a new thumbnailer if we have to.
     */
    if (!m_Thumbnailer) {
        SYS_DEBUG ("Creating new thumbnailer...");
        m_Thumbnailer = new Thumbnailer ();
        connect (m_Thumbnailer, SIGNAL(thumbnail(QUrl,QUrl,QPixmap,QString)),
                this, SLOT(thumbnailReady(QUrl,QUrl,QPixmap,QString)) );
        connect (m_Thumbnailer, SIGNAL(error(QString,QUrl)),
                this, SLOT(thumbnailError(QString,QUrl)) );
        connect (m_Thumbnailer, SIGNAL(finished(int)),
                this, SLOT(thumbnailLoadingFinished(int)));
    }

    /*
     * Collecting uris and mime types. Please note, that the index we got here
     * is most probably the index for the proxy model, where the order of the
     * rows is very much different. This is why we need to reference the index
     * using the model stored in the index itself.
     */
    for (int n = firstVisibleRow.row(); n <= lastVisibleRow.row(); ++n) {
        QModelIndex index = firstVisibleRow.model()->index(n, 0);
        QVariant data = index.data(WallpaperModel::WallpaperDescriptorRole);
        WallpaperDescriptor desc = data.value<WallpaperDescriptor>();

        #if 0
        SYS_DEBUG ("[%02d] -> %d, %s",
                n, 
                desc.historyIndex(),
                SYS_STR(desc.filePath()));
        #endif
        appendThumbnailRequest (uris, requestedFiles, mimeTypes, desc);
    }

    /*
     * The forward thumbnails.
     */
    start = lastVisibleRow.row() + 1;
    if (start > firstVisibleRow.model()->rowCount())
        start = firstVisibleRow.model()->rowCount();
    end   = start + Wallpaper::nForwardThumbnails;
    if (end > firstVisibleRow.model()->rowCount())
        end = firstVisibleRow.model()->rowCount();

    for (int n = start; n < end; ++n) {
        QModelIndex index = firstVisibleRow.model()->index(n, 0);
        QVariant data = index.data(WallpaperModel::WallpaperDescriptorRole);
        WallpaperDescriptor desc = data.value<WallpaperDescriptor>();

        appendThumbnailRequest (uris, requestedFiles, mimeTypes, desc);
    }
    
    /*
     * The backward thumbnails.
     */
    start = firstVisibleRow.row() - Wallpaper::nBackwardThumbnails;
    if (start < 0)
        start = 0;
    end   = firstVisibleRow.row();
    if (end < 0)
        end = 0;

    for (int n = start; n < end; ++n) {
        QModelIndex index = firstVisibleRow.model()->index(n, 0);
        QVariant data = index.data(WallpaperModel::WallpaperDescriptorRole);
        WallpaperDescriptor desc = data.value<WallpaperDescriptor>();

        appendThumbnailRequest (uris, requestedFiles, mimeTypes, desc);
    }

    /*
     * If we have something to ask from the thumbnailer, we are sending a
     * request.
     */
    if (!uris.isEmpty()) {
        SYS_DEBUG ("Requesting %d thumbnails.", uris.length());
        m_Thumbnailer->request (
                uris, mimeTypes, true, Wallpaper::DefaultThumbnailFlavor);
        m_ThumbnailMagicNumber++;
    }

    /*
     * Sometimes we need to let go the thumbnails we requested before, otherwise
     * the memory consumption grow as the user pans all around in the list. This
     * only has effect when the model contains a huge number ot items, though.
     */
    if (m_ThumbnailMagicNumber % 10 == 0 &&
            nThumbnails() > Wallpaper::maxThumbnails) {
        for (int n = 0; n < m_FilePathList.size(); ++n) {
            if (requestedFiles.contains(m_FilePathList[n]))
                continue;

            if (!m_FilePathHash[m_FilePathList[n]].hasThumbnail())
                continue;

            m_FilePathHash[m_FilePathList[n]].unsetThumbnail();
            emitChangedSignal (n);
        }
    }
}

void
WallpaperModel::stopLoadingThumbnails ()
{
    if (m_Thumbnailer)
        m_Thumbnailer->cancel (false);
    
    foreach (QString key, m_FilePathHash.keys())
        m_FilePathHash[key].setThumbnailPending (false);
}

/******************************************************************************
 * Methods to handle selection and current wallpaper. 
 */
void 
WallpaperModel::ensureSelection ()
{
    QString currentPath;
    QString originalPath;

    m_BusinessLogic->currentWallpaper (currentPath, originalPath);
    SYS_DEBUG ("'%s', '%s'", SYS_STR(currentPath), SYS_STR(originalPath));

    if (trySelect(originalPath)) {
        SYS_DEBUG ("Selected the original file.");
    } else if (trySelect(currentPath)) {
        SYS_DEBUG ("Selected the current file.");
    } else if (tryAddAndSelect(originalPath)) {
        SYS_DEBUG ("Added and selected the original file.");
    } else if (tryAddAndSelect(currentPath)) {
        SYS_DEBUG ("Added and selected the current file.");
    } else {
        SYS_WARNING ("Failed?");
    }
}

bool 
WallpaperModel::trySelect (
        const QString filePath)
{
    bool retval = false;

    SYS_DEBUG ("*** filePath = '%s'", SYS_STR(filePath));
    if (filePath.isEmpty())
        goto finalize;
   
    if (!m_FilePathList.contains(filePath))
        goto finalize;

    retval = true; 

finalize:
    if (retval)
        selectByFilepath (filePath);

    SYS_DEBUG ("Returning %s", SYS_BOOL(retval));
    return retval;
}

bool 
WallpaperModel::tryAddAndSelect (
        const QString filePath)
{
    bool retval = false;

    SYS_DEBUG ("*** filePath = '%s'", SYS_STR(filePath));
    if (filePath.isEmpty() || m_FilePathHash.contains(filePath))
        goto finalize;

    if (Wallpaper::imageFile(filePath)) {
        WallpaperDescriptor desc (filePath);
        QModelIndex  parent;

        /*
         * If this file is pending we should remove it from the pending files.
         */
        if (m_PendingFiles.contains(filePath))
            m_PendingFiles.remove (filePath);

        SYS_WARNING ("beginInsertRows (..., %d, %d)", 
                m_FilePathList.size(),
                m_FilePathList.size());
        beginInsertRows (parent, m_FilePathList.size(), m_FilePathList.size());

        m_FilePathList << filePath;
        m_OrderDirty = true;
        m_FilePathHash[filePath] = desc;
        SYS_WARNING ("endInsertRows()");
        endInsertRows ();

        retval = true;
    }
     
finalize:
    if (retval) {
        selectByFilepath (filePath);
    }

    SYS_DEBUG ("Returning %s", SYS_BOOL(retval));
    return retval;
}

/*!
 * \returns true if the item with the given path is indeed found
 *
 * Selects the item given by its file path and deselects all the other items.
 * Emits a signal for every changed item, so the widget will re-draw the parts
 * that needed to be updated.
 */
bool
WallpaperModel::selectByFilepath (
        const QString &filePath)
{
    bool retval = false;

    SYS_DEBUG ("*** filePath = '%s'", SYS_STR(filePath));

    for (int n = 0; n < m_FilePathList.size(); ++n) {
        QString              thisPath = m_FilePathList[n];
        WallpaperDescriptor  desc = m_FilePathHash[thisPath];
        bool                 selected = desc.selected();
        bool                 changed = false;
        bool                 thisIsSelected;

        if (selected && thisPath != filePath) {
            changed = true;
            thisIsSelected = false;
        } else if (!selected && thisPath == filePath) {
            thisIsSelected = true;
            changed = true;
            retval = true;
        }

        if (changed) {
            QModelIndex          first;

            desc.setSelected (thisIsSelected);
            first = index (n, 0);
            SYS_DEBUG ("emit dataChanged(%d, %d)", n, n);
            if (!thisIsSelected)
                emit dataChanged (first, first);
        }
    }

    //if (retval)
        QTimer::singleShot(100, this, SLOT(emitCurrentChanged()));
    
    return retval;
}

/******************************************************************************
 * File-system watcher and USB mode methods.
 */
#ifdef HAVE_QMSYSTEM
#ifdef DEBUG 
static QString 
usbModeName (QmUSBMode::Mode mode)
{
    switch (mode) {
        case QmUSBMode::Connected:
            return "QmUSBMode::Connected";

        case QmUSBMode::DataInUse:
            return "QmUSBMode::DataInUse";
        
        case QmUSBMode::Disconnected:
            return "QmUSBMode::Disconnected";
        
        case QmUSBMode::MassStorage:
            return "QmUSBMode::MassStorage";
        
        case QmUSBMode::ChargingOnly:
            return "QmUSBMode::ChargingOnly";
        
        case QmUSBMode::OviSuite:
            return "QmUSBMode::OviSuite";

        case QmUSBMode::ModeRequest:
            return "QmUSBMode::ModeRequest";
        
        case QmUSBMode::Ask:
            return "QmUSBMode::Ask";
#ifndef MEEGO
        case QmUSBMode::SDK:
            return "QmUSBMode::SDK";
#endif
        case QmUSBMode::Undefined:
            return "QmUSBMode::Undefined";
        
        default:
            return "QmUSBMode::UnknownMode!";
    }
}
#endif

void 
WallpaperModel::usbModeChanged (
        MeeGo::QmUSBMode::Mode mode)
{
    SYS_DEBUG ("Usbmode = %s", SYS_STR(usbModeName(mode)));
    
    /*
     * FIXME: Well, maybe we can find a robust solution for this issue?
     */
    if (mode == QmUSBMode::Undefined)
        QTimer::singleShot (5000, this, SLOT(loadFromDirectory()));
    else if (mode == QmUSBMode::MassStorage)
        loadFromDirectory ();

    ensureSelection ();
}
#endif

void 	
WallpaperModel::directoryChanged (
        const QString  &path)
{
    SYS_DEBUG ("*** path = %s", SYS_STR(path));
    if (path != m_ImagesDir) {
        SYS_WARNING ("This is not our images directory?!");
    }

    if (!m_FileSystemTimer.isActive())
        m_FileSystemTimer.start ();
    //loadFromDirectory ();

    // FIXME: ensureSelection() ?
}

void
WallpaperModel::startWatchFiles ()
{

    if (!m_FileWatcher) {
        SYS_DEBUG ("Creating file system watcher.");
        m_FileWatcher = new QFileSystemWatcher (this);
        connect (m_FileWatcher, SIGNAL(directoryChanged(const QString &)),
                this, SLOT(directoryChanged(const QString &)));
    }

    SYS_DEBUG ("Watching directory %s", SYS_STR(m_ImagesDir));
    /* No need to listen on SysImagesDir as it is on the rootfs */
    m_FileWatcher->addPath (m_ImagesDir);
}

/******************************************************************************
 * Low level private methods.
 */
void
WallpaperModel::emitChangedSignal (
        int    idx)
{
    QModelIndex         first;
    first = index (idx, 0);
    emit dataChanged (first, first);
}

void
WallpaperModel::emitCurrentChanged ()
{
    QModelIndex index;

    SYS_WARNING ("Emitting currentChanged()");

    index = currentIndex();
    emit dataChanged (index, index);
    emit currentChanged (index);
}


QModelIndex
WallpaperModel::currentIndex () 
{
    QModelIndex retval;

    for (int n = 0; n < m_FilePathList.size(); ++n) {
        WallpaperDescriptor  desc = m_FilePathHash[m_FilePathList[n]];
        bool                 selected = desc.selected();

        if (!selected)
            continue;

        retval = index(n, 0);
        break;
    }

    return retval;
}
        
void
WallpaperModel::showProgressByFilepath (
        const QString  &path)
{
    SYS_DEBUG ("*** path = %s", SYS_STR(path));
    for (int n = 0; n < m_FilePathList.size(); ++n) {
        QString              thisPath = m_FilePathList[n];
        WallpaperDescriptor  desc = m_FilePathHash[thisPath];
        bool                 changed = false;

        if (desc.progress() && thisPath != path) {
            desc.setProgress (false);
            changed = true;
        } else if (!desc.progress() && thisPath == path) {
            desc.setProgress (true);
            changed = true;
        }

        if (changed) {
            QModelIndex          first;
            
            first = index (n, 0);
            emit dataChanged (first, first);
        }
    }
}

void
WallpaperModel::hideProgress ()
{
    for (int n = 0; n < m_FilePathList.size(); ++n) {
        WallpaperDescriptor  desc = m_FilePathHash[m_FilePathList[n]];
        bool                 changed = false;

        if (desc.progress()) {
            desc.setProgress (false);
            changed = true;
        }

        if (changed) {
            QModelIndex          first;
            
            first = index (n, 0);
            emit dataChanged (first, first);
        }
    }
}

int
WallpaperModel::nThumbnails () const
{
    int retval = 0;

    foreach (QString path, m_FilePathList) {
        WallpaperDescriptor desc = m_FilePathHash[path];

        if (desc.hasThumbnail() || desc.thumbnailPending())
            ++retval;
    }

    SYS_WARNING ("Returning %d", retval);
    return retval;
}

bool
WallpaperModel::appendThumbnailRequest (
        QList<QUrl>          &uris,
        QStringList          &requestedFiles,
        QStringList          &mimeTypes,
        WallpaperDescriptor  &desc)
{
    QUrl                url;
    QString             mimeType;
    bool                retval = true;

    //SYS_DEBUG ("*** %s %s", 
    //        SYS_BOOL(desc.hasThumbnail()),
    //        SYS_STR(desc.filePath()));

    if (!requestedFiles.contains(desc.filePath()))
        requestedFiles << desc.filePath();

    if (desc.hasThumbnail() || desc.thumbnailPending())
        goto finalize;

    url = desc.url();
    mimeType = desc.mimeType();

    if (url.isValid() && !mimeType.isEmpty()) {
        uris      << url;
        mimeTypes << mimeType;
        desc.setThumbnailPending ();
    } else {
        SYS_WARNING ("Dropping, not found?");
        retval = false;
    }

finalize:
        return retval;
}

void 
WallpaperModel::mySort () const
{
    QList<WallpaperDescriptor> list;
    QStringList                history = m_BusinessLogic->history();

    if (!m_OrderDirty) {
        SYS_WARNING ("This method should not called when the order is not "
                "dirty.");
        return;
    }
    
    foreach (WallpaperDescriptor desc, m_FilePathHash) {
        desc.setHistoryIndex (history.indexOf(desc.filePath()));
        list << desc;
    }

    qSort (list.begin(), list.end());

    m_OrderHash.clear();
    for (int n = 0; n < list.size(); ++n)
        m_OrderHash[list[n].filePath()] = n;

    m_OrderDirty = false;
}

