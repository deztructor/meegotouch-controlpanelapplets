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

// FIXME: This icon is not yet specified, this is a substitute.
static const QString checkIconId = "icon-m-common-checkbox-checked";

/******************************************************************************
 * WallpaperImageLoader implementation.
 */

/*!
 * When this slot is called the loading of the thumbnails in a specific
 * interval will be initated.
 */
void
WallpaperImageLoader::loadPictures (
            const QModelIndex& firstVisibleRow, 
            const QModelIndex& lastVisibleRow)
{
#if 0
    int    from = firstVisibleRow.row();
    int    to = lastVisibleRow.row();

    SYS_DEBUG ("Between %d, %d - %d, %d", 
            firstVisibleRow.column(),
            firstVisibleRow.row(), 
            lastVisibleRow.column(),
            lastVisibleRow.row());

    for (int n = from; n <= to; ++n) {
        if (! firstVisibleRow.model ())
            break;
        //QModelIndex index(firstVisibleRow.sibling (n, 0));
        QModelIndex index = firstVisibleRow.model()->index(n, 0);
        if(!index.isValid())
            continue;

        QVariant data = index.data(WallpaperModel::WallpaperDescriptorRole);
        WallpaperDescriptor *desc = data.value<WallpaperDescriptor*>();
        
        /*
         * We only create jobs for those items that has no thumbnail in the
         * memory yet.
         */
        if (!desc->isThumbnailLoaded()) {
            Job job;
            job.desc = desc;
            job.row = index;

            m_ThumbnailLoadingJobs << job;
        }
    }

    if (m_ThumbnailLoadingJobs.count() != 0)
        QTimer::singleShot(0, this, SLOT(processJobQueue()));
#endif
}

/*!
 * When this slot is called all the thumbnail loading jobs will be cancelled.
 * This slot should be called when (1) the panning on the widget has been
 * initiated and when (2) the widget became hidden. In these cases the loading
 * of the thumbnails will be initiated again when (1) the panning stops or 
 * (2) when the widgets shown again.
 */
void 
WallpaperImageLoader::stopLoadingPictures()
{
    SYS_DEBUG ("");
    m_ThumbnailLoadingJobs.clear();
}

/*!
 * Processes one thumbnailing job and then calls itself to process the next 
 * one. While processing the job this method will only initate the thumbnailing,
 * it will not wait until the actual thumbnail is ready.
 */
void 
WallpaperImageLoader::processJobQueue ()
{
#if 0
    if (m_ThumbnailLoadingJobs.isEmpty())
        return;

    //SYS_DEBUG ("Initiating Thumbnailer");
    Job job = m_ThumbnailLoadingJobs.takeFirst();
    job.desc->initiateThumbnailer ();

    // Continue loading and letting UI thread do something
    if(m_ThumbnailLoadingJobs.count() > 0)
        QTimer::singleShot(loadPictureDelay, this, SLOT(processJobQueue()));
#endif
}

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
   
    if (desc.hasThumbnail()) {
        QPixmap thumb = desc.thumbnail ();
        QSizeF  cSize = cellSize();
        imageWidget->setPixmap (
                thumb.scaled((int)cSize.width(), (int)cSize.height()));
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
    
    // The selection. 
    imageWidget->setCurrent (desc->isCurrent());
    //imageWidget->setSelected (true);
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
    SYS_DEBUG ("dirPath = %s", SYS_STR(m_ImagesDir));
    loadFromDirectory ();
#if 0
    Q_ASSERT (logic != 0);
    
    /*
     * FIXME: Maybe we should delay this?
     */
    m_DescriptorList = logic->availableWallpapers();
    
    for (int n = 0; n < m_DescriptorList.size(); ++n) {
        connect (m_DescriptorList[n], SIGNAL (changed (WallpaperDescriptor *)),
                this, SLOT(descriptorChanged (WallpaperDescriptor *)));
    }

    connect (logic, SIGNAL(wallpaperChanged()),
            this, SLOT(wallpaperChanged()));
#endif
}

WallpaperModel::~WallpaperModel ()
{
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
#if 1
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
            retval.setValue (m_FilePathHash[m_FilePathList[index.row()]]);
            break;

        default:
            SYS_WARNING ("Unsupported role");
            retval.setValue (QString("Unsupported role"));
    }
#endif
    return retval;
}

int 
WallpaperModel::columnCount (
        const QModelIndex&) const
{
    return 1;
}


/*!
 * This slot is called when the wallpaper descriptor has been changed. 
 */
void 
WallpaperModel::descriptorChanged (
        WallpaperDescriptor *desc)
{
#if 0
    for (int n = 0; n < m_DescriptorList.size(); ++n) {
        if (m_DescriptorList[n] == desc) {
            QModelIndex first;
            first = index (n, 0);
            emit dataChanged (first, first);

            break;
        }
    }
#endif
}

/*
 * This slot is activated when the wallpaper has been changed. We emit a
 * dataChanged() signal here, so the UI can sense that and refresh itself.
 */
void 
WallpaperModel::wallpaperChanged ()
{
    /*
     * The current wallpaper is always the first, we need to refresh that.
     */
    QModelIndex first = index (0, 0);
    emit dataChanged (first, first);
}

void 
WallpaperModel::loadFromDirectory ()
{
    QSet<QString> entries;
    QStringList   removedFilePaths;
    
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
            SYS_WARNING ("Should remove: %s", SYS_STR(oldPath));
        }
    }

    if (!entries.empty()) {
        QModelIndex  parent;
        /*
         * Inserting the new items.
         */
        beginInsertRows (parent, m_FilePathList.size(), 
                m_FilePathList.size() + entries.size());

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

    SYS_DEBUG ("Between %d, %d - %d, %d", 
            firstVisibleRow.column(),
            firstVisibleRow.row(), 
            lastVisibleRow.column(),
            lastVisibleRow.row());

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
            SYS_WARNING ("Unimplemented.");
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
