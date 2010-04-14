/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERBUSINESSLOGIC_H
#define WALLPAPERBUSINESSLOGIC_H

#include <QList>
#include <QObject>

class QString;
class DuiGConfItem;
class WallpaperDescriptor;
class QStringList;

class WallpaperBusinessLogic : public QObject
{
    Q_OBJECT

public:
    WallpaperBusinessLogic ();
    ~WallpaperBusinessLogic ();

    bool hasWallpaperFileName (bool portrait = false);
    QString WallpaperFileName (bool portrait = false);
    WallpaperDescriptor Wallpaper (bool portrait = false);

    QStringList availableWallpapers () const;

public slots:
    void setBackground (WallpaperDescriptor &desc);

private:
    DuiGConfItem   *m_LandscapeGConfItem;
    DuiGConfItem   *m_PortraitGConfItem;
};

#endif
