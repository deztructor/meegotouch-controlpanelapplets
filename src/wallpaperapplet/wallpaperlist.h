/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERLIST_H
#define WALLPAPERLIST_H

#include <DuiList>
#include <QModelIndex>
class WallpaperDescriptor;

class WallpaperList : public DuiList {
    Q_OBJECT
        
public:
    WallpaperList ();

    typedef enum {
        DataSourceLocal,
        DataSourceOvi,
        DataSourceUnknown
    } DataSourceType;

    void setDataSourceType (WallpaperList::DataSourceType sourceType);

private slots:
    void slotItemClicked (const QModelIndex &index);

signals:
    void imageActivated (WallpaperDescriptor &desc);

private:
    DataSourceType    m_DataSourceType;
};

#endif
