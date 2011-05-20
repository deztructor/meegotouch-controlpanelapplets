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
#include "wallpaperconfiguration.h"
#include "wallpaperutils.h"

#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "gridimagewidget.h"

#include <QUrl>

#include <QTimer>
#include <MImageWidget>
#include <MProgressIndicator>


#define DEBUG
#define WARNING
#include <../debug.h>

/*
 * The intentional delay between the loading of two picture files. This gives a
 * little chance for other applications.
 */
static const int loadPictureDelay = 0;

/******************************************************************************
 * WallpaperContentItemCreator implementation.
 */
MWidget *
WallpaperCellCreator::createCell (
        const QModelIndex &index, 
        MWidgetRecycler   &recycler) const
{
    GridImageWidget *cell;

    cell = qobject_cast <GridImageWidget *> (
            recycler.take(GridImageWidget::staticMetaObject.className()));

    if (!cell) {
        cell = new GridImageWidget ();
    }

    updateCell(index, cell);

    return cell;
}

void 
WallpaperCellCreator::setCellSize (
        const QSizeF &size)
{
    m_CellSize = size;
}

QSizeF 
WallpaperCellCreator::cellSize() const
{
    return m_CellSize;
}

void 
WallpaperCellCreator::updateCell (
        const QModelIndex &index, 
        MWidget           *cell) const
{
    static QPixmap   placeholderPixmap;
    GridImageWidget *imageWidget = qobject_cast<GridImageWidget *>(cell);
    QVariant         data = index.data(WallpaperModel::WallpaperDescriptorRole);
    WallpaperDescriptor desc = data.value<WallpaperDescriptor>();
   
    SYS_DEBUG ("file = %s", SYS_STR(desc.filePath()));
    if (desc.hasThumbnail()) {
        QPixmap thumb = desc.thumbnail ();
        QSizeF  cSize = cellSize();
        imageWidget->setPixmap (
                thumb.scaled((int)cSize.width(), (int)cSize.height()));
        imageWidget->setID (desc.filePath());
        if (desc.thumbnailPending()) {
            desc.setThumbnailPending (false);
        }
            imageWidget->showAnimated();
    } else {
        /*
         * resetting the cell thumbnail pixmap. We need this because the cells
         * are re-used.
         */
        if (placeholderPixmap.size() != cellSize()) {
            QSizeF  cSize = cellSize();

            placeholderPixmap = QPixmap(
                    (int)cSize.width(), (int)cSize.height());
            placeholderPixmap.fill (QColor("black"));
        }

        imageWidget->setPixmap (placeholderPixmap);
    }
    
    /*
     * The selection. 
     */
    imageWidget->setCurrent (desc.selected());

#if 0
    // The spinner.
    if (desc->loading()) {
        imageWidget->progressIndicator(true)->show();
    } else {
        MProgressIndicator *indicator;

        indicator = imageWidget->progressIndicator(false);
        if (indicator)
            indicator->hide();
    }
#endif
}

/******************************************************************************
 * WallpaperModel implementation.
 */

