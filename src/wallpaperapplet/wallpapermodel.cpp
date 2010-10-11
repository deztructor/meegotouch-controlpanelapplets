/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"

#ifdef USE_IMAGE_WIDGET
#  include "gridimagewidget.h"
#endif

#include <QTimer>
#include <MImageWidget>
#include <MProgressIndicator>

//#define DEBUG
#include <../debug.h>

/*
 * The intentional delay between the loading of two picture files. This gives a
 * little chance for other applications.
 */
static const int loadPictureDelay = 100;

// FIXME: This icon is not yet specified, this is a substitute.
static const QString checkIconId = "icon-m-common-checkbox-checked";
static const QString imagePlaceHolderIconId = "icon-m-content-not-loaded";

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
    int    from = firstVisibleRow.row();
    int    to = lastVisibleRow.row();

    SYS_DEBUG ("Between %d, %d - %d, %d", 
            firstVisibleRow.column(),
            firstVisibleRow.row(), 
            lastVisibleRow.column(),
            lastVisibleRow.row());
    for (int n = from; n < to; ++n) {
        QModelIndex index(firstVisibleRow.sibling (n, 0));
        if(!index.isValid())
            continue;

        QVariant data = index.data(Qt::DisplayRole);
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
 * This slot is called when the loading of a specific thumbnail has been
 * finished. The thumbnailer is a separate process that caches the created
 * thumbnails, so the thumbnailing can happen fast (when the thumbnail is
 * already created and saved) or slow (when the thumbnail has to be created).
 */
void
WallpaperImageLoader::thumbnailLoaded (
        WallpaperDescriptor *desc)
{
    /*
     * The pending jobs are in order, the first initated is at the very first
     * position of the list. The thumbler answers however might come in a
     * diefferent order, it is possible, that the generation of some thumbnails
     * is slower than the others.
     */
    SYS_DEBUG ("");
    for (int n = 0; n < m_ThumbnailPendingJobs.size(); ++n) {
        if (m_ThumbnailPendingJobs[n].desc == desc) {
            WallpaperModel *model = (WallpaperModel*) 
                m_ThumbnailPendingJobs[n].row.model();

            model->imageLoaded (m_ThumbnailPendingJobs[n].row);
            m_ThumbnailPendingJobs.removeAt (n);

            disconnect (desc, 0, this, 0);
            break;
        }
    }
}

/*!
 * Processes one thumbnailing job and then calls itself to process the next 
 * one. While processing the job this method will only initate the thumbnailing,
 * it will not wait until the actual thumbnail is ready.
 */
void 
WallpaperImageLoader::processJobQueue ()
{
    if (m_ThumbnailLoadingJobs.isEmpty())
        return;

    //SYS_DEBUG ("Initiating Thumbnailer");
    Job job = m_ThumbnailLoadingJobs.takeFirst();
    m_ThumbnailPendingJobs.append (job);
    connect (job.desc, SIGNAL(thumbnailLoaded(WallpaperDescriptor*)),
            this, SLOT(thumbnailLoaded (WallpaperDescriptor*)));

    job.desc->initiateThumbnailer ();

    // Continue loading and letting UI thread do something
    if(m_ThumbnailLoadingJobs.count() > 0)
        QTimer::singleShot(loadPictureDelay, this, SLOT(processJobQueue()));
}

/******************************************************************************
 * WallpaperContentItemCreator implementation.
 */
#ifdef USE_IMAGE_WIDGET
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
    SYS_WARNING ("setting %gx%g", size.width(), size.height());
    m_CellSize = size;
}

QSizeF 
WallpaperCellCreator::cellSize() const
{
    SYS_DEBUG ("Returning %gx%g", m_CellSize.width(), m_CellSize.height());
    return m_CellSize;
}

