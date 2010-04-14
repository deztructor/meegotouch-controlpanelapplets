/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERMODEL_H
#define WALLPAPERMODEL_H

#include <QAbstractTableModel>
#include <DuiAbstractCellCreator>
#include <DuiContentItem>
#include "wallpaperdescriptor.h"

class WallpaperBusinessLogic;

class WallpaperModel: public QAbstractTableModel
{
    Q_OBJECT

    public:
        WallpaperModel (
                WallpaperBusinessLogic *logic,
                QObject                *parent = 0);

    virtual int rowCount (
            const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data (
            const QModelIndex &index, 
            int role = Qt::DisplayRole) const;
    virtual int columnCount (const QModelIndex&) const;

private:
    WallpaperBusinessLogic *m_BusinessLogic;
    QStringList             m_Filenames;
};

class WallpaperContentItemCreator : 
    public DuiAbstractCellCreator<DuiContentItem>
{
  public:
      void updateCell(const QModelIndex& index, DuiWidget *cell) const;
};

#endif
