/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "profilecontainer.h"

#include <QGraphicsLinearLayout>
#include <MButton>
#include <MLabel>
#include <QDebug>

#undef DEBUG
#include "../debug.h"

ProfileContainer::ProfileContainer (
        int            id, 
        const QString &title, 
        bool           vibra, 
        MWidget     *parent) :
    MWidget (parent),
    m_ProfileId (id),
    m_Button(0),
    m_Label(0)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    SYS_DEBUG ("Creating container for %s", SYS_STR(title)); 

    m_Button = new MButton();
    m_Button->setCheckable (true);
    m_Button->setViewType (MButton::switchType);
    m_Button->setChecked (vibra);
    connect(m_Button, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));

    m_Label = new MLabel(title);

    layout->addItem(m_Label);
    layout->addItem(m_Button);

    setLayout(layout);
}

ProfileContainer::~ProfileContainer()
{
    SYS_DEBUG ("");
    delete m_Button;
    delete m_Label;
}

int 
ProfileContainer::id ()
{
    return m_ProfileId;
}

bool ProfileContainer::isChecked () const
{
    return m_Button->isChecked();
}

void ProfileContainer::setChecked (bool buttonChecked)
{
    m_Button->setChecked(buttonChecked);
}

void ProfileContainer::setText (const QString &text)
{
    m_Label->setText(text);
}

QString ProfileContainer::text () const
{
    return m_Label->text();
}
