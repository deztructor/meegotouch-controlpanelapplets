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
#include "wallpapercellcreator.h"

#include "gridimagewidget.h"
#include "wallpapermodel.h"

#define DEBUG
#define WARNING
#include <../debug.h>

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
        imageWidget->setID (desc.filePath());
        if (desc.thumbnailPending()) {
            desc.setThumbnailPending (false);
        }

        if (!desc.progress())
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
    imageWidget->setProgress (desc.progress());
}

