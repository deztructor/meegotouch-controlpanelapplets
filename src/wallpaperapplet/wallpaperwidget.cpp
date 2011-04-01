/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <QTimer>
#include <MContentItem>
#include <MAction>
#include <QGraphicsLinearLayout>
#include <QDBusInterface>
#include <MLabel>

#include "wallpapereditorsheet.h"

//#define DEBUG
#define WARNING
#include "../debug.h"

static const int MaxColumns = 2;
static const qreal navBarTransparency = 0.85;

/******************************************************************************
 *
 */
WallpaperWidget::WallpaperWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic),
    m_NavigationBarTransparency (-1)
{
    MWindow *win = MApplication::activeWindow ();

    SYS_WARNING ("Creating widget");
    if (win) {
        m_NavigationBarTransparency = 
            win->property("navigationBarOpacity").toReal();
        win->setProperty ("navigationBarOpacity", navBarTransparency);
    }

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
    SYS_WARNING ("Destroying widget");
    if (m_NavigationBarTransparency >= 0) {
        MWindow *win = MApplication::activeWindow ();

        if (win) {
            win->setProperty ("navigationBarOpacity", 
                    m_NavigationBarTransparency);
        }
    }
}

void
WallpaperWidget::createContent ()
{
    MLayout              *layout;
    MLinearLayoutPolicy  *mainLayout;

    /*
     * Creating a layout that holds the rows of the internal widgets.
     */
    layout = new MLayout (this);
    mainLayout = new MLinearLayoutPolicy (layout, Qt::Vertical);
    mainLayout->setContentsMargins (0., 0., 0., 0.);
    mainLayout->setSpacing (0.);
    setLayout (layout);

    /*
     * Adding the header/title
     */
    addHeaderContainer (mainLayout);

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

void 
WallpaperWidget::addHeaderContainer (
        MLinearLayoutPolicy *mainLayout)
{
    MContainer            *container;
    QGraphicsLinearLayout *layout;

    Q_ASSERT (mainLayout);

    SYS_DEBUG ("***********************************************************");
    /*
     * Creating a lcontainer and a layout.
     */
    container = new MContainer (this);
    container->setStyleName ("CommonXLargeHeaderPanelInverted");
    container->setHeaderVisible (false);
    container->setContentsMargins (0., 0., 0., 0.);
    //container->setSpacing (0.);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);
    container->centralWidget()->setLayout (layout);

    /*
     * The label that we use as title.
     */
    //% "Wallpaper"
    m_TitleLabel = new MLabel (qtTrId("qtn_wall_wallpaper"));
    m_TitleLabel->setStyleName ("CommonXLargeHeaderInverted");
    layout->addItem (m_TitleLabel);
    layout->setAlignment (m_TitleLabel, Qt::AlignLeft);

    /*
     * Adding the whole row to the main container.
     */
    mainLayout->addItem (container);
    mainLayout->setStretchFactor (container, 0);
}

void
WallpaperWidget::retranslateUi ()
{
    if (m_TitleLabel)
        //% "Wallpaper"
        m_TitleLabel->setText (qtTrId("qtn_wall_wallpaper"));
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
    /*
     * With this define
     */
#if 0
    WallpaperEditorSheet  *sheet;
    sheet = new WallpaperEditorSheet (m_WallpaperBusinessLogic);
    sheet->appear(scene(), MSceneWindow::DestroyWhenDone);
#else
    emit changeWidget (1);
#endif
}


void 
WallpaperWidget::oviActivated ()
{
    static const char OviIf[] = "com.nokia.OviStoreClient";
    QDBusInterface OviStore (OviIf, "/", OviIf);

    OviStore.call ("LaunchWithKeyword", QString ("graphics"));
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
    action = new MAction("icon-m-toolbar-gallery-white", "", this);
    action->setLocation(MAction::ToolBarLocation);
    page->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(galleryActivated()));
    #endif

    /*
     * Adding the ovi action.
     */
    action = new MAction("icon-m-toolbar-content-ovi-music-white", "", this);
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
        m_ImageBrowser = new ContentItemsPage (this);
        m_ImageBrowser->setContentTypes (
          QStringList() <<
            "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image");

        m_ImageBrowser->setObjectName (
                "SelectSingleContentItemPage_imageBrowser");
        m_ImageBrowser->setStyleName ("CommonApplicationPageInverted");
        m_ImageBrowser->setCommonLayoutSuffix ("Inverted");

        connect (m_ImageBrowser, SIGNAL (backButtonClicked ()),
                 m_ImageBrowser, SLOT (dismiss ()));
        connect (m_ImageBrowser, SIGNAL (itemClicked (const QString &)),
                 SLOT (galleryImageSelected (const QString &)));
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
    SYS_WARNING ("m_ImageBrowser->unselectItem (%s);", SYS_STR(uri));
    if (m_ImageBrowser)
        m_ImageBrowser->unselectItem (uri);
}
#endif

