/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERWIDGET_H
#define WALLPAPERWIDGET_H

#include <QPointer>
#include <SelectSingleContentItemPage.h>

#include "dcpwidget.h"

#include "wallpaperbusinesslogic.h"

class MContainer;
class MLinearLayoutPolicy;
class MList;
class WallpaperList;
class MContentItem;

class WallpaperWidget : public DcpWidget
{
    Q_OBJECT

public:
    WallpaperWidget (
            WallpaperBusinessLogic *wallpaperBusinessLogic, 
            QGraphicsWidget        *parent = 0);
    ~WallpaperWidget ();

    void retranslateUi ();

private slots:
    void slotImageActivated (WallpaperDescriptor *desc);
    void slotImageActivated ();
    virtual void createContent ();
    void galleryActivated ();
    void oviActivated ();
    void galleryImageSelected(const QString &);

private:
    QPointer<WallpaperBusinessLogic>       m_WallpaperBusinessLogic;
    WallpaperList                         *m_ImageList;
    MContentItem                          *m_GalleryItem;
    MContentItem                          *m_OviItem;
    QPointer<SelectSingleContentItemPage>  m_ImageBrowser;
    #ifdef UNIT_TEST
    friend class Ut_WallpaperWidget;
    #endif
};

#endif
