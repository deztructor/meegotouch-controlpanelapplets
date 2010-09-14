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

#include "profilebuttons.h"

#include <QHash>
#include <MButton>
#include <MButtonGroup>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <QGraphicsLinearLayout>

#define DEBUG
#include "../debug.h"

static const int nButtonColumns = 2;

ProfileButtons::ProfileButtons (
        MWidget *parent) :
    MWidget (parent),
    m_Buttons (0)
{
    SYS_DEBUG ("");
}

ProfileButtons::~ProfileButtons()
{
    SYS_DEBUG ("");
}

/*!
 * This method will initialize the button group by adding the buttons. The map
 * parameter will hold a map with the integer profile ID and a pair of icon IDs
 * and profile names.
 */
void
ProfileButtons::init (
        QMap<int, QPair<QString, QString> > data,
        int currentId)
{
    SYS_DEBUG ("");

    /*
     * Creating the button group. FIXME: This method does two separate things;
     * it creates widgets and sets layouts but it also initializes the widgets
     * with input data. Maybe it would be clearer to separate these things.
     */
    m_Buttons = new MButtonGroup;
    connect (m_Buttons, SIGNAL (buttonClicked (int)),
             this, SIGNAL (profileSelected (int)));

    QList<int> ids = data.keys ();
    for (int i = 0; i < data.count (); ++i) {
        int id = ids.at (i);
        addButton (
                data.value (id).first,
                data.value (id).second,
                id, (currentId == id));
    }

    createLayout ();
}

/*!
 * \param id The numerical ID of the profile to select.
 * \returns true if the button for the profile has been found.
 */
bool
ProfileButtons::selectProfile (
        int id)
{
    bool retval = false;

    SYS_DEBUG ("id = %d", id);

    if (m_Buttons == NULL)
        return retval;

    QList<MButton*> buttonList = m_Buttons->buttons();
    for (int i = 0; i < buttonList.size(); ++i) {
        MButton *button = buttonList.at (i);
        bool       checked;

        checked = id == m_Buttons->id (button);
        button->setChecked (checked);
        if (checked)
            retval = true;
    }

    return retval;
}

/*!
 * \returns The translated UI string from the checked button or an empty string
 *   if there is no selected button.
 */
QString
ProfileButtons::selectedProfileName ()
{
    SYS_DEBUG ("");
    if (m_Buttons == NULL)
        return QString ("");

    MButton *button = m_Buttons->checkedButton();
    if (button)
        return button->text();

    return QString("");
}

/*!
 * \param languageInfo A map with profile IDs and profile names
 *
 * With this method it is possible to change the button texts. The argument is a
 * map with profile IDs and UI strings that will represent the profiles with the
 * given IDs.
 */
void
ProfileButtons::retranslate (
        QMap<int, QString> languageInfo)
{
    SYS_DEBUG ("");
    if (m_Buttons == 0)
        return;

    QList<int> ids = languageInfo.keys();

    for (int i = 0; i < languageInfo.count(); ++i)
    {
        int id = ids.at (i);
        if (m_Buttons->button (id) != 0)
            m_Buttons->button (id)->setText (languageInfo.value (id));
    }
}

/*!
 * Creates the layout for the profile buttons.
 */
void
ProfileButtons::createLayout ()
{
    SYS_DEBUG ("");
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    QList<MButton*>       buttonList = m_Buttons->buttons();

    // Storing the buttons in the layout.
    for (int i = 0; i < buttonList.size(); ++i) {
        layout->addItem (buttonList[i]);
    }

    setLayout (layout);
}

/*!
 * \param iconId Logical ID string for the icon representing the profile
 * \param name profile name UI string
 * \param id The profile ID for the new button
 * \param checked If the button will be representing the selected profile
 *
 * This low level method will be used to add a new button to the button group.
 */
void
ProfileButtons::addButton (
        QString   iconId,
        QString   name,
        int       id,
        bool      checked)
{
    MButton *button;
    SYS_DEBUG ("*** iconId  = %s", SYS_STR(iconId));
    SYS_DEBUG ("*** name    = %s", SYS_STR(name));
    SYS_DEBUG ("*** id      = %d", id);
    SYS_DEBUG ("*** checked = %s", SYS_BOOL(checked));

    Q_ASSERT (m_Buttons);
    button = new MButton (iconId, name);
    button->setCheckable (true);
    button->setChecked (checked);
    m_Buttons->addButton (button, id);
}

