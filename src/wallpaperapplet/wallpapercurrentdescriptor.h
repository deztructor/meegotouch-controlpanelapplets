/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERCURRENTDESCRIPTOR_H
#define WALLPAPERCURRENTDESCRIPTOR_H

#include "wallpaperdescriptor.h"

class MDesktopEntry;

class Q_DECL_EXPORT WallpaperCurrentDescriptor : public WallpaperDescriptor {
    Q_OBJECT

public:
    static WallpaperCurrentDescriptor *instance ();

    virtual bool isCurrent () const;
    bool setFromDestopFile (QString desktopFileName);

protected:
    WallpaperCurrentDescriptor ();

private:
    bool getValue (
            const QString &group,
            const QString &key, 
            QString       &value);

private:
    static WallpaperCurrentDescriptor *sm_Instance;

    MDesktopEntry         *m_DesktopEntry;
};

#endif
