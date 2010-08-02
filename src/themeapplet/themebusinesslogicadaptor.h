/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#ifndef THEMEBUSINESSLOGICADAPTOR_H
#define THEMEBUSINESSLOGICADAPTOR_H

#include <QObject>
#include <QPointer>
#include <QDBusAbstractAdaptor>

class QDBusInterface;
class ThemeBusinessLogic;

#define THEME_DBUS_SERVICE "com.nokia.theme_applet"
#define THEME_DBUS_PATH "/com/nokia/theme_applet/request"


/*
       DBus Interface for the Theme Applet: ThemeBusinessLogicAdaptor

  This class implements a dbus adapter for the theme applet of the controlpanel.
  The DBus adapter makes it possible to hide a theme from the available theme
  list before the theme is uninstalled. Here is an example how a theme should be
  diabled before it is removed.

dbus-send \
  --print-reply \
  --session \
  --dest=com.nokia.theme_applet \
  /com/nokia/theme_applet/request \
  com.nokia.theme_applet.request.remove_theme string:blanco

  The DBus interface also implements a method to add a theme to the available
  themes list when the theme is installed. Here is an example how the theme
  install should be reported for the theme applet after it is installed. Please
  note that the theme must be valid and visible in order to show up in the theme
  list.

dbus-send \
  --print-reply \
  --session \
  --dest=com.nokia.theme_applet \
  /com/nokia/theme_applet/request \
  com.nokia.theme_applet.request.add_theme string:blanco
*/
class ThemeBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
#ifndef UNIT_TEST
    Q_CLASSINFO("D-Bus Interface", "com.nokia.theme_applet.request")
#endif

public:
    ThemeBusinessLogicAdaptor (
            QObject *                obj, 
            ThemeBusinessLogic      *logic);
    ~ThemeBusinessLogicAdaptor ();

public slots:
    void remove_theme (QString themeName);
    void add_theme (QString themeName);

signals:
    void themeAdded (QString themeName);
    void themeRemoved (QString themeName);
    
private:
    ThemeBusinessLogic  *m_ThemeBusinessLogic;
};
#endif
