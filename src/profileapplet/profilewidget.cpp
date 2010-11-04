/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "profilewidget.h"
#include "profilecontainer.h"
#include "dcpprofile.h"

#include <QGraphicsLinearLayout>
#include <MContainer>
#include <QDebug>

#undef DEBUG
#include "../debug.h"

ProfileWidget::ProfileWidget (
        ProfileDataInterface *api,
        QGraphicsWidget      *parent) :
    DcpWidget (parent),
    m_ProfileIf (api)
{
    SYS_DEBUG ("");
    initWidget ();
}

ProfileWidget::~ProfileWidget ()
{
}


void 
ProfileWidget::initWidget ()
{
    // catch profile If actions
    connect (m_ProfileIf, SIGNAL(vibrationValue(int, bool)), 
             SLOT(setVibration(int, bool)));

    // get init values
    initProfiles ();

    // widgets're initialized, connect to
    // backends profile-changed signal
    connect (m_ProfileIf, SIGNAL (currentProfile (int)),
             SLOT (profileChanged (int)));
}

void 
ProfileWidget::initProfiles ()
{
    QList<ProfileDataInterface::ProfileData> l = m_ProfileIf->getProfilesData();
    //% "Vibration"
    MContainer *mContainer = new MContainer(qtTrId("qtn_prof_vibration"));
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    QGraphicsLinearLayout *vibraLayout = new QGraphicsLinearLayout (Qt::Vertical);
    
    SYS_DEBUG ("We have %d profiles.", l.count());
    mainLayout->setContentsMargins (0., 0., 0., 0.);
    vibraLayout->setContentsMargins (0., 0., 0., 0.);
    vibraLayout->setSpacing(0.);

    // create profile containers
    for (int i = 0; i < l.count(); ++i) {
        ProfileDataInterface::ProfileData d = l.at(i);
        ProfileContainer* cont = new ProfileContainer(
            d.profileId,
            d.profileName,
            d.vibrationEnabled);
        // For testability driver: set some object name...
        cont->setObjectName (ProfileDataInterface::mapId (d.profileId));
        cont->setSelected(false);
        cont->setIconId(m_ProfileIf->mapId2StatusIconId(d.profileId));

        connect (cont, SIGNAL (toggled (bool)), SLOT (vibrationChanged (bool)));
        connect (cont, SIGNAL (clicked ()), SLOT (selectionChanged ()));

        m_Containers.insert(d.profileId, cont);
        vibraLayout->addItem(cont);

        cont->setSelected (d.profileId == m_ProfileIf->getCurrentProfile ());
    }

    /*
     * mainLayout
     */
    mContainer->centralWidget()->setLayout(vibraLayout);

    mainLayout->addItem(mContainer);
    mainLayout->addStretch ();

    setLayout (mainLayout);
}

void 
ProfileWidget::vibrationChanged (
        bool enabled)
{
    //NOTE: MButton->isChecked() method returns the state before the 
    // press at this point
    ProfileContainer *profile =
        static_cast<ProfileContainer*> (this->sender ());

    SYS_DEBUG ("valuechanged for %s: %d",
               SYS_STR (profile->text ()), index);

    m_ProfileIf->setVibration (profile->id (), enabled);
}

void
ProfileWidget::selectionChanged ()
{
    ProfileContainer *profile =
        static_cast<ProfileContainer*> (this->sender ());

    /* deselect all */
    foreach (ProfileContainer *cont, m_Containers)
        cont->setSelected (false);
    /* and select the current */
    profile->setSelected (true);

    m_ProfileIf->setProfile (profile->id ());
}

void
ProfileWidget::profileChanged (int id)
{
    /* 
     * this function called when the profile is changed
     * in an other process
     * [eg.: status-indicator-menu profile selection dialog]
     */

    /* deselect all */
    foreach (ProfileContainer *cont, m_Containers)
        cont->setSelected (false);

    /* and select the current */
    m_Containers.value(id)->setSelected (true);
}

void 
ProfileWidget::setVibration (
        int profileId, 
        bool enabled)
{
    SYS_DEBUG ("set vibration for profile: %d, enabled: %s",
               profileId, SYS_BOOL (enabled));

    ProfileContainer *cont = m_Containers.value (profileId);

    if (cont)
        cont->setChecked (enabled);

}

void
ProfileWidget::retranslateUi ()
{
    SYS_DEBUG ("");

    // Re-load the profiles-data [with the new translations]
    QList<ProfileDataInterface::ProfileData> l = m_ProfileIf->getProfilesData();

    for (int i = 0; i < l.count(); ++i) {
        ProfileDataInterface::ProfileData d = l.at (i);

        // Update the containers title field
        m_Containers.value (d.profileId)->setText (d.profileName);
    }
}

