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
#ifndef PROFILEBUTTONS_H
#define PROFILEBUTTONS_H

#include <MWidget>

class MButtonGroup;

/*!
 * A buttog group widget that holds a button for every available profile. This
 * class is able to get profile data, change the button texts when the
 * retranslation is reguired and emit/get signals about the activation and the
 * selection changes. 
 */
class ProfileButtons : public MWidget
{
    Q_OBJECT

public:
    ProfileButtons (MWidget *parent = 0);
    virtual ~ProfileButtons ();

    void init (QMap<int, QPair<QString, QString> > data, int currentId);
    QString selectedProfileName ();
    void retranslate (QMap<int, QString> languageInfo);

public slots:
    bool selectProfile(int id);

signals:
    void profileSelected(int id);

private:
    void addButton(QString iconID, QString name, int id, bool checked);
    void createLayout();

private:
    MButtonGroup *m_Buttons;
    #ifdef UNIT_TEST
    friend class Ut_ProfileButtons;
    #endif
};

#endif 
