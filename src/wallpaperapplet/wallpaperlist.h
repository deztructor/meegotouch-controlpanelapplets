/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERLIST_H
#define WALLPAPERLIST_H

#include <MList>
#include <QModelIndex>

class QGraphicsItem;
class WallpaperDescriptor;
class WallpaperBusinessLogic;
class WallpaperImageLoader;
class WallpaperModel;

/*!
 * A list widget that shows a list of the available wallpapers.
 */
class WallpaperList : public MList {
    Q_OBJECT
        
public:
    WallpaperList (
            WallpaperBusinessLogic  *logic,
            QGraphicsItem           *parent = 0);

    typedef enum {
        DataSourceLocal,
        DataSourceOvi,
        DataSourceUnknown
    } DataSourceType;

    void setDataSourceType (WallpaperList::DataSourceType sourceType);

protected:
    void hideEvent (QHideEvent *event);
    void showEvent (QShowEvent *event);

private slots:
    void slotItemClicked (const QModelIndex &index);
    void loadPictures ();
    
signals:
    void imageActivated (WallpaperDescriptor *desc);

private:
    WallpaperBusinessLogic *m_BusinessLogic;
    WallpaperImageLoader   *m_ImageLoader;
    WallpaperModel         *m_Model;
    DataSourceType          m_DataSourceType;
    #ifdef UNIT_TEST
    friend class Ut_WallpaperList;
    #endif
};

#endif
