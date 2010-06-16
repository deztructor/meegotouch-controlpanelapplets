/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_THEMEDESCRIPTOR_H
#define UT_THEMEDESCRIPTOR_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class ThemeDescriptor;
class ThemeBusinessLogic;

class Ut_ThemeDescriptor : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testPerfectDescriptor ();
    void testMissingFileDescriptor ();
    void testHiddenThemeDescriptor ();
    void testEmptyThemeDescriptor ();

private:
    MApplication                 *m_App;
};

#endif
