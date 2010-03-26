/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_THEMEBUSINESSLOGIC_H
#define UT_THEMEBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class DuiApplication;
class ThemeBusinessLogic;

class Ut_ThemeBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testCurrentTheme ();
    
private:
    DuiApplication       *m_App;
    ThemeBusinessLogic   *m_Api;
};

#endif
