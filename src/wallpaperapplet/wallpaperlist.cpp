/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperlist.h"
#include "wallpapermodel.h"

#define DEBUG
#include "../debug.h"

WallpaperList::WallpaperList () :
    m_DataSourceType (WallpaperList::DataSourceUnknown)
{
    connect (this, SIGNAL(itemClicked(const QModelIndex &)),
            this, SLOT(slotItemClicked(const QModelIndex &)));
}

void
WallpaperList::setDataSourceType (
        WallpaperList::DataSourceType sourceType)
{
    WallpaperContentItemCreator *cellCreator;
    WallpaperModel *model;

    Q_ASSERT (m_DataSourceType == DataSourceUnknown);

    cellCreator = new WallpaperContentItemCreator;
    setCellCreator (cellCreator);

    model = new WallpaperModel;
    setItemModel (model);
}


void 
WallpaperList::slotItemClicked (
        const QModelIndex &index)
{
    QVariant data = index.data(Qt::DisplayRole);
    WallpaperDescriptor rowData = data.value<WallpaperDescriptor>();

    SYS_DEBUG ("*** filename = %s", SYS_STR(rowData.filename()));
    emit imageActivated(rowData);
}

