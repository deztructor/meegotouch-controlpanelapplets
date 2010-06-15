/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef PROFILEAPPLET_H
#define PROFILEAPPLET_H

#include "dcpappletif.h"
#include <QObject>

class ProfileDataInterface;
class ProfileWidget;
class MAction;
class ProfileBrief;

class ProfileApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    ProfileApplet();
    ~ProfileApplet();
    
    virtual void init();
    virtual DcpWidget* constructWidget(int widgetId);
    virtual DcpWidget* pageMain();
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    QPointer<ProfileWidget> m_MainWidget;
    QPointer<ProfileDataInterface> m_Api;

    #ifdef UNIT_TEST
    friend class Ut_ProfileApplet;
    #endif
};
#endif 
