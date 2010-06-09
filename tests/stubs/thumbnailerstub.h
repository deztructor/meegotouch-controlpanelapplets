/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
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

#ifndef __THUMBNAILER_H__
#define __THUMBNAILER_H__

#include <QObject>
#include <QList>
#include <QUrl>
#include <QPixmap>

class QStringList;
class QString;

class Thumbnailer : public QObject 
{
Q_OBJECT

public:
    Thumbnailer ();
    ~Thumbnailer ();
    
    void reset ();
    void sendFinished ();
    void sendThumbnail ();
    void sendError ();
    
    bool request (
            QList<QUrl> &uris, 
            QStringList &mimeTypes, 
            bool sendPixmap = false, 
            QString flavor = QString(), 
            QString scheduler = QString(), 
            bool autoCancel = false, 
            bool sendRemainingSignals = true );

signals:
    void finished (int left);

   	void thumbnail (
            QUrl       uri,
            QUrl       thumbnailUri,
            QPixmap    pixmap,
            QString    flavor);

 	void error (
            QString    error, 
            QUrl       uri);

public:
    bool     m_RequestCame;
    QUrl     m_Url;
    QString  m_Flavor;
};
#endif
