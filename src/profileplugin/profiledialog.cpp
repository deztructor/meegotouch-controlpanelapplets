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
#include "profiledialog.h"
#include "profiledatainterface.h"
#include <QGraphicsLinearLayout>
#include <QDBusInterface>
#include <MListItem>
#include <MLabel>
#include <QList>
#include <MButton>
#include <MImageWidget>
#include <MListItem>
#include <MListItem>

#ifndef UNIT_TEST
#include <MLibrary>
M_LIBRARY
#endif

#define DEBUG
#include "../debug.h"


ProfileDialog::ProfileDialog(ProfileDataInterface *dataIf) :
    MDialog (),
    dataIf (dataIf)
{
    setSystem (true);
    setObjectName ("ProfileChooserDialog");

    //% "Select profile"
    setTitle (qtTrId ("qtn_prof_select"));

    createContents ();
    // and init the contents:
    currentProfileChanged (dataIf->getCurrentProfile ());
}

ProfileDialog::~ProfileDialog()
{

}

void
ProfileDialog::createContents ()
{
    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->setContentsMargins (0, 0, 0, 0);
    mainLayout->setSpacing (0);

    QList<ProfileDataInterface::ProfileData> profiles =
        dataIf->getProfilesData ();

    for (int x = 0; x < profiles.count (); x++)
    {
        ProfileDataInterface::ProfileData i = profiles.at (x);

        QGraphicsLinearLayout *itemLayout =
            new QGraphicsLinearLayout (Qt::Horizontal);
        itemLayout->setContentsMargins (0, 0, 0, 0);

        MListItem *item = new MListItem (this);
        item->setStyleName ("CommonSmallPanelInverted");
        item->setLayout (itemLayout);

        MImageWidget *icon = new MImageWidget (item);
        icon->setStyleName ("CommonTitleIcon");
        icon->setImage (dataIf->mapId2IconId (i.profileId));
        icon->setZoomFactor (1.0);

        itemLayout->addItem (icon);
        itemLayout->setAlignment (icon, Qt::AlignVCenter | Qt::AlignLeft);

        MLabel *label = new MLabel (item);
        label->setObjectName ("CommonSingleTitleInverted");
        label->setText (i.profileName);

        itemLayout->addItem (label);
        itemLayout->setAlignment (label, Qt::AlignVCenter | Qt::AlignLeft);

        profileList.insert (i.profileId, item);

        mainLayout->addItem (item);

        connect (item, SIGNAL (clicked ()), SLOT (clickedOnProfile ()));
    }

    connect (dataIf, SIGNAL (currentProfile (int)),
             SLOT (currentProfileChanged (int)));

    MButton *vibraSettingsButton = new MButton (this);
    vibraSettingsButton->setStyleName ("CommonSingleButtonInverted");
    //% "Vibration settings"
    vibraSettingsButton->setText (qtTrId ("qtn_prof_settings"));

    connect (vibraSettingsButton, SIGNAL (clicked ()),
             SLOT (openProfileSettings ()));

    mainLayout->addItem(vibraSettingsButton);
    mainLayout->setAlignment(vibraSettingsButton, Qt::AlignCenter);

    MWidget *centralWidget = new MWidget;
    centralWidget->setLayout (mainLayout);

    setCentralWidget (centralWidget);
}

void
ProfileDialog::currentProfileChanged (int newId)
{
    // deselect all
    foreach (MListItem *item, profileList)
        item->setSelected (false);

    // and select the new one
    profileList.value (newId)->setSelected (true);
}

void
ProfileDialog::clickedOnProfile ()
{
    // get the id from the hash list 'profileList' by the sender widget
    int id = profileList.key (static_cast<MListItem*> (this->sender ()));

    SYS_DEBUG ("profileId = %d", id);

    // select the current profile
    currentProfileChanged (id);

    emit profileChanged (id);

    // set the profile in the backend
    dataIf->setProfile (id);

    // and close the dialog
    accept ();
}

void
ProfileDialog::openProfileSettings ()
{
    // create a DBus interface for controlpanel
    QDBusInterface dcpIf ("com.nokia.DuiControlPanel",
                          "/", "com.nokia.DuiControlPanelIf");

    // lets close the dialog
    accept ();

    // and finally call the "Profile" applet open method on dcpIf
    dcpIf.call (QDBus::Block, "appletPage", "Profile");
}

