/*
 * profileplugin.h
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

#ifndef PROFILEPLUGIN_H
#define PROFILEPLUGIN_H

#include <MStatusIndicatorMenuExtensionInterface>
#include <QObject>

class ProfileWidget;
class MStatusIndicatorMenuInterface;

// This class doesn't do anything else but creates a widget
class ProfilePlugin : public QObject, public MStatusIndicatorMenuExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(MStatusIndicatorMenuExtensionInterface MApplicationExtensionInterface)
public:
    ProfilePlugin();
    // Getter for the status indicator menu interface
    MStatusIndicatorMenuInterface *statusIndicatorMenuInterface() const;

    // Methods derived from MStatusIndicatorMenuPlugin
    virtual void setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &menuInterface);

    // Methods derived from MApplicationExtensionInterface
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();

private:
    MStatusIndicatorMenuInterface *statusIndicatorMenu;
    ProfileWidget *profile;
};

#endif // PROFILEPLUGIN_H
