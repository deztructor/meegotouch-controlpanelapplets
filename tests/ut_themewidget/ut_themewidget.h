/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_THEMEWIDGET_H
#define UT_THEMEWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class ThemeWidget;
class ThemeBusinessLogic;

class Ut_ThemeWidget : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

private:
    MApplication                 *m_App;
    ThemeBusinessLogic          *m_Api;
};

#endif
