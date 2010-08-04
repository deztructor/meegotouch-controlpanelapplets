/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_THEMEBUSINESSLOGIC_H
#define UT_THEMEBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>

#include <themedescriptor.h>

class MApplication;
class ThemeBusinessLogic;

class UtThemeBusinessLogicPrivate : public QObject 
{
    Q_OBJECT

public:
    UtThemeBusinessLogicPrivate ();
    void reset ();

public slots:
    void themeChanged (QString themeCodeName);
    void themeChangeStarted (QString themeCodeName);

    void themeAboutToBeRemoved (int index);
    void themeRemoved (QList<ThemeDescriptor *> list);
    void themeAboutToBeAdded (int index);
    void themeAdded (QList<ThemeDescriptor *> list);

signals:
    void changeTheme (QString themeCodeName);

public:
    QString m_ThemeCodeNameUnderProcess;
    QString m_ThemeCodeName;
    bool    m_ThemeAboutToBeAdded;
    bool    m_ThemeAdded;
    bool    m_ThemeAboutToBeRemoved;
    bool    m_ThemeRemoved;
};

/*
 * There is a new signal notifyes us about the theme change being complete. The
 * signal sent by MTheme and should be simulated in this unit test.
 */
class Ut_ThemeBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testCurrentTheme ();
    void testAvailableThemes ();
    void testSetTheme ();
    
private:
    UtThemeBusinessLogicPrivate *m_Priv;
    MApplication                *m_App;
    ThemeBusinessLogic          *m_Api;
};

#endif
