/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef RESETAPPLET_H
#define RESETAPPLET_H

#include "dcpappletif.h"
#include "resetbusinesslogic.h"

#include <QObject>
#include <QPointer>
#include <DcpAppletIf>

class ResetWidget;

class ResetApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    ResetApplet ();
    ~ResetApplet ();

    virtual void init();
    virtual DcpWidget *constructWidget (int widgetId);
    virtual DcpWidget *pageMain (int widgetId);
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private slots:
    void userGuide ();

private:
    QPointer<ResetWidget>        m_MainWidget;
    QPointer<ResetBusinessLogic> m_ResetBusinessLogic;
    #ifdef UNIT_TEST
    friend class Ut_ResetApplet;
    #endif
};
#endif
