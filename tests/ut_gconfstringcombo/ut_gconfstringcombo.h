/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_GCONFSTRINGCOMBO_H
#define UT_GCONFSTRINGCOMBO_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class QGConfValue;

class Ut_GConfStringComboTests : public QObject
{
Q_OBJECT
public:
      Ut_GConfStringComboTests() { m_App = 0; }

private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void gconfstringcomboConstructor_data ();
      void gconfstringcomboConstructor ();
      void gconfstringcomboChangedInGConf_data();
      void gconfstringcomboChangedInGConf();
      void gconfstringcomboCurrentIndexChanged_data();
      void gconfstringcomboCurrentIndexChanged();
      void gconfstringcomboRetranslateUi_data();
      void gconfstringcomboRetranslateUi();

private:
      void _setGConfReturnString (
              const QString   &key,
              const QString   &newValue, 
              QGConfValue     *toNotify);

      MApplication * m_App;
};

#endif
