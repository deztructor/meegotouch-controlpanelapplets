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

#include "wallpaperviewwidgetstyle.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"

#include <DcpStylableWidget>
#include <QPointer>
#include <MAction>
#include <MImageWidget>

class WallpaperViewWidget : public DcpStylableWidget
{
    Q_OBJECT
    
    public:
        WallpaperViewWidget (
                WallpaperBusinessLogic *wallpaperBusinessLogic, 
                QGraphicsWidget        *parent = 0);
        ~WallpaperViewWidget ();

        virtual void saveImage ();
        virtual void dropImage ();

        virtual bool pagePans () const; 
#if 0
        virtual void paint (
            QPainter                        *painter,
            const QStyleOptionGraphicsItem  *option,
            QWidget *widget = 0);
#endif

    signals:
        void doneClicked ();
        void cancelClicked ();

    protected:
        virtual void polishEvent ();
        virtual int imageX () const;
        virtual int imageY () const;
        virtual int imageDX () const;
        virtual int imageDY () const;
        virtual QPixmap generatePixmap (
                const WallpaperITrans  &transformations);
        QSize imageVisualSize (qreal scale = 1.0);

    protected slots:
        virtual void initialize (QuillImage &image, QSize size);
        virtual void slotDoneActivated ();
        virtual void slotCancelActivated ();
        virtual void wallpaperSaved ();
        virtual void wallpaperLoaded (QuillImage image, QSize originalSize);
        virtual void redrawImage ();
        
    protected:
        QPointer<WallpaperBusinessLogic>  m_BusinessLogic;
        WallpaperITrans                   m_Trans;
        QColor                            m_BgColor;
        QuillImage                        m_Image;
        QSize                             m_OriginalSize;
        bool                              m_Saving;
        bool                              m_Initialized;
        MImageWidget                     *m_ImageWidget;

    private:
        //M_STYLABLE_WIDGET(WallpaperViewWidgetStyle)
        QImage transformedImage ();

    private:
        bool                              m_PageRealized;
        QPointer<MAction>                 m_DoneAction;
        QPointer<MAction>                 m_CancelAction;
};

#endif
