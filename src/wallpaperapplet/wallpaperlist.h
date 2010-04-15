/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERLIST_H
#define WALLPAPERLIST_H

#include <DuiList>
#include <QModelIndex>

class WallpaperDescriptor;
class WallpaperBusinessLogic;
class WallpaperImageLoader;

/*!
 * A list widget that shows a list of the available wallpapers.
 */
class WallpaperList : public DuiList {
    Q_OBJECT
        
public:
    WallpaperList (WallpaperBusinessLogic *logic);

    typedef enum {
        DataSourceLocal,
        DataSourceOvi,
        DataSourceUnknown
    } DataSourceType;

    void setDataSourceType (WallpaperList::DataSourceType sourceType);

private slots:
    void slotItemClicked (const QModelIndex &index);
    void loadPictures ();
    
signals:
    void imageActivated (WallpaperDescriptor *desc);

private:
    WallpaperBusinessLogic *m_BusinessLogic;
    WallpaperImageLoader   *m_ImageLoader;
    DataSourceType          m_DataSourceType;
};

#endif
