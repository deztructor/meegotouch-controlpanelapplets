/*
 * profile.cpp
 *
 * This file is part of mstatusindicatormenu
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#include "profilewidget.h"
#include "profiledatainterface.h"
#include "profilebuttons.h"

#undef DEBUG
#include "../debug.h"

#include <MButton>
#include <MButtonGroup>
#include <MApplication>
#include <MContainer>
#include <DuiControlPanelIf>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLocale>
#include <MStatusIndicatorMenuPluginInterface>

#include <QGraphicsLinearLayout>

#define SYSTEMUI_TRANSLATION "systemui-applets"

ProfileWidget::ProfileWidget (
    MStatusIndicatorMenuInterface &statusIndicatorMenu,
    QGraphicsItem *parent) :
        MWidget (parent),
        statusIndicatorMenu (statusIndicatorMenu),
        dataIf (0),
        profileButtons (0)
{
    MApplication  *App = MApplication::instance ();

    Q_UNUSED(statusIndicatorMenu);
    dataIf = new ProfileDataInterface ();

    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);

    setLayout (mainLayout);
    mainLayout->setContentsMargins (0, 0, 0, 0);

    connect (App, SIGNAL (localeSettingsChanged ()),
             this, SLOT (loadTranslation ()));
    loadTranslation ();

    // Create a container for the profiles
    initProfileButtons ();

    connect (dataIf, SIGNAL (currentProfile (int)),
             profileButtons, SLOT (selectProfile (int)));

    mainLayout->addItem (profileButtons);
}

ProfileWidget::~ProfileWidget ()
{
    delete dataIf;
    dataIf = NULL;
}

void
ProfileWidget::initProfileButtons ()
{
    profileButtons = new ProfileButtons ();
    //% "Profiles"
    profileButtons->setTitle (qtTrId ("qtn_prof_profile"));
    QMap<int, QString> map;
    QList<ProfileDataInterface::ProfileData> l = dataIf->getProfilesData ();

    for (int i = 0; i < l.count (); ++i) {
        ProfileDataInterface::ProfileData d = l.at (i);
        map.insert (d.profileId, d.profileName);
    }
    profileButtons->init (map, dataIf->getCurrentProfile ());

    connect (profileButtons, SIGNAL (headerClicked ()),
             this, SLOT (showProfileModificationPage ()));
    connect (profileButtons, SIGNAL (profileSelected (int)),
             dataIf, SLOT (setProfile (int)));
}

void
ProfileWidget::showProfileModificationPage ()
{
    // instantiate the interface
    DuiControlPanelIf cpIf;
    // check the interface is valid
    if (!cpIf.isValid ())
        return;
    cpIf.appletPage ("Profile");
}

void
ProfileWidget::loadTranslation ()
{
    static bool running = false;
    SYS_DEBUG ("");

    if (running == true)
        return;
    running = true;

    MLocale       locale;

    SYS_DEBUG ("Language changed to '%s'",
               SYS_STR (locale.language ()));

    locale.installTrCatalog (SYSTEMUI_TRANSLATION ".qm");
    locale.installTrCatalog (SYSTEMUI_TRANSLATION);
    MLocale::setDefault (locale);

    running = false;
}

void
ProfileWidget::retranslateUi ()
{
    if (profileButtons == 0)
        return;

    SYS_DEBUG ("");

    profileButtons->setTitle (qtTrId ("qtn_prof_profile"));

    QMap<int, QString> map;
    QList<ProfileDataInterface::ProfileData> l = dataIf->getProfilesData ();

    for (int i = 0; i < l.count (); ++i) {
        ProfileDataInterface::ProfileData d = l.at (i);
        map.insert (d.profileId, d.profileName);
    }
    // Update the profile names [dataIf will returns a localised lists]
    profileButtons->retranslate (map);
}

