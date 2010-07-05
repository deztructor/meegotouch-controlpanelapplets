/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_THEMEDIALOG_H
#define UT_THEMEDIALOG_H

#include <QtTest/QtTest>
#include <QObject>
#include <QPointer>
#include "themebusinesslogic.h"

class MApplication;

class Ut_ThemeDialog : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testAccept ();
    void testCancel ();
    
private:
    MApplication                 *m_App;
    QPointer<ThemeBusinessLogic>  m_ThemeBusinessLogic;
};

#endif
