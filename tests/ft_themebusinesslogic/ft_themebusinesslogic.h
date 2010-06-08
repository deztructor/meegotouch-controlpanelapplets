/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef FT_THEMEBUSINESSLOGIC_H
#define FT_THEMEBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class ThemeBusinessLogic;

class FtThemeBusinessLogicPrivate : public QObject 
{
    Q_OBJECT

public slots:
    void themeChanged (QString themeCodeName);

signals:
    void changeTheme (QString themeCodeName);

public:
    QString m_ThemeCodeName;
};

class Ft_ThemeBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testCurrentTheme ();
    void testAvailableThemes ();
    void testChangeTheme ();

private:
    FtThemeBusinessLogicPrivate *m_Priv;
    MApplication              *m_App;
    ThemeBusinessLogic          *m_Api;
};

#endif
