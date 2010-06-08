/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef FT_ABOUTBUSINESSLOGIC_H
#define FT_ABOUTBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class AboutBusinessLogic;

class SignalSink : public QObject
{
Q_OBJECT
public:
    SignalSink ();

    
public slots:
    void ready();

public:
    bool  m_ReadyCame;
};

class Ft_AboutBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testCollectData ();

private:
    MApplication         *m_App;
    AboutBusinessLogic   *m_Api;
    SignalSink            m_SignalSink;
};

#endif
