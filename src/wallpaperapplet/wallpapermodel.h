/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERMODEL_H
#define WALLPAPERMODEL_H

#include <QList>
#include <QAbstractTableModel>
#include <MAbstractCellCreator>
#include <MContentItem>
#include "wallpaperdescriptor.h"

class WallpaperBusinessLogic;
class WallpaperDescriptor;

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

public slots:
    void imageLoaded (const QModelIndex &row);

private:
    WallpaperBusinessLogic *m_BusinessLogic;
    QList<WallpaperDescriptor *> m_DescriptorList;
};

class WallpaperContentItemCreator : 
    public MAbstractCellCreator<MContentItem>
{
  public:
      void updateCell(
              const QModelIndex &index, 
              MWidget *cell) const;
      void updateContentItemMode(
              const QModelIndex &index, 
              MContentItem *contentItem) const;
};


class WallpaperImageLoader : public QObject
{
    Q_OBJECT

public:
    struct Job {
        WallpaperDescriptor  *desc;
        QModelIndex           row;
    };

public slots:
    void loadPictures (
            const QModelIndex& firstVisibleRow, 
            const QModelIndex& lastVisibleRow);

    void thumbnailLoaded (WallpaperDescriptor *desc);
    void processJobQueue ();
    void stopLoadingPictures ();

private:
    QList<Job>  m_ThumbnailLoadingJobs;
    QList<Job>  m_ThumbnailPendingJobs;
    #ifdef UNIT_TEST
    friend class Ut_WallpaperModel;
    #endif
};

#endif
