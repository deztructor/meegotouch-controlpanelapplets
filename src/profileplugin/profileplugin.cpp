/*
 * profileplugin.cpp
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

#include "profileplugin.h"
#include "profilewidget.h"

Q_EXPORT_PLUGIN2(profile, ProfilePlugin);

ProfilePlugin::ProfilePlugin() : statusIndicatorMenu(0), profile(0)
{
}

void ProfilePlugin::setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &menuInterface)
{
    statusIndicatorMenu = &menuInterface;
}

MStatusIndicatorMenuInterface *ProfilePlugin::statusIndicatorMenuInterface() const
{
    return statusIndicatorMenu;
}

// Methods derived from MApplicationExtensionInterface
bool ProfilePlugin::initialize(const QString &)
{
    profile = new ProfileWidget(this);

    return true;
}

QGraphicsWidget *ProfilePlugin::widget()
{
    return profile;
}

