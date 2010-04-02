/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <QObject>
#include <DuiLayout>
#include <DuiLabel>
#include <DuiButton>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>

#include "ledwidget.h"
#include "leddbusinterface.h"

#define FOREACHCATEGORY(n) for((n) = 0; (n) < LastCategoryType; ++n)

#define DEBUG
#include "../debug.h"

LedWidget::LedWidget (
        LedDBusInterface  *dbusIf,
        QGraphicsWidget   *parent):
    DcpWidget (parent),
    m_LedDBusInterface (dbusIf)
{
    SYS_DEBUG ("");
    initWidget ();

    connect (m_LedDBusInterface, SIGNAL(eventsLedStateReceived(int)),
            this, SLOT(eventsLedStateReceived(int)));

    m_LedDBusInterface->eventsLedStateRequired ();
}

/*!
 * Ok, this widget is not even half ready, I have to on some other stuff now...
 */
void
LedWidget::initWidget ()
{
    DuiLayout               *mainLayout;
    DuiGridLayoutPolicy     *landscapePolicy;
    DuiLinearLayoutPolicy   *portraitPolicy;
    int                      n;

    SYS_DEBUG ("");

    mainLayout = new DuiLayout (this);
    landscapePolicy = new DuiGridLayoutPolicy (mainLayout);
    portraitPolicy  = new DuiLinearLayoutPolicy (mainLayout, Qt::Vertical);
    mainLayout->setLandscapePolicy (landscapePolicy);
    mainLayout->setPortraitPolicy (portraitPolicy);

    FOREACHCATEGORY(n) {
        m_EventLabels[n] = new DuiLabel (
                labelStringForCategory((LedWidget::CategoryType) n));
        m_EventButtons[n] = new DuiButton ();
        m_EventButtons[n]->setViewType (DuiButton::switchType);
        m_EventButtons[n]->setCheckable (true);
        
        portraitPolicy->addItem (
                m_EventLabels[n], Qt::AlignLeft | Qt::AlignVCenter);
        portraitPolicy->addItem (
                m_EventButtons[n], Qt::AlignRight | Qt::AlignVCenter);
        
        landscapePolicy->addItem(m_EventLabels[n], n, 0);
        landscapePolicy->addItem(m_EventButtons[n], n, 1);
    
        connect (m_EventButtons[n], SIGNAL (toggled(bool)),
            this, SLOT (eventButtonToggled(bool)));
    }
}

void 
LedWidget::eventsToggled (
        bool newState)
{
    m_LedDBusInterface->setEventsLedState (newState);
}

void 
LedWidget::eventButtonToggled (
        bool newState)
{
    DuiButton *button = qobject_cast<DuiButton *> (sender());
    LedWidget::CategoryType category;

    Q_UNUSED (newState);
    SYS_DEBUG ("*** newState = %s", SYS_BOOL(newState));

    if (button == 0) {
        SYS_WARNING ("The sender is not a button?");
        return;
    }

    category = categoryFromWidget (button);
    SYS_DEBUG ("*** category = %d", category);

}

void 
LedWidget::eventsLedStateReceived (
        int enabledLeds)
{
    int   n;
    bool  checked;

    SYS_DEBUG ("*** enabledLeds = %d", enabledLeds);

    FOREACHCATEGORY(n) {
        checked = enabledLeds & (1 << n);
        SYS_DEBUG ("checked[%d] = %s", n, SYS_BOOL(checked));
        if (m_EventButtons[n])
            m_EventButtons[n]->setChecked (checked);
    }
}


QString 
LedWidget::labelStringForCategory (
        LedWidget::CategoryType category) const
{
    switch (category) {
        case MissedCall:
            //% "Missed call"
            return qtTrId ("qtn_ligh_missed_call");

        case SMSReceived:
            //% "SMS received"
            return qtTrId ("qtn_ligh_sms");

        case EmailReceived:
            //% "Email message received"
            return qtTrId ("qtn_ligh_email");

        case InstantMessageReceived:
            //% "Instant message received"
            return qtTrId ("qtn_ligh_im");

        case Charging:
            //% "Charging"
            return qtTrId ("qtn_ligh_charging");
        
        case OtherNotifications:
            //% "Other notifications"
            return qtTrId ("qtn_ligh_other_notifications");
    }

    return "";
}

LedWidget::CategoryType 
LedWidget::categoryFromWidget (
        DuiButton *button) const
{
    int n;

    FOREACHCATEGORY(n) {
        if (button == m_EventButtons[n])
            return (LedWidget::CategoryType) n;
    }

    return LedWidget::LastCategoryType;
}

