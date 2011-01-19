/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef WALLPAPERWIDGET_H
#define WALLPAPERWIDGET_H

#include <QPointer>
#ifdef HAVE_CONTENT_MANAGER
#  include <SelectSingleContentItemPage.h>
#endif

#include "dcpwidget.h"
#include "wallpaperbusinesslogic.h"

class MContainer;
class MLinearLayoutPolicy;
class MLabel;
class MList;
class WallpaperList;
class MContentItem;
class MLinearLayoutPolicy;

class WallpaperWidget : public DcpWidget
{
    Q_OBJECT

public:
    WallpaperWidget (
            WallpaperBusinessLogic *wallpaperBusinessLogic, 
            QGraphicsWidget        *parent = 0);
    ~WallpaperWidget ();

protected:
    virtual void polishEvent ();

private slots:
    void slotImageActivated (WallpaperDescriptor *desc);
    void slotImageActivated ();
    virtual void createContent ();
    void oviActivated ();
    
    #ifdef HAVE_CONTENT_MANAGER
    void galleryActivated ();
    void galleryImageSelected(const QString &);
    #endif

private:
    void addHeaderContainer (MLinearLayoutPolicy *mainLayout);
    void retranslateUi ();

private:
    QPointer<WallpaperBusinessLogic>       m_WallpaperBusinessLogic;
    MLabel                                *m_TitleLabel;
    WallpaperList                         *m_ImageList;
    #ifdef HAVE_CONTENT_MANAGER
    QPointer<SelectSingleContentItemPage>  m_ImageBrowser;
    #endif

    MContentItem                          *m_OviItem;
    #ifdef UNIT_TEST
    friend class Ut_WallpaperWidget;
    #endif
};

#endif
