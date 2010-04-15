/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include "dcpwidget.h"

class MButton;
class MLabel;
class LedDBusInterface;

class LedWidget : public DcpWidget
{
    Q_OBJECT

public:
    LedWidget (LedDBusInterface *dbusIf, QGraphicsWidget *parent = 0);

    typedef enum {
        MissedCall               = 0,
        SMSReceived,
        EmailReceived,
        InstantMessageReceived,
        Charging,
        OtherNotifications,
        LastCategoryType
    } CategoryType;

protected:
    void initWidget (void);

protected slots:
    void eventButtonToggled  (bool newState);
    void eventsLedStateReceived (int enabledLeds);

private:
    QString labelStringForCategory (LedWidget::CategoryType category) const;
    LedWidget::CategoryType categoryFromWidget (MButton *button) const;
    
    LedDBusInterface   *m_LedDBusInterface;
    MButton          *m_EventButtons[LastCategoryType];
    MLabel           *m_EventLabels[LastCategoryType];
};
#endif
