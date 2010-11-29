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
#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QPointer>
#include "dcpwidget.h"
#include "profiledatainterface.h"

class MLabel;
class MContainer;
class ProfileContainer;

class ProfileWidget : public DcpWidget
{
    Q_OBJECT

public:
    ProfileWidget (
            ProfileDataInterface *api,
            QGraphicsWidget *parent = 0);
    virtual ~ProfileWidget();

    void retranslateUi ();

protected:
    void initWidget();

private slots:
   void initProfiles();
   void setVibration (int profileId, bool enabled);
   void vibrationChanged (bool enabled);
   void selectionChanged ();
   void profileChanged (int id);

private:
    MContainer* createContainer();

private:
    QPointer<ProfileDataInterface>   m_ProfileIf;
    QHash<int, ProfileContainer*>    m_Containers;
    bool                             m_switchEnabled;
};
#endif
