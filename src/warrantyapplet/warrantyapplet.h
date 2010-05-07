/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WARRANTYAPPLET_H
#define WARRANTYAPPLET_H

#include "dcpappletif.h"
#include "warrantybusinesslogic.h"

#include <QObject>
#include <QPointer>
#include <DcpAppletIf>

class WarrantyWidget;

class WarrantyApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    WarrantyApplet ();
    ~WarrantyApplet ();

    virtual void init();
    virtual DcpWidget *constructWidget (int widgetId);
    virtual DcpWidget *pageMain (int widgetId);
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    QPointer<WarrantyWidget>        m_MainWidget;
    QPointer<WarrantyBusinessLogic> m_WarrantyBusinessLogic;
};
#endif
