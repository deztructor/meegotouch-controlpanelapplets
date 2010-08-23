/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYAPPLET_H
#define BATTERYAPPLET_H

#include <DcpAppletIf>
#include <QObject>
#include <QPointer>

class DcpWidget;
class MAction;
class BatteryWidget;

class BatteryApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    BatteryApplet ();
    ~BatteryApplet ();
    
    virtual void init();
    virtual DcpWidget *constructWidget(int widgetId);
    virtual DcpWidget *pageMain();
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId = 0);

private slots:
    void userGuide ();

private:
    QPointer<BatteryWidget> m_MainWidget;
    #ifdef UNIT_TEST
    friend class Ut_BatteryApplet;
    #endif
};
#endif
