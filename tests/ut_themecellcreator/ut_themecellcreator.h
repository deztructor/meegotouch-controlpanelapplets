/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_THEMECELLCREATOR_H
#define UT_THEMECELLCREATOR_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>

#include "themedescriptor.h"

class MApplication;
class ThemeBusinessLogic;


/*
 */
class Ut_ThemeCellCreator : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testCreateCell ();

private:
    MApplication                *m_App;
    ThemeBusinessLogic          *m_ThemeBusinessLogic;
    QList<ThemeDescriptor *>     m_ThemeDescList;
};

#endif
