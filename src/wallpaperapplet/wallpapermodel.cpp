/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapermodel.h"
#include "wallpaperbusinesslogic.h"

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
    m_Filenames = logic->availableWallpapers();
}

int 
WallpaperModel::rowCount(
		const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    
    //SYS_DEBUG ("Returning %d", m_Filenames.size());
    return m_Filenames.size();
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
    Q_ASSERT (index.row() >= 0);
    Q_ASSERT (index.row() < m_Filenames.size());

    desc.setFilename (m_Filenames[index.row()]);
    var.setValue (desc);
    
    SYS_DEBUG ("Returning for %d", index.row());
    return var;
}

int 
WallpaperModel::columnCount (
        const QModelIndex&) const
{
    return 1;
}