void 
WallpaperCellCreator::updateCell (
        const QModelIndex &index, 
        MWidget           *cell) const
{
    GridImageWidget *imageWidget = qobject_cast<GridImageWidget *>(cell);
    QVariant data = index.data(Qt::DisplayRole);
    WallpaperDescriptor *desc = data.value<WallpaperDescriptor *>();
   
    //SYS_DEBUG ("Updating cell %p", imageWidget);
    if (!imageWidget)
        return;


    if (desc->isThumbnailLoaded()) {
        /*
         * We can experiment with different image sizes.
         */
        #if 1
        imageWidget->setPixmap (desc->thumbnailPixmap().scaled(
                    cellSize().width(),
                    cellSize().height()));
        #else
        //imageWidget->setPixmap (desc->pixmap());
        imageWidget->setPixmap (desc->thumbnailPixmap());
        #endif
    } else {
        if (imageWidget->image() != imagePlaceHolderIconId) {
            /*
             * Need to set the size also, otherwise the iamge gets pixelated.
             */
            imageWidget->setImage (imagePlaceHolderIconId, cellSize()); 
        }
    }

    // The spinner.
    if (desc->loading()) {
        imageWidget->progressIndicator(true)->show();
    } else {
        MProgressIndicator *indicator;

        indicator = imageWidget->progressIndicator(false);
        if (indicator)
            indicator->hide();
    }
    
    // A checkmark indicating that this is the current wallpaper.
    if (desc->isCurrent()) {
        if (imageWidget->topRightImage() != checkIconId)
            imageWidget->setTopRightImage (checkIconId);
    } else {
        imageWidget->setTopRightImage ("");
    }
}
#else
MWidget *
WallpaperCellCreator::createCell (
        const QModelIndex &index, 
        MWidgetRecycler   &recycler) const
{
    MAdvancedListItem *cell;
    
    cell = qobject_cast <MAdvancedListItem *> (
            recycler.take(MAdvancedListItem::staticMetaObject.className()));

    if (!cell) {
        cell = new MAdvancedListItem (
            MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons);
        cell->progressIndicator()->setUnknownDuration (true);
        cell->sideTopImageWidget()->hide();
        cell->sideBottomImageWidget()->hide();
    }

    updateCell(index, cell);

    return cell;
}

void 
WallpaperCellCreator::updateCell (
        const QModelIndex &index, 
        MWidget           *cell) const
{
    MAdvancedListItem *listItem = qobject_cast<MAdvancedListItem *>(cell);
    QVariant data = index.data(Qt::DisplayRole);
    WallpaperDescriptor *desc = data.value<WallpaperDescriptor *>();

    if (desc->isCurrent()) {
        //% "Current wallpaper"
        listItem->setTitle (qtTrId("qtn_wall_current_wallpaper"));
        //listItem->setSubtitle (desc->title());
    } else {
        listItem->setTitle (desc->title());
        //listItem->setSubtitle ("");
    }

    // The spinner.
    if (desc->loading()) {
        listItem->progressIndicator()->show();
    } else {
        listItem->progressIndicator()->hide();
    }

    // The image
    SYS_DEBUG ("Setting pixmap for %s", SYS_STR(desc->title()));
    if (desc->isThumbnailLoaded())
        listItem->imageWidget()->setPixmap (desc->thumbnailPixmap());
    else {
        if (listItem->imageWidget()->image() != "icon-m-content-not-loaded")
            listItem->imageWidget()->setImage("icon-m-content-not-loaded");
    }

    // The style
    updateListItemMode (index, listItem);
}

void 
WallpaperCellCreator::updateListItemMode (
              const QModelIndex &index, 
              MAdvancedListItem *listItem) const
{
    int row = index.row();
    int rows = index.model()->rowCount();

    if (row == 0)
        listItem->setLayoutPosition (M::VerticalTopPosition);
    else if (row < rows - 1)
        listItem->setLayoutPosition (M::VerticalCenterPosition);
    else 
        listItem->setLayoutPosition (M::VerticalBottomPosition);
}
#endif

/******************************************************************************
 * WallpaperModel implementation.
 */

WallpaperModel::WallpaperModel (
        WallpaperBusinessLogic *logic,
        QObject                *parent) :
    QAbstractTableModel (parent),
    m_BusinessLogic (logic)
{
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
}

int 
WallpaperModel::rowCount(
		const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    
    return m_DescriptorList.size();
}

QVariant
WallpaperModel::data (
        const QModelIndex &index, 
        int role) const
{
    QVariant             var;

    Q_UNUSED (role);
    Q_UNUSED (index);

    Q_ASSERT (index.row() >= 0);
    Q_ASSERT (index.row() < m_DescriptorList.size());

    var.setValue (m_DescriptorList[index.row()]);
    
    return var;
}

int 
WallpaperModel::columnCount (
        const QModelIndex&) const
{
    return 1;
}

void
WallpaperModel::imageLoaded (
        const QModelIndex &row)
{
    emit dataChanged (row, row);
}

void 
WallpaperModel::descriptorChanged (
        WallpaperDescriptor *desc)
{
    SYS_WARNING ("");

    for (int n = 0; n < m_DescriptorList.size(); ++n) {
        if (m_DescriptorList[n] == desc) {
            QModelIndex first;
            first = index (n, 0);
            emit dataChanged (first, first);

            break;
        }
    }
}

void 
WallpaperModel::wallpaperChanged ()
{
    SYS_DEBUG ("");

    QModelIndex first = index (0, 0);
    if (first.isValid())
        emit dataChanged (first, first);
}
