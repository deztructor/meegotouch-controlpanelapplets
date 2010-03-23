/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themedialog.h"
#include <themebusinesslogic.h>

#define DEBUG
#include "../debug.h"

ThemeDialog::ThemeDialog (
        ThemeBusinessLogic *themeBusinessLogic,
        const QString      &themeName):
    DuiDialog (),
    m_ThemeBusinessLogic (themeBusinessLogic),
    m_ThemeName (themeName)
{
    addButton ("Select");
    addButton ("Cancel");
    setTitle (m_ThemeName);
}

void
ThemeDialog::showDialog()
{
    appear(DuiSceneWindow::DestroyWhenDone);
}
