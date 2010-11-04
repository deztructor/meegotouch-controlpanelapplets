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
#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QObject>
#include <QHash>
#include <MDialog>

class MListItem;
class ProfileDataInterface;

class ProfileDialog : public MDialog
{
    Q_OBJECT

public:
    ProfileDialog(ProfileDataInterface *dataIf);
    virtual ~ProfileDialog();

signals:
    void profileChanged(int newId);

private slots:
    void openProfileSettings ();
    void currentProfileChanged (int newId);
    void clickedOnProfile ();

private:
    void createContents ();
    ProfileDataInterface            *dataIf;
    QHash<int, MListItem*>           profileList;

friend class Ut_ProfileDialog;
};

#endif
