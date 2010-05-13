/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebrief.h"
#include <dcpwidgettypes.h>

#include "../debug.h"

ThemeBrief::ThemeBrief (
        ThemeBusinessLogic *businessLogic) :
    m_ThemeBusinessLogic (businessLogic)
{
    connect (businessLogic, SIGNAL(themeChanged(QString)),
            this, SLOT(themeChanged(QString)));
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

    SYS_DEBUG ("Returning %s", 
            SYS_STR(m_ThemeBusinessLogic->currentThemeIconName ()));
    return m_ThemeBusinessLogic->currentThemeIconName ();
}

void 
ThemeBrief::themeChanged (
        QString themeCodeName)
{
    Q_UNUSED (themeCodeName);
    SYS_DEBUG ("*** themeCodeName = %s", SYS_STR(themeCodeName));

    emit valuesChanged();
}

