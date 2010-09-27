/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

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

//#define DEBUG
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

    portraitPreviewFileName = themeBusinessLogic->themePreviewFileName (
            themeDesc->codeName(), M::Portrait);

    landscapePreviewFileName = themeBusinessLogic->themePreviewFileName (
            themeDesc->codeName(), M::Landscape);

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
    if (pixmap.load (portraitPreviewFileName)) {
        image1->setPixmap (pixmap);
    } else {
        SYS_WARNING ("Failed to load %s", SYS_STR(portraitPreviewFileName));
        image1->setImage (themeDesc->iconName());
    }

    portraitLayoutPolicy->addItem (image1);

    image2 = new MImageWidget (this);
    if (pixmap.load (landscapePreviewFileName)) {
        image2->setPixmap (pixmap);
    } else {
        SYS_WARNING ("Failed to load %s", SYS_STR(landscapePreviewFileName));
        image2->setImage (themeDesc->iconName());
    }
    
    landscapeLayoutPolicy->addItem (image2);

    //% "Select"
    m_SelectButton = new MButton (qtTrId ("qtn_teme_select"));
    connect (m_SelectButton, SIGNAL(clicked()),
            this, SLOT(acceptClicked()));
    
    //% "Cancel"
    m_CancelButton = new MButton (qtTrId ("qtn_comm_cancel"));
    connect (m_CancelButton, SIGNAL(clicked()),
            this, SLOT(cancelClicked()));
    connect (this, SIGNAL(rejected()),
            this, SLOT(cancelClicked()));
    
    portraitButtonLayoutPolicy->addItem (m_SelectButton);
    portraitButtonLayoutPolicy->addItem (m_CancelButton);
    
    landscapeButtonLayoutPolicy->addItem (m_SelectButton);
    landscapeButtonLayoutPolicy->addItem (m_CancelButton);
    
    buttonLayout->setLandscapePolicy (landscapeButtonLayoutPolicy);
    buttonLayout->setPortraitPolicy (portraitButtonLayoutPolicy);

    portraitLayoutPolicy->addItem (buttonLayout);
    landscapeLayoutPolicy->addItem (buttonLayout);

    mainLayout->setLandscapePolicy (landscapeLayoutPolicy);
    mainLayout->setPortraitPolicy (portraitLayoutPolicy);

    hbox = new MContainer (this);
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
    accept();
    disappear ();

    m_ThemeBusinessLogic->changeTheme (m_ThemeDesc->codeName());
}

void 
ThemeDialog::cancelClicked ()
{
    SYS_DEBUG ("");
    emit themeChangeCancelled ();
    accept();
    disappear ();
}


