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
#ifndef WALLPAPERMODEL_H
#define WALLPAPERMODEL_H

#include <QPointer>
#include <QList>
#include <QHash>
#include <QAbstractTableModel>
#include <Thumbnailer>


#include <MAbstractCellCreator>
#include "wallpaperdescriptor.h"
#include <MImageWidget> 
#include <QSizeF>

class WallpaperBusinessLogic;
class WallpaperDescriptor;


class WallpaperModel: public QAbstractTableModel
{
    Q_OBJECT

    public:
        typedef enum {
            StateDisappeared,
            StateDiscovered,
            StateProcessed,
            StateProcessing,
        } EntryState;

        typedef enum {
            WallpaperDescriptorRole = Qt::UserRole,
        } WallpaperModelRole;

        WallpaperModel (
                WallpaperBusinessLogic *logic,
                QObject                *parent = 0);
        
        ~WallpaperModel ();
    
        /*
         * Methods for accessing data.
         */
        virtual int rowCount (
                const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data (
                const QModelIndex &index, 
                int role = Qt::DisplayRole) const;
        virtual int columnCount (const QModelIndex&) const;
        
        void loadThumbnails (
                const QModelIndex& firstVisibleRow, 
                const QModelIndex& lastVisibleRow);

    public slots:
        void descriptorChanged (WallpaperDescriptor *desc);
        void wallpaperChanged ();
        /*
         * Slots for the thumbnailer connection.
         */
        void thumbnailLoadingFinished (
                int          left);
        void thumbnailError (
                QString      message,
                QUrl         fileUri);

        void thumbnailReady (
                QUrl         fileUri, 
                QUrl         thumbnailUri, 
                QPixmap      pixmap, 
                QString      flavor);

    private:
        void loadFromDirectory ();

    private:
        WallpaperBusinessLogic                 *m_BusinessLogic;
        QStringList                             m_FilePathList;
        QHash<QString, WallpaperDescriptor>     m_FilePathHash; 
        QString                                 m_ImagesDir;
        QPointer<Thumbnailer>                   m_Thumbnailer;
};

class WallpaperCellCreator : 
    public MAbstractCellCreator<MWidgetController>
{
    public:
        virtual MWidget *createCell (
                const QModelIndex &index, 
                MWidgetRecycler   &recycler) const;
      
        virtual void setCellSize (const QSizeF &size);
        virtual QSizeF cellSize() const;

        virtual void updateCell (
                const QModelIndex &index, 
                MWidget *cell) const;

    private:
        QSizeF   m_CellSize;
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

    void processJobQueue ();
    void stopLoadingPictures ();

private:
    QList<Job>  m_ThumbnailLoadingJobs;
    #ifdef UNIT_TEST
    friend class Ut_WallpaperModel;
    friend class Ut_WallpaperList;
    #endif
};

#endif
