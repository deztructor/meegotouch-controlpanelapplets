/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebrief.h"

#define DEBUG
#include "../debug.h"

ThemeBrief::ThemeBrief (
        ThemeBusinessLogic *businessLogic) :
    m_ThemeBusinessLogic (businessLogic)
{
}

QString 
ThemeBrief::valueText() const
{
    Q_ASSERT (m_ThemeBusinessLogic != 0);
    return m_ThemeBusinessLogic->currentThemeName ();
}

