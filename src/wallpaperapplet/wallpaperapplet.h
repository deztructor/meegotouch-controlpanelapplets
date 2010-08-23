/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERAPPLET_H
#define WALLPAPERAPPLET_H

#include "dcpappletif.h"
#include "wallpaperbusinesslogic.h"

#include <QObject>
#include <QPointer>
#include <DcpAppletIf>

class WallpaperWidget;
class WallpaperEditorWidget;

class WallpaperApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    WallpaperApplet ();
    ~WallpaperApplet ();

    typedef enum {
        MainWidget    = 0,
        EditorWidget  = 1,
    } WindowID;

    virtual void init();
    virtual DcpWidget *constructWidget (int widgetId);
    virtual DcpWidget *pageMain (int widgetId);
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private slots:
    void userGuide ();

private:
    QPointer<WallpaperWidget>        m_MainWidget;
    QPointer<WallpaperEditorWidget>  m_EditorWidget;
    QPointer<WallpaperBusinessLogic> m_WallpaperBusinessLogic;
    #ifdef UNIT_TEST
    friend class Ut_WallpaperApplet;
    #endif
};
#endif
