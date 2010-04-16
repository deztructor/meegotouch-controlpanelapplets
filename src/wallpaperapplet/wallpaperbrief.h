/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERBRIEF_H
#define WALLPAPERBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

#include "wallpaperbusinesslogic.h"


class WallpaperBrief: public DcpBrief
{
    Q_OBJECT

public:
    WallpaperBrief (WallpaperBusinessLogic *businessLogic);

    virtual int widgetTypeID() const;
    virtual QString valueText() const;
    virtual QString image () const;

private:
    QPointer<WallpaperBusinessLogic> m_WallpaperBusinessLogic;
};

#endif
