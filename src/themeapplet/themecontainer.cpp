/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themecontainer.h"

#include <QString>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

#define DEBUG
#include "../debug.h"


ThemeContainer::ThemeContainer (
        const QString &themeName,
        DuiWidget     *parent) :
    DuiLayout (parent),
    m_ThemeName (themeName)
{
    createWidgets ();
}

ThemeContainer::~ThemeContainer ()
{
}

void
ThemeContainer::createWidgets ()
{
    DuiLinearLayoutPolicy  *layoutPolicy;

    layoutPolicy = new DuiLinearLayoutPolicy (this, Qt::Vertical);
    setPolicy (layoutPolicy);
    /*
     * A label with the name of the theme. 
     */
    m_NameLabel = new DuiLabel (m_ThemeName);
    m_NameLabel->setObjectName ("ThemeNameLabel");
    layoutPolicy->addItem (m_NameLabel, Qt::AlignRight);
}
