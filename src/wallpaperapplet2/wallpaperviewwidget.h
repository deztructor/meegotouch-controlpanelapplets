/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef WALLPAPERVIEWWIDGET_H
#define WALLPAPERVIEWWIDGET_H

#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"

#include <dcpwidget.h>
#include <QPointer>

class WallpaperViewWidget : public DcpWidget
{
    Q_OBJECT
    
    public:
        WallpaperViewWidget (
                WallpaperBusinessLogic *wallpaperBusinessLogic, 
                QGraphicsWidget        *parent = 0);

        virtual void saveImage ();
        virtual void dropImage ();
            
        virtual void paint (
            QPainter                        *painter,
            const QStyleOptionGraphicsItem  *option,
            QWidget *widget = 0);

    protected slots:
        virtual void createContent ();

    protected:
        QPointer<WallpaperBusinessLogic>  m_BusinessLogic;
        WallpaperITrans                   m_Trans;
        QColor                            m_BgColor;
        QuillImage                        m_Image;
};

#endif