/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef TESTAPPLET_H
#define TESTAPPLET_H

#include "dcpappletif.h"
#include "testwidget.h"

#include <QObject>
#include <QPointer>
#include <DcpAppletIf>

class TestApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    TestApplet ();
    ~TestApplet ();

    virtual void init();
    virtual DcpWidget *constructWidget (int widgetId);
    virtual DcpWidget *pageMain (int Id);
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    QPointer<TestWidget> m_MainWidget;
    QPointer<TestWidget> m_Page1;
    QPointer<TestWidget> m_Page2;
    QPointer<TestWidget> m_Page3;
    QPointer<TestWidget> m_Page4;
    QPointer<TestWidget> m_Page5;
};
#endif
