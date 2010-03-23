/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEAPPLET_H
#define THEMEAPPLET_H

#include "dcpappletif.h"
#include "themewidget.h"
#include "themebusinesslogic.h"

#include <QObject>
#include <DcpAppletIf>

class ThemeApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    ThemeApplet ();
    ~ThemeApplet ();

    virtual void init();
    virtual DcpWidget* constructWidget(int widgetId);
    virtual DcpWidget* pageMain();
    virtual QString title() const;
    virtual QVector<DuiAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int partId);

private:
    QPointer<ThemeWidget> m_MainWidget;
    QPointer<ThemeBusinessLogic> m_ThemeBusinessLogic;
};
#endif
