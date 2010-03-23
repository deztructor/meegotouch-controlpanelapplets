/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themedialog.h"
#include "themebusinesslogic.h"

#include <DuiButton>
#include <QGraphicsLinearLayout>
#include <DuiContainer>

#define DEBUG
#include "../debug.h"

ThemeDialog::ThemeDialog (
        ThemeBusinessLogic *themeBusinessLogic,
        const QString      &themeName):
    DuiDialog (),
    m_ThemeBusinessLogic (themeBusinessLogic),
    m_ThemeName (themeName)
{  
    QGraphicsLinearLayout *layout;
    DuiContainer  *hbox;
    DuiButton *selectButton;
    DuiButton *cancelButton;

    layout = new QGraphicsLinearLayout;

    //% "Select"
    selectButton = new DuiButton (qtTrId ("qtn_theme_select"));
    connect (selectButton, SIGNAL(clicked()),
            this, SLOT(acceptClicked()));
    
    //% "Cancel"
    cancelButton = new DuiButton (qtTrId ("qtn_theme_cancel"));
    connect (cancelButton, SIGNAL(clicked()),
            this, SLOT(cancelClicked()));

    layout->addItem (selectButton);
    layout->setStretchFactor (selectButton, 1);
    layout->addItem (cancelButton);
    layout->setStretchFactor (cancelButton, 1);
    
    hbox = new DuiContainer;
    hbox->setHeaderVisible (false);
    hbox->setLayout (layout);

    setCentralWidget (hbox);
    setButtonBoxVisible (false);

    setTitle (m_ThemeName);

    connect (m_ThemeBusinessLogic, SIGNAL(themeChanged(QString)),
            this, SLOT(themeChanged(QString)));
}

void
ThemeDialog::showDialog()
{
    appear(DuiSceneWindow::DestroyWhenDone);
}

void 
ThemeDialog::acceptClicked ()
{
    SYS_DEBUG ("");
    m_ThemeBusinessLogic->changeTheme (m_ThemeName);
    //deleteLater ();
}

void 
ThemeDialog::cancelClicked ()
{
    SYS_DEBUG ("");
    deleteLater ();
}

void
ThemeDialog::themeChanged (
        QString themeId)
{
    SYS_DEBUG ("*** themeId = %s", SYS_STR(themeId));
    deleteLater();
}
