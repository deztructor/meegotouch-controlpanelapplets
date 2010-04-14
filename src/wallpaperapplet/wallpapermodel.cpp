/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapermodel.h"

#define DEBUG
#include <../debug.h>

/******************************************************************************
 * WallpaperContentItemCreator implementation.
 */
void 
WallpaperContentItemCreator::updateCell (
        const QModelIndex&   index, 
        DuiWidget           *cell) const
{
    DuiContentItem * contentItem = qobject_cast<DuiContentItem *>(cell);

    QVariant data = index.data(Qt::DisplayRole);
    WallpaperDescriptor rowData = data.value<WallpaperDescriptor>();
    contentItem->setTitle (rowData.basename());
    contentItem->setSubtitle (rowData.filename());
}

/******************************************************************************
 * WallpaperModel implementation.
 */
int 
WallpaperModel::rowCount(
		const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    SYS_DEBUG ("Returning 5");
    return 5;
}

QVariant
WallpaperModel::data (
        const QModelIndex &index, 
        int role) const
{
    Q_UNUSED (role);
    Q_UNUSED (index);
    QString tmp = "Row...";
    QVariant var;

    WallpaperDescriptor desc;
    if (index.row() == 0) {
        desc.setFilename ("/usr/share/themes/base/dui/images/duiapplicationpage-background.png");
    } else if (index.row() == 1) {
        desc.setFilename ("/usr/share/themes/plankton/dui/images/duiapplicationpage-background.png");
    } else if (index.row() == 2) {
        desc.setFilename ("/usr/share/themes/plankton/dui/images/duiapplicationpage-portrait-background.png");
    } else {
        desc.setFilename ("/usr/share/themes/plankton/dui/images/duiapplicationpage-portrait-background.png");
    }

    var.setValue (desc);

    /*
    /usr/share/themes/base/dui/images/duiapplicationpage-background.png
    /usr/share/themes/plankton/dui/images/duiapplicationpage-background.png
    /usr/share/themes/plankton/dui/images/duiapplicationpage-portrait-background.png
    */

    SYS_DEBUG ("Returning for %d", index.row());
    return var;
}

int 
WallpaperModel::columnCount (
        const QModelIndex&) const
{
    SYS_DEBUG ("Returning 1");
    return 1;
}
