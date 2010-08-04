/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_THEMELISTMODEL_H
#define UT_THEMELISTMODEL_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>
#include <QModelIndex>

#include <themedescriptor.h>

class MApplication;
class ThemeBusinessLogic;
class ThemeListModel;

class UtThemeListModelPrivate : public QObject 
{
    Q_OBJECT

public:
    UtThemeListModelPrivate ();
    void reset ();

public slots:
    void dataChanged (const QModelIndex &first, const QModelIndex &last);

public:
    QModelIndex m_FirstChanged;
    QModelIndex m_LastChanged;
};

/*
 * There is a new signal notifyes us about the theme change being complete. The
 * signal sent by MTheme and should be simulated in this unit test.
 */
class Ut_ThemeListModel : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testAvailableThemes ();
    void testRefresh ();
private:
    UtThemeListModelPrivate     *m_Priv;
    MApplication                *m_App;
    ThemeBusinessLogic          *m_Logic;
    ThemeListModel              *m_Api;
};

#endif

