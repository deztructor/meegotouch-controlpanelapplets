/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperbrief.h"

#define DEBUG
#include "../debug.h"

WallpaperBrief::WallpaperBrief (
        WallpaperBusinessLogic *businessLogic) :
    m_WallpaperBusinessLogic (businessLogic)
{
}

QString 
WallpaperBrief::valueText() const
{
    Q_ASSERT (m_WallpaperBusinessLogic != 0);
    return "Unimplemented";
}

