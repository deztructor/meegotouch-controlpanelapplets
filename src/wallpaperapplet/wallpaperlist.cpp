/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperlist.h"
#include "wallpapermodel.h"

#include <QTimer>

//#define DEBUG
#include "../debug.h"

static const int loadPicturesDelay = 200;

WallpaperList::WallpaperList (
        WallpaperBusinessLogic *logic,
        QGraphicsItem          *parent) :
    MList (parent),
    m_BusinessLogic (logic),
    m_ImageLoader (new WallpaperImageLoader),
    m_DataSourceType (WallpaperList::DataSourceUnknown)
{
    connect (this, SIGNAL(itemClicked(const QModelIndex &)),
            this, SLOT(slotItemClicked(const QModelIndex &)));
    /*
     * When the panning stops we load thumbnail images, when it starts we stop
     * doing the image loading so the panning will be smooth.
     */
    connect (this, SIGNAL(panningStarted()), 
            m_ImageLoader, SLOT(stopLoadingPictures()));
    connect (this, SIGNAL(panningStopped()), 
            this, SLOT(loadPictures()));
}

void
WallpaperList::setDataSourceType (
        WallpaperList::DataSourceType sourceType)
{
    /*
     * FIXME: Support for OVI is not implemented yet, so we don't use this.
     */
    Q_UNUSED (sourceType);


    WallpaperContentItemCreator *cellCreator;
    WallpaperModel *model;

    Q_ASSERT (m_DataSourceType == DataSourceUnknown);
    
    model = new WallpaperModel (m_BusinessLogic);
    setItemModel (model);

    cellCreator = new WallpaperContentItemCreator;
    setCellCreator (cellCreator);


    QTimer::singleShot (loadPicturesDelay, this, SLOT(loadPictures()));
}


void 
WallpaperList::slotItemClicked (
        const QModelIndex &index)
{
    QVariant data = index.data(Qt::DisplayRole);
    WallpaperDescriptor *rowData = data.value<WallpaperDescriptor *>();

    SYS_DEBUG ("*** filename = %s", SYS_STR(rowData->filename()));
    emit imageActivated(rowData);
}

void 
WallpaperList::loadPictures ()
{
    SYS_DEBUG ("");

    /*
     * We used to get panningStopped() signals when we got hidden, so we will
     * not initiate loading of the images when we are not visible.
     */
    if (m_ImageLoader == 0 || !isVisible())
        return;

    m_ImageLoader->loadPictures (firstVisibleItem(), lastVisibleItem());
}

void
WallpaperList::hideEvent (
        QHideEvent *event)
{
    Q_UNUSED (event);
    /*
     * When we got hidden we stop all the image loading. We have to give some
     * CPU for the other page/widget.
     */
    m_ImageLoader->stopLoadingPictures();
}

void 
WallpaperList::showEvent (
        QShowEvent *event)
{
    Q_UNUSED (event);
    /*
     * When we page back from the other widget we might want to load the images
     * we stopped loading when we become hidden.
     */
    QTimer::singleShot (loadPicturesDelay, this, SLOT(loadPictures()));
}

