/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERMODEL_H
#define WALLPAPERMODEL_H

#include <QList>
#include <QAbstractTableModel>
#include <MAbstractCellCreator>
#include <MAdvancedListItem>
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
    void descriptorChanged (WallpaperDescriptor *desc);
    
private:
    WallpaperBusinessLogic       *m_BusinessLogic;
    QList<WallpaperDescriptor *>  m_DescriptorList;
};

class WallpaperCellCreator : 
    public MAbstractCellCreator<MAdvancedListItem>
{
  public:
      virtual MWidget *createCell (
                const QModelIndex &index, 
                MWidgetRecycler   &recycler) const;

      virtual void updateCell (
              const QModelIndex &index, 
              MWidget *cell) const;
      
      void updateListItemMode (
              const QModelIndex &index, 
              MAdvancedListItem *contentItem) const;
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
    friend class Ut_WallpaperList;
    #endif
};

#endif
