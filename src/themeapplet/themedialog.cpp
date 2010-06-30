/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themedialog.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <MButton>
#include <QGraphicsLinearLayout>
#include <MContainer>
#include <MImageWidget>

//#define DEBUG
#include "../debug.h"

ThemeDialog::ThemeDialog (
        ThemeBusinessLogic *themeBusinessLogic,
        ThemeDescriptor    *themeDesc):
    MDialog (),
    m_ThemeBusinessLogic (themeBusinessLogic),
    m_ThemeDesc (themeDesc)
{  
    QGraphicsLinearLayout *buttonLayout, *mainLayout;
    MContainer  *hbox;
    MButton *selectButton;
    MButton *cancelButton;
    MImageWidget *image1;

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    buttonLayout = new QGraphicsLinearLayout;

    // An image widget to show some preview screenshot.
    image1 = new MImageWidget (themeDesc->iconName());
    mainLayout->addItem (image1);

    //% "Select"
    selectButton = new MButton (qtTrId ("qtn_teme_select"));
    connect (selectButton, SIGNAL(clicked()),
            this, SLOT(acceptClicked()));
    
    //% "Cancel"
    cancelButton = new MButton (qtTrId ("qtn_comm_cancel"));
    connect (cancelButton, SIGNAL(clicked()),
            this, SLOT(cancelClicked()));
    
    buttonLayout->addItem (selectButton);
    buttonLayout->setStretchFactor (selectButton, 1);
    buttonLayout->addItem (cancelButton);
    buttonLayout->setStretchFactor (cancelButton, 1);

    mainLayout->addItem (buttonLayout);
    
    hbox = new MContainer;
    hbox->setHeaderVisible (false);
    hbox->setLayout (mainLayout);

    setCentralWidget (hbox);
    setButtonBoxVisible (false);

    setTitle (m_ThemeDesc->name());

    connect (m_ThemeBusinessLogic, SIGNAL(themeChanged(QString)),
            this, SLOT(themeChanged(QString)));
}

void
ThemeDialog::showDialog()
{
    appear(MSceneWindow::DestroyWhenDone);
}

void 
ThemeDialog::acceptClicked ()
{
    SYS_DEBUG ("");
    m_ThemeBusinessLogic->changeTheme (m_ThemeDesc->codeName());
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
    Q_UNUSED(themeId);
    SYS_DEBUG ("*** themeId = %s", SYS_STR(themeId));
    deleteLater();
}
