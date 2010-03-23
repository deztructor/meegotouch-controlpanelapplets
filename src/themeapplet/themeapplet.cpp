/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "themeapplet.h"
#include "themewidget.h"
#include "themebrief.h"

#include <DuiTheme>
#include <DuiAction>

Q_EXPORT_PLUGIN2(themeapplet, ThemeApplet)

const QString cssDir = "/usr/share/themes/base/dui/duicontrolpanel/style/";

ThemeApplet::ThemeApplet() 
{
}

ThemeApplet::~ThemeApplet() 
{
}

void 
ThemeApplet::init()
{
    DuiTheme::loadCSS(cssDir + "themeapplet.css");
}

DcpWidget *
ThemeApplet::pageMain()
{
    if (m_MainWidget == NULL)
        m_MainWidget = new ThemeWidget;

    return m_MainWidget;
}

DcpWidget *
ThemeApplet::constructWidget (
        int widgetId)
{
    Q_UNUSED(widgetId);

    return pageMain();
}

QString
ThemeApplet::title() const
{
    //% "Themes"
    return qtTrId ("qtn_theme_theme"); // This is not official logical id
}

QVector<DuiAction*>
ThemeApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    //% "Help"
    DuiAction* helpAction = new DuiAction (qtTrId ("qtn_comm_help"), 
            pageMain ());

    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenuLocation);

    return vector;
}

DcpBrief *
ThemeApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new ThemeBrief;
}
