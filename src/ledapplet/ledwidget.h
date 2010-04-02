/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include "dcpwidget.h"

class DuiButton;
class DuiLabel;
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

    void illuminationToggled (bool newState);
    void eventsToggled       (bool newState);

    void illuminationLedStateReceived (bool enabled);
    void eventsLedStateReceived (bool enabled);

private:
    QString labelStringForCategory (LedWidget::CategoryType category) const;
    LedWidget::CategoryType categoryFromWidget (DuiButton *button) const;
    
    LedDBusInterface   *m_LedDBusInterface;
    DuiButton          *m_IlluminationButton;
    DuiButton          *m_EventsButton;

    DuiButton          *m_EventButtons[LastCategoryType];
    DuiLabel           *m_EventLabels[LastCategoryType];
};
#endif
