/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERWIDGET_H
#define WALLPAPERWIDGET_H

#include <QPointer>
#include "dcpwidget.h"

#include "wallpaperbusinesslogic.h"

class DuiContainer;
class DuiGridLayoutPolicy;

class WallpaperWidget : public DcpWidget
{
    Q_OBJECT

public:
    WallpaperWidget (
            WallpaperBusinessLogic *wallpaperBusinessLogic, 
            QGraphicsWidget        *parent = 0);
    ~WallpaperWidget ();

    void retranslateUi ();

    typedef enum {
        ThemeLocal,
        ThemeOvi
    } ThemeCategoryId;

private:
    void createWidgets ();
    DuiContainer *createContainer (WallpaperWidget::ThemeCategoryId category);
    void readLocalThemes ();

    QPointer<WallpaperBusinessLogic>  m_WallpaperBusinessLogic;
    DuiContainer                 *m_LocalContainer;
    DuiGridLayoutPolicy          *m_LocalLayoutPolicy;
    DuiContainer                 *m_OviContainer;
    DuiGridLayoutPolicy          *m_OviLayoutPolicy;
};

#endif
