/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef ABOUTAPPLET_H
#define ABOUTAPPLET_H

#include "dcpappletif.h"
#include "aboutbusinesslogic.h"

#include <QObject>
#include <QPointer>
#include <DcpAppletIf>

class AboutWidget;

class AboutApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    AboutApplet ();
    ~AboutApplet ();

    virtual void init();
    virtual DcpWidget *constructWidget (int widgetId);
    virtual DcpWidget *pageMain (int widgetId);
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    QPointer<AboutWidget>        m_MainWidget;
    QPointer<AboutBusinessLogic> m_AboutBusinessLogic;
};
#endif
