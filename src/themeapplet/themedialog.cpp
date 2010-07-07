/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themedialog.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <QGraphicsLinearLayout>
#include <QPixmap>

#include <MButton>
#include <MContainer>
#include <MImageWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>

#define DEBUG
#include "../debug.h"

/*
 * You might wonder why this dialog is damn complicated, why do we hide the
 * button box and create a custom one. The answer is simple: the original UI
 * spec was different, so we needed to create and handle our own buttons and
 * this is how that is done.
 * 
 * if you think you can simplify the applet by modifying ths dialog, then by all
 * means, please do it!
 */
ThemeDialog::ThemeDialog (
        ThemeBusinessLogic *themeBusinessLogic,
        ThemeDescriptor    *themeDesc):
    MDialog (),
    m_ThemeBusinessLogic (themeBusinessLogic),
    m_ThemeDesc (themeDesc)
{  
    MLayout               *buttonLayout;
    MLinearLayoutPolicy   *portraitButtonLayoutPolicy;
    MLinearLayoutPolicy   *landscapeButtonLayoutPolicy;

    MLayout               *mainLayout;
    MLinearLayoutPolicy   *portraitLayoutPolicy;
    MLinearLayoutPolicy   *landscapeLayoutPolicy;

    MContainer            *hbox;
    MImageWidget          *image1, *image2;
    QString                landscapePreviewFileName;
    QString                portraitPreviewFileName;
    QPixmap pixmap;

    portraitPreviewFileName = 
        "/usr/share/themes/" +
        themeDesc->codeName() + 
        "/meegotouch/images/meegotouch-theme-preview-portrait.jpg";
    landscapePreviewFileName = 
        "/usr/share/themes/" +
        themeDesc->codeName() + 
        "/meegotouch/images/meegotouch-theme-preview-landscape.jpg";

    mainLayout = new MLayout;
    portraitLayoutPolicy =
        new MLinearLayoutPolicy (mainLayout, Qt::Vertical);
    landscapeLayoutPolicy =
        new MLinearLayoutPolicy (mainLayout, Qt::Vertical);
    landscapeLayoutPolicy->setSpacing (10);
    portraitLayoutPolicy->setSpacing (10);

    buttonLayout = new MLayout;
    portraitButtonLayoutPolicy =
        new MLinearLayoutPolicy (buttonLayout, Qt::Vertical);
    landscapeButtonLayoutPolicy =
        new MLinearLayoutPolicy (buttonLayout, Qt::Horizontal);
    landscapeButtonLayoutPolicy->setSpacing (10);
    portraitButtonLayoutPolicy->setSpacing (10);

    // An image widget to show some preview screenshot.

    image1 = new MImageWidget (this);
    if (pixmap.load (portraitPreviewFileName))
        image1->setPixmap (pixmap);
    else
        image1->setImage (themeDesc->iconName());

    portraitLayoutPolicy->addItem (image1);

    image2 = new MImageWidget (this);
    if (pixmap.load (landscapePreviewFileName))
        image2->setPixmap (pixmap);
    else
        image2->setImage (themeDesc->iconName());
    
    landscapeLayoutPolicy->addItem (image2);

    //% "Select"
    m_SelectButton = new MButton (qtTrId ("qtn_teme_select"));
    connect (m_SelectButton, SIGNAL(clicked()),
            this, SLOT(acceptClicked()));
    
    //% "Cancel"
    m_CancelButton = new MButton (qtTrId ("qtn_comm_cancel"));
    connect (m_CancelButton, SIGNAL(clicked()),
            this, SLOT(cancelClicked()));
    
    portraitButtonLayoutPolicy->addItem (m_SelectButton);
    //buttonLayout->setStretchFactor (m_SelectButton, 1);
    portraitButtonLayoutPolicy->addItem (m_CancelButton);
    //buttonLayout->setStretchFactor (m_CancelButton, 1);
    
    landscapeButtonLayoutPolicy->addItem (m_SelectButton);
    //buttonLayout->setStretchFactor (m_SelectButton, 1);
    landscapeButtonLayoutPolicy->addItem (m_CancelButton);
    //buttonLayout->setStretchFactor (m_CancelButton, 1);
    
    buttonLayout->setLandscapePolicy (landscapeButtonLayoutPolicy);
    buttonLayout->setPortraitPolicy (portraitButtonLayoutPolicy);

    portraitLayoutPolicy->addItem (buttonLayout);
    landscapeLayoutPolicy->addItem (buttonLayout);

    mainLayout->setLandscapePolicy (landscapeLayoutPolicy);
    mainLayout->setPortraitPolicy (portraitLayoutPolicy);

    hbox = new MContainer;
    hbox->setHeaderVisible (false);
    hbox->setLayout (mainLayout);

    setCentralWidget (hbox);
    setButtonBoxVisible (false);

    setTitle (m_ThemeDesc->name());
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
    accept();
    disappear ();
}

void 
ThemeDialog::cancelClicked ()
{
    SYS_DEBUG ("");
    accept();
    disappear ();
}


