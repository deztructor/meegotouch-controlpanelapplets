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

#include "wallpaperwidget.h"
#include "wallpaperlist.h"

#include <MApplicationPage>
#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MContainer>
#include <QTimer>
#include <MContentItem>
#include <MAction>
#include <QDebug>
#include <QtTracker/Tracker>

//#define DEBUG
#define WARNING
#include "../debug.h"

static const int MaxColumns = 2;

static const char *oviCommand = "webwidgetrunner /usr/share/webwidgets/applications/d34177b1c241ea44cb132005b63ee6527c9f6040-wrt-widget.desktop -widgetparameter graphics &";

WallpaperWidget::WallpaperWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic)
{
    /*
     * Unfortunatelly the content has to be created late, otherwise the list
     * only shows one row.
     * FIXME: I'm n ot sure how late should this be done, I thing we should use
     * the void MApplicationPage::createContent(), but I don't know how to do
     * that.
     */
    //createContent ();
    QTimer::singleShot(100, this, SLOT(createContent()));
}

WallpaperWidget::~WallpaperWidget ()
{
}

void
WallpaperWidget::createContent ()
{
    QGraphicsLinearLayout *mainLayout;

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->setContentsMargins (0, 0, 0, 0);
    setLayout (mainLayout);

    /*
     * The list of the available images.
     */
    m_ImageList = new WallpaperList (m_WallpaperBusinessLogic, this);
    m_ImageList->setObjectName("WallpaperImageList");
    connect (m_ImageList, SIGNAL(imageActivated(WallpaperDescriptor *)),
            this, SLOT(slotImageActivated(WallpaperDescriptor *)));

    m_ImageList->setDataSourceType (WallpaperList::DataSourceLocal);


    /*
     * Adding all widgets into the layout.
     */
    mainLayout->addItem (m_ImageList);
    mainLayout->setStretchFactor (m_ImageList, 1);

    connect (m_WallpaperBusinessLogic, SIGNAL(imageEditRequested()),
            this, SLOT(slotImageActivated()));
}


/*
 * This slot is called when the user activates an image in the list. The
 * WallpaperList has a signal for that.
 */
void 
WallpaperWidget::slotImageActivated (
        WallpaperDescriptor *desc)
{
    SYS_DEBUG ("*** desc = %s", SYS_STR(desc->basename()));
    m_WallpaperBusinessLogic->startEdit (desc);
}

/*
 * When this slot is activated the image has to be already loaded, so that the
 * editor widget can be shown without delay.
 */
void 
WallpaperWidget::slotImageActivated ()
{
    SYS_DEBUG ("");
    
    emit changeWidget (1);
}


void 
WallpaperWidget::oviActivated ()
{
    SYS_DEBUG ("Executing %s", oviCommand);
    system (oviCommand);
}

void
WallpaperWidget::polishEvent ()
{
    QGraphicsWidget  *parent;
    MApplicationPage *page = 0;
    MAction          *action;
    
    /*
     * We need to find the MApplicationPage among our parents.
     */
    parent = parentWidget();
    while (parent) {
        page = qobject_cast <MApplicationPage *>(parent);
        if (page)
            break;
        parent = parent->parentWidget();
    }

    if (!page)
        return;

    /**************************************************************************
     * Hiding the home button. 
     */
    page->setComponentsDisplayMode (
            MApplicationPage::HomeButton,
            MApplicationPageModel::Hide);

    /*
     * Adding the gallery action.
     */
    #ifdef HAVE_CONTENT_MANAGER
    action = new MAction("icon-m-content-gallery", "", this);
    action->setLocation(MAction::ToolBarLocation);
    page->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(galleryActivated()));
    #endif

    /*
     * Adding the ovi action.
     */
    action = new MAction("icon-m-common-ovi", "", this);
    action->setLocation(MAction::ToolBarLocation);
    page->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(oviActivated()));
}

#ifdef HAVE_CONTENT_MANAGER
/*!
 * Slot that is activated when the user clicked on the gallery item, the widget
 * that activates the ContentManager page. We support this only when the
 * ContentManager library is available.
 */
void 
WallpaperWidget::galleryActivated ()
{
    /*
     * Please note that the m_ImageBrowser is a QPointer object that will reset
     * itself to NULL when the widget is destroyed. It will also initialize
     * itself to NULL when it is created.
     */
    if (!m_ImageBrowser) {
        m_ImageBrowser = new SelectSingleContentItemPage (
                QString(), 
                QStringList() << "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image", 
                QString());

        m_ImageBrowser->setObjectName (
                "SelectSingleContentItemPage_imageBrowser");

        connect (m_ImageBrowser, SIGNAL(backButtonClicked()), 
                m_ImageBrowser, SLOT(dismiss()));
        connect (m_ImageBrowser, SIGNAL(contentItemSelected(const QString &)),
                this, SLOT(galleryImageSelected(const QString &)));
    }
     
    m_ImageBrowser->appear (MSceneWindow::DestroyWhenDismissed);
}
#endif


#ifdef HAVE_CONTENT_MANAGER
/*!
 * Slot that activated when the user picks an image from the content manager
 * page. We only need this slot when the ContentManager library is activated.
 */
void 
WallpaperWidget::galleryImageSelected (
        const QString &uri)
{
    m_WallpaperBusinessLogic->addImageFromGallery (uri);
}
#endif
