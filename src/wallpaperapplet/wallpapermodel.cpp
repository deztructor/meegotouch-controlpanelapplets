/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapermodel.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"

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

    SYS_DEBUG ("Between %d - %d", from, to);
    for (int n = from; n <= to; ++n) {
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

    SYS_DEBUG ("Initiating Thumbnailer");
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
MWidget *
WallpaperCellCreator::createCell(
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
    listItem->imageWidget()->setPixmap (desc->thumbnailPixmap());

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
    SYS_DEBUG ("");

    for (int n = 0; n < m_DescriptorList.size(); ++n) {
        if (m_DescriptorList[n] == desc) {
            QModelIndex first;
            first = index (n, 0);
            emit dataChanged (first, first);

            break;
        }
    }
}
