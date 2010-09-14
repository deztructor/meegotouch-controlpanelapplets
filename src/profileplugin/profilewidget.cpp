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
#include "profiledatainterface.h"
#include "profilebuttons.h"
#include "profileplugin.h"

#undef DEBUG
#include "../debug.h"

#include <MButton>
#include <MDialog>
#include <MButtonGroup>
#include <MContainer>
#include <DuiControlPanelIf>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLocale>
#include <MStatusIndicatorMenuExtensionInterface>

#include <QGraphicsLinearLayout>

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
    dataIf = new ProfileDataInterface ();

    loadTranslation ();

    setViewType (MButton::iconType);
    setObjectName("StatusIndicatorMenuTopRowExtensionButton");
    connect(this, SIGNAL (clicked ()), this, SLOT (showProfileDialog ()));
    connect (dataIf, SIGNAL (currentProfile (int)),
             this, SLOT (profileChanged ()));

    profileChanged ();
}

ProfileWidget::~ProfileWidget ()
{
    SYS_DEBUG ("");

    if (! profileDialog.isNull ())
        delete profileDialog.data ();

    delete dataIf;
    dataIf = 0;
}

void
ProfileWidget::profileChanged()
{
    /*
     * UI spec changed, we are not using the current parofile name in the status
     * menu any more.
     */
    //setText(dataIf->getCurrentProfileName());
    //% "Profile"
    setText (qtTrId ("qtn_prof_profile"));

    setIconID(dataIf->getCurrentProfileIconId ());
}

void ProfileWidget::showProfileDialog ()
{
    MStatusIndicatorMenuInterface *menu;

    menu = plugin->statusIndicatorMenuInterface ();
    if (menu) {
        menu->hideStatusIndicatorMenu ();
    }

    if (profileDialog.isNull ())
    {
        // Create a dialog for choosing the profile
        //% "Select Profile"
        profileDialog = new MDialog (qtTrId ("qtn_prof_select"),
                                     M::NoStandardButton);
        profileDialog->setButtonBoxVisible (false);

        initProfileButtons ();
        profileDialog->setCentralWidget (profileButtons);
        connect (profileButtons, SIGNAL (profileSelected (int)),
                 profileDialog, SLOT (accept ()));

        // Needed because the dialog will be shown on the hidden
        // status-menu-window
        profileDialog->setModal (false);
        profileDialog->setSystem (true);
    }

    profileDialog->exec ();
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
    MLocale       locale;

    locale.installTrCatalog (profiles_translation);
    MLocale::setDefault (locale);
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

