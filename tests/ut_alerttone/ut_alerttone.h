/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
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
#ifndef UT_ALERTTONETESTS_H
#define UT_ALERTTONETESTS_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class Ut_AlertToneTests: public QObject
{
Q_OBJECT

public:
      Ut_AlertToneTests(){m_App = 0;}
private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void alerttonesAlertTones_data(){};
      void alerttonesAlertTones();
/*
 * AlertTone::niceNameFromFileName() has been removed, use
 * TrackerConnection::niceNameFromFileName() instead.
 */
      //void alerttonesNiceNameFromFileName();
      void alerttoneFetchFromBackend();
      void alerttoneRealSetValue();
      void alerttoneMaybeUpdate();

      void alertToneChanged();
private:
    bool checkIfAlarmTone (QString name);
    MApplication* m_App;
    bool called_alertToneChanged;
};

#endif
