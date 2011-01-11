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
#include "profiledialog.h"
#include "profileplugin.h"

#include <MLabel>
#include <MImageWidget>
#include <DuiControlPanelIf>
#include <MLocale>
#include <MStatusIndicatorMenuExtensionInterface>
#include <QGraphicsLinearLayout>

#undef DEBUG
#include "../debug.h"

static const char *profiles_translation = "profiles";

ProfileWidget::ProfileWidget (
    ProfilePlugin *profilePlugin,
    QGraphicsItem *parent) :
        MBasicListItem (MBasicListItem::IconWithTitleAndSubtitle, parent),
        plugin (profilePlugin),
        dataIf (0)
{
    MLocale       locale;

    SYS_DEBUG ("");
    dataIf = new ProfileDataInterface ();

    // load our translation catalogue...
    locale.installTrCatalog (profiles_translation);
    MLocale::setDefault (locale);

    setStyleName ("CommonBasicListItemInverted");

    connect (this, SIGNAL (clicked ()), this, SLOT (showProfileDialog ()));
    connect (dataIf, SIGNAL (currentProfile (int)), SLOT (profileChanged ()));

    profileChanged ();
}

ProfileWidget::~ProfileWidget ()
{
    SYS_DEBUG ("");

    delete dataIf;
    dataIf = 0;
}

void
ProfileWidget::profileChanged()
{
    SYS_DEBUG ("");
    QString iconId;

    //% "Profile"
    setTitle (qtTrId ("qtn_prof_profile"));

    iconId = dataIf->mapId2StatusIconId (dataIf->getCurrentProfile ());

    setSubtitle (dataIf->getCurrentProfileName ());

    imageWidget ()->setImage (iconId);
}

void
ProfileWidget::showProfileDialog ()
{
    ProfileDialog   *dialog = new ProfileDialog;
    MStatusIndicatorMenuInterface *menu;

    menu = plugin->statusIndicatorMenuInterface ();
    if (menu) {
        menu->hideStatusIndicatorMenu ();
    }

    connect (dialog, SIGNAL (profileChanged (int)),
             SLOT (profileChanged ()));

    dialog->exec ();

    delete dialog;
}

void
ProfileWidget::retranslateUi ()
{
    //% "Profile"
    setTitle (qtTrId ("qtn_prof_profile"));
    setSubtitle (dataIf->getCurrentProfileName ());
}

