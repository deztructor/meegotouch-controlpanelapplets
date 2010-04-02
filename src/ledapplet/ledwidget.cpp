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

    connect (m_LedDBusInterface, SIGNAL(illuminationLedStateReceived(bool)),
            this, SLOT(illuminationLedStateReceived(bool)));
    
    connect (m_LedDBusInterface, SIGNAL(eventsLedStateReceived(bool)),
            this, SLOT(eventsLedStateReceived(bool)));

    m_LedDBusInterface->illuminationLedStateRequired ();
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
#if 0
    /*
     * This was the old implementation before the UI spec was received (this
     * version was created using the design brief as source). I think we can
     * keep this piece of code for some time.
     */
    DuiLabel                *label1, *label2;

    label1 = new DuiLabel ("Illumination light");

    m_IlluminationButton = new DuiButton ();
    m_IlluminationButton->setViewType (DuiButton::switchType);
    m_IlluminationButton->setCheckable (true);

    label2 = new DuiLabel ("Incoming events");

    m_EventsButton = new DuiButton ();
    m_EventsButton->setViewType (DuiButton::switchType);
    m_EventsButton->setCheckable (true);

    /*
     * Well, no UI spec yet, so I'm not sure this is what we want...
     */
    portraitPolicy->addItem (label1, Qt::AlignLeft | Qt::AlignVCenter);
    portraitPolicy->addItem (m_IlluminationButton, Qt::AlignRight);
    portraitPolicy->addItem (label2, Qt::AlignLeft | Qt::AlignVCenter);
    portraitPolicy->addItem (m_EventsButton, Qt::AlignRight);
   
    landscapePolicy->addItem(label1, 0, 0);
    landscapePolicy->addItem(m_IlluminationButton, 0, 1);
    landscapePolicy->addItem(label2, 1, 0);
    landscapePolicy->addItem(m_EventsButton, 1, 1);

    connect (m_IlluminationButton, SIGNAL (toggled(bool)),
            this, SLOT (illuminationToggled(bool)));
    connect (m_EventsButton, SIGNAL (toggled(bool)),
            this, SLOT (eventsToggled(bool)));
#endif
}

void 
LedWidget::illuminationToggled (
        bool newState)
{
    SYS_DEBUG ("*** state = %s", newState ? "true" : "false");
    m_LedDBusInterface->setIlluminationLedState (newState);
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
LedWidget::illuminationLedStateReceived (
        bool enabled)
{
#if 0
    SYS_DEBUG ("enabled = %s", enabled ? "yes" : "no");
    m_IlluminationButton->setChecked (enabled);
#endif
}

void 
LedWidget::eventsLedStateReceived (
        bool enabled)
{
#if 0
    SYS_DEBUG ("enabled = %s", enabled ? "yes" : "no");
    m_EventsButton->setChecked (enabled);
#endif
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

