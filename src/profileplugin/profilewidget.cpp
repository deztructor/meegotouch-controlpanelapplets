/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#include "profileplugin.h"

#define DEBUG
#include "../debug.h"

#include <MButton>
#include <MDialog>
#include <MButtonGroup>
#include <MApplication>
#include <MContainer>
#include <DuiControlPanelIf>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLocale>
#include <MStatusIndicatorMenuExtensionInterface>

#include <QGraphicsLinearLayout>

static const char *systemui_translation = "systemui-applets.qm";
static const char *profiles_translation = "profiles";

ProfileWidget::ProfileWidget (
    ProfilePlugin *profilePlugin,
    QGraphicsItem *parent) :
        MButton (parent),
        plugin (profilePlugin),
        dataIf (0),
        profileButtons (0)
{
    SYS_DEBUG ("");
    MApplication  *App = MApplication::instance ();
    dataIf = new ProfileDataInterface ();
    connect (App, SIGNAL (localeSettingsChanged ()),
             this, SLOT (loadTranslation ()));
    loadTranslation ();

    setViewType(MButton::iconType);
    setObjectName("StatusIndicatorMenuTopRowExtensionButton");
    connect(this, SIGNAL(clicked()), this, SLOT(showProfileDialog()));
    connect (dataIf, SIGNAL (currentProfile (int)),
             this, SLOT (profileChanged ()));
    profileChanged();
}

ProfileWidget::~ProfileWidget ()
{
    SYS_DEBUG ("");
    delete dataIf;
    dataIf = NULL;
}

void ProfileWidget::profileChanged()
{
    setText(dataIf->getCurrentProfileName());
    setIconID(dataIf->getCurrentProfileIconId());
}

void ProfileWidget::showProfileDialog()
{
    // Create a dialog for choosing the profile
    //% "Select Profile"
    MDialog* dialog = new MDialog(qtTrId("qtn_prof_select"), M::NoStandardButton);

    initProfileButtons();
    dialog->setCentralWidget(profileButtons);
    profileButtons->connect(profileButtons, SIGNAL(profileSelected(int)), dialog, SLOT(accept()));
    // Show the dialog
    dialog->exec();

    // Hide the status indicator menu
    if (MStatusIndicatorMenuInterface *menu = plugin->statusIndicatorMenuInterface())
    {
        menu->hideStatusIndicatorMenu();
    }
}
void
ProfileWidget::initProfileButtons ()
{

    if (profileButtons)
        return;
    profileButtons = new ProfileButtons ();
    //% "Profiles"
    QMap<int, QPair<QString, QString> > map;
    QList<ProfileDataInterface::ProfileData> l = dataIf->getProfilesData ();

    for (int i = 0; i < l.count (); ++i) {
        ProfileDataInterface::ProfileData d = l.at (i);
        map.insert (d.profileId, d.visualData);
    }
    profileButtons->init (map, dataIf->getCurrentProfile ());

    connect (dataIf, SIGNAL (currentProfile (int)),
             profileButtons, SLOT (selectProfile (int)));
    connect (profileButtons, SIGNAL (profileSelected (int)),
             dataIf, SLOT (setProfile (int)));
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

    locale.installTrCatalog (systemui_translation);
    locale.installTrCatalog (profiles_translation);
    MLocale::setDefault (locale);

    running = false;
}

void
ProfileWidget::retranslateUi ()
{
    if (profileButtons == 0)
        return;

    SYS_DEBUG ("");

    QMap<int, QString> map;
    QList<ProfileDataInterface::ProfileData> l = dataIf->getProfilesData ();

    for (int i = 0; i < l.count (); ++i) {
        ProfileDataInterface::ProfileData d = l.at (i);
        map.insert (d.profileId, d.visualData.second);
    }
    // Update the profile names [dataIf will returns a localised lists]
    profileButtons->retranslate (map);
}

