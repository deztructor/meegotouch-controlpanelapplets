/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebrief.h"
#include <dcpwidgettypes.h>

#define DEBUG
#include "../debug.h"

ThemeBrief::ThemeBrief (
        ThemeBusinessLogic *businessLogic) :
    m_ThemeBusinessLogic (businessLogic)
{
}

int
ThemeBrief::widgetTypeID() const
{
    SYS_DEBUG ("");
    return DcpWidgetType::Image;
}

QString 
ThemeBrief::valueText() const
{
    Q_ASSERT (m_ThemeBusinessLogic != 0);
    return m_ThemeBusinessLogic->currentThemeName ();
}

QString 
ThemeBrief::icon () const
{
    Q_ASSERT (m_ThemeBusinessLogic != 0);

    SYS_DEBUG ("================================");
    SYS_DEBUG ("Returning %s", 
            SYS_STR(m_ThemeBusinessLogic->currentThemeIconName ()));
    return m_ThemeBusinessLogic->currentThemeIconName ();
}

QString 
ThemeBrief::image () const
{
    Q_ASSERT (m_ThemeBusinessLogic != 0);

    SYS_DEBUG ("================================");
    SYS_DEBUG ("Returning %s", 
            SYS_STR(m_ThemeBusinessLogic->currentThemeIconName ()));
    return m_ThemeBusinessLogic->currentThemeIconName ();
}
