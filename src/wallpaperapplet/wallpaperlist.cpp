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
#include "wallpaperbusinesslogic.h"
#include "wallpaperlist.h"
#include "wallpapermodel.h"

#include <QTimer>

#define DEBUG
#define WARNING
#include "../debug.h"

static const int loadPicturesDelay = 200;

#ifdef USE_GRID_LAYOUT
static int columnsLandscape = 5;
static int columnsPortrait = 3;
#include <MOrientationChangeEvent>
#include <MApplication>
#include <MApplicationWindow>
#endif

WallpaperList::WallpaperList (
        WallpaperBusinessLogic *logic,
        QGraphicsItem          *parent) :
    MList (parent),
    m_BusinessLogic (logic),
    m_ImageLoader (new WallpaperImageLoader),
    m_Model (0),
    m_CellCreator (new WallpaperCellCreator),
    m_DataSourceType (WallpaperList::DataSourceUnknown)
{
    #ifdef USE_GRID_LAYOUT
    MApplicationWindow *window;
    int                 columns;
    int                 initialWidth;
    #endif

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
    connect (m_BusinessLogic, SIGNAL(wallpaperChanged()), 
            this, SLOT(loadPictures()));

    #ifdef USE_GRID_LAYOUT

    columns = columnsLandscape;
    window = MApplication::activeApplicationWindow();
    if (window) {
        SYS_DEBUG ("--------------->");
        columns = window->orientation() == M::Landscape ?
            columnsLandscape : columnsPortrait;
        initialWidth = window->visibleSceneSize().width() / columns;
        SYS_DEBUG ("initialWidth = %d", initialWidth);
        m_CellCreator->setCellSize (QSizeF(initialWidth, initialWidth));
    }

    setColumns (columns);
    #endif
}

void
WallpaperList::setDataSourceType (
        WallpaperList::DataSourceType sourceType)
{
    Q_ASSERT (m_DataSourceType == DataSourceUnknown);
    
    m_Model = new WallpaperModel (m_BusinessLogic);
    setItemModel (m_Model);

    setCellCreator (m_CellCreator);

    QTimer::singleShot (loadPicturesDelay, this, SLOT(loadPictures()));
    m_DataSourceType = sourceType;
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

#ifdef USE_GRID_LAYOUT
void 
WallpaperList::orientationChangeEvent (
        MOrientationChangeEvent *event)
{
    int   columns;
    qreal width;

    columns = event->orientation() == M::Landscape ?
        columnsLandscape : columnsPortrait;
    //width = geometry().width() / columns;
    width = event->orientation() == M::Landscape ?
        864  / columns : 480 / columns;

    #if 0
    SYS_DEBUG ("*** geometry().width()  = %g", geometry().width());
    SYS_DEBUG ("*** geometry().height() = %g", geometry().height());
    SYS_DEBUG ("*** columns             = %d", columns);
    SYS_DEBUG ("*** width               = %g", width);
    #endif

    m_CellCreator->setCellSize (QSizeF(width, width));
    setColumns (columns);
}
#endif

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
    SYS_DEBUG ("");
    /*
     * When we page back from the other widget we might want to load the images
     * we stopped loading when we become hidden.
     */
    QTimer::singleShot (loadPicturesDelay, this, SLOT(loadPictures()));
}