WallpaperModel::WallpaperModel (
        WallpaperBusinessLogic *logic,
        QObject                *parent) :
    QAbstractTableModel (parent),
    m_BusinessLogic (logic)
{
    m_ImagesDir = Wallpaper::constructPath (Wallpaper::ImagesDir);

    loadFromDirectory ();
    ensureSelection ();
    startWatchFiles ();

    connect (m_BusinessLogic, SIGNAL(wallpaperChanged()),
            this, SLOT(wallpaperChanged()));
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

    Q_ASSERT (index.row() >= 0);
    Q_ASSERT (index.row() < m_FilePathList.size());

    switch (role) {
        case Qt::DisplayRole:
            #if 0
            /*
             * We use the base-name when sorting the images, the full path would
             * make the current image at the fix position because the current
             * image in a separate directory.
             */
            retval.setValue (m_DescriptorList[index.row()]->basename());
            #endif
            retval.setValue (m_FilePathList[index.row()]);
            break;

        case WallpaperModel::WallpaperDescriptorRole:
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
    SYS_WARNING ("UNIMPLEMENTED");
}

void 
WallpaperModel::loadFromDirectory ()
{
    QSet<QString> entries;
    QStringList   removedFilePaths;
    QStringList   itemsToRemove;

    /*
     * Getting the full list of the given directory. 
     */
    entries = Wallpaper::readDir (
            m_ImagesDir, Wallpaper::imageNameFilter());

    /*
     * Comparing the data we have with the file list we got.
     */
    foreach (QString oldPath, m_FilePathList) {
        if (!Wallpaper::isInDir (m_ImagesDir, oldPath))
            continue;

        if (entries.contains(oldPath)) {
            /*
             * Here we found a file, that we already have. This one we don't
             * need to worry about.
             */
            entries -= oldPath;
        } else {
            /*
             * Here we have a path that we have, it was in the given directory,
             * but it is not found any more.
             */
            itemsToRemove << oldPath;
        }
    }
    
    foreach (QString oldPath, itemsToRemove) {
        int idx = m_FilePathList.indexOf(oldPath);
        QModelIndex  parent;

        beginRemoveRows (parent, idx, idx);
        m_FilePathList.removeAt (idx);
        m_FilePathHash.remove (oldPath);
        endRemoveRows ();
    }

    if (!entries.empty()) {
        QModelIndex  parent;
        /*
         * Inserting the new items.
         */
        beginInsertRows (parent, m_FilePathList.size(), 
                m_FilePathList.size() + entries.size() - 1);

        foreach (QString newPath, entries) {
            WallpaperDescriptor desc (newPath);

            SYS_DEBUG ("Adding '%s'", SYS_STR(newPath));
            m_FilePathList << newPath;
            m_FilePathHash[newPath] = desc;
        }

        endInsertRows ();
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

    path = fileUri.toString();
    if (path.startsWith("file://"))
        path.remove (0, 7);

    //SYS_DEBUG ("*** flavor = %s", SYS_STR(flavor));
    //SYS_DEBUG ("*** size   = %dx%d", pixmap.width(), pixmap.height());
    //SYS_DEBUG ("*** path   = %s", SYS_STR(path));
    if (m_FilePathHash.contains(path)) {
        WallpaperDescriptor desc = m_FilePathHash[path];
        int                 idx = m_FilePathList.indexOf(path);
        QModelIndex         first;

        first = index (idx, 0);
        desc.setThumbnail (pixmap);
        SYS_WARNING ("-> %d", idx);
        emit dataChanged (first, first);
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
    SYS_WARNING ("Failed thumbnailing: %s", SYS_STR(message));
#if 0
    Q_UNUSED (message);

    for (int n = Landscape; n < NVariants; ++n) {
        if (m_Images[n].url() == fileUri) {
            bool success;

            success = m_Images[n].thumbnail (true);

            if (success) {
                emit thumbnailLoaded (this);
                emit changed (this);
            }
        }
    }
#endif
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
    QStringList  mimeTypes;

    #if 0
    SYS_DEBUG ("Between %d, %d - %d, %d", 
            firstVisibleRow.column(),
            firstVisibleRow.row(), 
            lastVisibleRow.column(),
            lastVisibleRow.row());
    #endif

    if (!firstVisibleRow.model()) {
        SYS_WARNING ("No model for the index.");
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
        QUrl                url;
        QString             mimeType;
       
        if (desc.hasThumbnail() || desc.thumbnailPending())
            continue;

        url = desc.url();
        mimeType = desc.mimeType();

        if (url.isValid() && !mimeType.isEmpty()) {
            uris      << url;
            mimeTypes << mimeType;
            desc.setThumbnailPending ();
        } else {
            SYS_WARNING ("Unimplemented: [%d/%d] '%s'/'%s'", n, index.row(),
                    SYS_STR(desc.filePath()),
                    SYS_STR(mimeType));
        }
    }

    /*
     * If we have something to ask from the thumbnailer, we are sending a
     * request.
     */
    if (!uris.isEmpty()) {
        SYS_DEBUG ("Requesting %d thumbnails.", uris.length());
        m_Thumbnailer->request (
                uris, mimeTypes, true, Wallpaper::DefaultThumbnailFlavor);
    }
}

void
WallpaperModel::stopLoadingThumbnails ()
{
    foreach (QString key, m_FilePathHash.keys())
        m_FilePathHash[key].setThumbnailPending (false);

    if (m_Thumbnailer)
        m_Thumbnailer->cancel (false);
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

    if (filePath.isEmpty())
        goto finalize;
    
    SYS_WARNING ("UNIMPLEMENTED");

finalize:
    if (retval)
        selectByFilepath (filePath);

    return retval;
}

bool 
WallpaperModel::tryAddAndSelect (
        const QString filePath)
{
    bool retval = false;

    if (filePath.isEmpty() || m_FilePathHash.contains(filePath))
        goto finalize;

    if (Wallpaper::imageFile(filePath)) {
        WallpaperDescriptor desc (filePath);
        QModelIndex  parent;

        SYS_WARNING ("Adding '%s'", SYS_STR(filePath));
        beginInsertRows (parent, m_FilePathList.size(), 
                m_FilePathList.size() + 1);

        m_FilePathList << filePath;
        m_FilePathHash[filePath] = desc;
        endInsertRows ();

        retval = true;
    }
     
finalize:
    if (retval)
        selectByFilepath (filePath);

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
        const QString &filepath)
{
    bool retval = false;

    for (int n = 0; n < m_FilePathList.size(); ++n) {
        QString              thisPath = m_FilePathList[n];
        WallpaperDescriptor  desc = m_FilePathHash[thisPath];
        bool                 selected = desc.selected();
        bool                 changed = false;

        if (selected && thisPath != filepath) {
            desc.setSelected (false);
            changed = true;
        } else if (!selected && thisPath == filepath) {
            desc.setSelected ();
            changed = true;
            retval = true;
        }

        if (changed) {
            QModelIndex          first;

            first = index (n, 0);
            emit dataChanged (first, first);
        }
    }

    return retval;
}

/******************************************************************************
 * File-system watcher implementation.
 */
void 	
WallpaperModel::directoryChanged (
        const QString  &path)
{
    SYS_DEBUG ("*** path = %s", SYS_STR(path));
    if (path != m_ImagesDir) {
        SYS_WARNING ("This is not our images directory?!");
    }

    loadFromDirectory ();
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
    m_FileWatcher->addPath (m_ImagesDir);
}

