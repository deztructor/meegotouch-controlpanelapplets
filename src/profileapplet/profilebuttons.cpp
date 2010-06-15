/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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


void 
ProfileButtons::init (
        QMap<int, QPair<QString, QString> > data, 
        int currentId)
{
    SYS_DEBUG ("");
    m_Buttons = new MButtonGroup();
    connect (m_Buttons, SIGNAL(buttonClicked(int)), 
            this, SIGNAL(profileSelected(int)));

    QList<int> ids = data.keys();
    for (int i = 0; i < data.count(); ++i) {
        int id = ids.at(i);
        addButton (data.value(id).first, data.value(id).second ,id, (currentId == id));
    }
    createLayout();
}

/*!
 *
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

/*!
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
 * Creates the layout for the profile buttons. Takes the profile selection
 * buttons from m_Buttons and sets a portrait and a landscape layout for them.
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

void
ProfileButtons::retranslate (QMap<int, QString> data)
{
    SYS_DEBUG ("");
    if (m_Buttons == 0)
        return;

    QList<int> ids = data.keys();

    for (int i = 0; i < data.count(); ++i)
    {
        int id = ids.at (i);
        if (m_Buttons->button (id) != 0)
            m_Buttons->button (id)->setText (data.value (id));
    }
}

