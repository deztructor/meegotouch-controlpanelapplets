/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "themeapplet.h"
#include "themewidget.h"
#include "themebrief.h"

#include <MTheme>
#include <MAction>

#include "../debug.h"

Q_EXPORT_PLUGIN2(themeapplet, ThemeApplet)

const QString cssDir = "/usr/share/themes/base/meegotouch/duicontrolpanel/style/";

ThemeApplet::ThemeApplet() :
    m_ThemeBusinessLogic (new ThemeBusinessLogic)
{
}

ThemeApplet::~ThemeApplet() 
{
}

void 
ThemeApplet::init()
{
    QString themeFile = cssDir + "themeapplet.css";

    SYS_DEBUG ("Loading theme file: %s", SYS_STR(themeFile));
    MTheme::loadCSS (themeFile);
}

DcpWidget *
ThemeApplet::pageMain()
{
    static int i = 0;

    SYS_DEBUG ("Entering %d", i);
    if (m_MainWidget == NULL) {
        m_MainWidget = new ThemeWidget (m_ThemeBusinessLogic);
        SYS_DEBUG ("Widget created");
    }

    SYS_DEBUG ("returning m_MainWidget: %d", i);
    ++i;
    return m_MainWidget;
}

DcpWidget *
ThemeApplet::constructWidget (
        int widgetId)
{
    static int i = 0;
    Q_UNUSED(widgetId);

    SYS_DEBUG ("We are called: %d", i);
    ++i;
    return pageMain ();
}

QString
ThemeApplet::title() const
{
    //% "Theme"
    return qtTrId ("qtn_teme_theme"); // This is not official logical id
}

QVector<MAction*>
ThemeApplet::viewMenuItems()
{
    QVector<MAction*> vector;

    return vector;
}

DcpBrief *
ThemeApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    SYS_DEBUG ("Creating brief");
    return new ThemeBrief (m_ThemeBusinessLogic);
}
