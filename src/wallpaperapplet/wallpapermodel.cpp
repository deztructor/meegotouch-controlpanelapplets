/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapermodel.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"

#include <QTimer>

#define DEBUG
#include <../debug.h>

/*
 * The intentional delay between the loading of two picture files. This gives a
 * little chance for other applications.
 */
static const int loadPictureDelay = 100;

/******************************************************************************
 * WallpaperImageLoader implementation.
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

        if (!desc->isThumbnailLoaded()) {
            Job job;
            job.desc = desc;
            job.row = index;

            thumbnailLoadingJobs << job;
        }
    }

    if (thumbnailLoadingJobs.count() != 0)
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
    thumbnailLoadingJobs.clear();
}

void
WallpaperImageLoader::thumbnailLoaded (
        WallpaperDescriptor *desc)
{
    SYS_DEBUG ("");
    for (int n = 0; n < thumbnailPendingJobs.size(); ++n) {
        SYS_DEBUG ("*** n = %d", n);
        if (thumbnailPendingJobs[n].desc == desc) {
            WallpaperModel *model = (WallpaperModel*) 
                thumbnailPendingJobs[n].row.model();

            model->imageLoaded (thumbnailPendingJobs[n].row);
            SYS_DEBUG ("Removing pending job at %d", n);
            thumbnailPendingJobs.removeAt (n);
            break;
        }
    }
}

void 
WallpaperImageLoader::processJobQueue ()
{
    if (thumbnailLoadingJobs.isEmpty())
        return;

    Job job = thumbnailLoadingJobs.takeFirst();
    thumbnailPendingJobs.append (job);
    connect (job.desc, SIGNAL(thumbnailLoaded(WallpaperDescriptor*)),
            this, SLOT(thumbnailLoaded (WallpaperDescriptor*)));

    job.desc->initiateThumbnailer ();

    // Continue loading and letting UI thread do something
    if(thumbnailLoadingJobs.count() > 0)
        QTimer::singleShot(loadPictureDelay, this, SLOT(processJobQueue()));
}

/******************************************************************************
 * WallpaperContentItemCreator implementation.
 */
void 
WallpaperContentItemCreator::updateContentItemMode(
              const QModelIndex &index, 
              MContentItem *contentItem) const
{
    int row = index.row();
    int rows = index.model()->rowCount();

    SYS_DEBUG ("");
    if (row == 0)
        contentItem->setItemMode(MContentItem::SingleColumnTop);
    else if (row < rows - 1)
        contentItem->setItemMode(MContentItem::SingleColumnCenter);
    else 
        contentItem->setItemMode(MContentItem::SingleColumnBottom);
}

void 
WallpaperContentItemCreator::updateCell (
        const QModelIndex &index, 
        MWidget           *cell) const
{
    MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
    QVariant data = index.data(Qt::DisplayRole);
    WallpaperDescriptor *rowData = data.value<WallpaperDescriptor *>();

    contentItem->setTitle (rowData->title());
    contentItem->setSubtitle (rowData->filename());

#if 1
    /*
     * Older libdui (that we use) supports pixmap here, while newer versions has
     * the support for QImage.
     */
    SYS_DEBUG ("Setting pixmap for %s", SYS_STR(rowData->title()));
    contentItem->setPixmap (rowData->thumbnailPixmap());
#else
    contentItem->setImage (rowData->thumbnail());
#endif

    updateContentItemMode(index, contentItem);
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

