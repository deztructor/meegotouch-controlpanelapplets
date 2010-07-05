/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_DISPLAYWIDGET_H
#define UT_DISPLAYWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;

class Ut_DisplayWidget : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testBrightnessSlider ();
    void testScreenTimeout ();
    void testBlankInhibit ();
    void testTranslation ();
    
private:
    MApplication       *m_App;
};

#endif
