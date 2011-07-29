/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <MLabel>
#include <MSlider>
#include <QVariant>
#include <MGConfItem>

#undef DEBUG
#include "../debug.h"

const QString switchEnabledKey =
    "/meegotouch/settings/profileapplet/switch_enabled";

ProfileWidget::ProfileWidget (
        ProfileDataInterface *api,
        QGraphicsWidget      *parent) :
    DcpWidget (parent),
    m_ProfileIf (api),
    m_ProfileSlider (0)
{
    SYS_DEBUG ("");

    MGConfItem switchEnabled(switchEnabledKey);
    m_switchEnabled = switchEnabled.value (true).toBool ();

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
    QGraphicsLinearLayout *mainLayout;
    QGraphicsLinearLayout *vibraLayout;
    

    /*
     *
     */
    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    vibraLayout = new QGraphicsLinearLayout (Qt::Vertical);

    SYS_DEBUG ("We have %d profiles.", l.count());
    mainLayout->setContentsMargins (0., 0., 0., 0.);
    mainLayout->setSpacing (0.);
    vibraLayout->setContentsMargins (0., 0., 0., 0.);
    vibraLayout->setSpacing (0.);

    MContainer *headerContainer = new MContainer;
    headerContainer->setHeaderVisible (false);
    headerContainer->setStyleName ("CommonLargeHeaderPanelInverted");

    QGraphicsLinearLayout *headerLayout =
        new QGraphicsLinearLayout (Qt::Horizontal);
    headerLayout->setContentsMargins (0., 0., 0., 0.);

    MLabel *titleLabel = new MLabel;
    //% "Profile"
    titleLabel->setText (qtTrId ("qtn_prof_profile"));
    titleLabel->setStyleName ("CommonLargeHeaderInverted");
    headerLayout->addItem (titleLabel);

    headerLayout->addStretch ();


    headerContainer->centralWidget ()->setLayout (headerLayout);

    mainLayout->addItem (headerContainer);

    /*
     *
     */
    m_ProfileSlider = createSlider ();
    mainLayout->addItem (m_ProfileSlider);

    /*
     *
     */
    MContainer *listContainer = new MContainer;
    listContainer->setHeaderVisible (false);
    listContainer->setStyleName ("CommonPanelInverted");
    listContainer->setContentsMargins (0., 0., 0., 0.);

    /*
     *
     */
    MLabel *vibraLabel = new MLabel;
    //% "Vibration"
    vibraLabel->setText (qtTrId ("qtn_prof_vibration"));
    vibraLabel->setStyleName ("CommonHeaderInverted");
    vibraLabel->setAlignment (Qt::AlignBottom | Qt::AlignRight);
    vibraLayout->addItem (vibraLabel);

    // create profile containers
    for (int i = 0; i < l.count(); ++i) {
        ProfileDataInterface::ProfileData d = l.at(i);
        ProfileContainer* cont = new ProfileContainer(
            d.profileId,
            d.profileName,
            d.vibrationEnabled);
        // For testability driver: set some object name...
        cont->setObjectName (ProfileDataInterface::mapId (d.profileId));
        cont->setIconId(m_ProfileIf->mapId2StatusIconId(d.profileId));

        connect (cont, SIGNAL (toggled (bool)), SLOT (vibrationChanged (bool)));

        if (m_switchEnabled)

        m_Containers.insert(d.profileId, cont);
        vibraLayout->addItem(cont);

        if (m_switchEnabled)
        {
            /*
             * If profile switching is enabled, connect the clicked signal,
             * and select the profile
             */
            cont->setSelected (d.profileId == m_ProfileIf->getCurrentProfile ());
            connect (cont, SIGNAL (clicked ()), SLOT (selectionChanged ()));
        }
    }
    listContainer->centralWidget ()->setLayout (vibraLayout);

    /*
     * mainLayout
     */
    mainLayout->addItem (listContainer);
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
    /*
     * INFO: this slot is called only when profile-switiching
     * feature is set to 'true'
     */
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
     * No-op if profile-switching feature is disabled...
     */
    if (m_switchEnabled == false)
        return;

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

MSlider *
ProfileWidget::createSlider ()
{
    MSlider  *slider;
    int       currentProfile;

    currentProfile = m_ProfileIf->getCurrentProfile ();

    slider = new MSlider;
    slider->setStyleName ("CommonSliderInverted");
    slider->setObjectName ("ProfileSlider");
    slider->setRange (0, 3);
    slider->setHandleLabelVisible (true);
    slider->setValue (currentProfile);
    slider->setHandleLabel (intToSliderTitle(currentProfile));


    connect (slider, SIGNAL(valueChanged(int)),
            this, SLOT(slidervalueChanged(int)));
    return slider;
}

void 
ProfileWidget::slidervalueChanged (
        int value)
{
    SYS_DEBUG ("*** calue = %d", value);
    m_ProfileIf->setProfile (value);
    m_ProfileSlider->setHandleLabel (intToSliderTitle(value));
}

QString 
ProfileWidget::intToSliderTitle (
        int sliderValue)
{
    switch (sliderValue) {
        case 0:
            //% "Silent"
            return qtTrId("qtn_volu_silent");

        case 1:
            //% "Beep"
            return qtTrId("qtn_volu_beep");

        case 2:
            //% "Ringing"
            return qtTrId("qtn_volu_ringer");
        
        case 3:
            //% "Profile"
            return qtTrId("qtn_volu_profile");
    }

    return QString();
}
