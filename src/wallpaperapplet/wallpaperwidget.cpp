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

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <QTimer>
#include <MContentItem>
#include <QDebug>
#include <QtTracker/Tracker>

#undef DEBUG
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

    SYS_DEBUG ("--- Start ------------");
    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    setLayout (mainLayout);

    MLayout *buttonsLayout = new MLayout;

    MLinearLayoutPolicy *wallpaperLandscape =
        new MLinearLayoutPolicy (buttonsLayout, Qt::Horizontal);

    MLinearLayoutPolicy *wallpaperPortrait =
        new MLinearLayoutPolicy (buttonsLayout, Qt::Vertical);

    /*
     * The gallery item. We support this widget only when the ContentManager
     * library is available.
     */
    #ifdef HAVE_CONTENT_MANAGER
    m_GalleryItem = new MContentItem (MContentItem::IconAndSingleTextLabel);
    m_GalleryItem->setObjectName ("GalleryItem");
    m_GalleryItem->setItemMode (MContentItem::Single);
    m_GalleryItem->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    wallpaperLandscape->addItem (m_GalleryItem);
    wallpaperPortrait->addItem (m_GalleryItem);

    connect (m_GalleryItem, SIGNAL(clicked()),
            this, SLOT(galleryActivated()));
    #endif

    /*
     * The OVI item.
     */
    m_OviItem = new MContentItem(MContentItem::IconAndSingleTextLabel);
    m_OviItem->setImageID ("icon-m-common-ovi");
    m_OviItem->setObjectName ("OviItem");
    m_OviItem->setItemMode (MContentItem::Single);
    m_OviItem->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    wallpaperLandscape->addItem (m_OviItem);
    wallpaperPortrait->addItem (m_OviItem);

    connect (m_OviItem, SIGNAL(clicked()),
            this, SLOT(oviActivated()));


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
    buttonsLayout->setLandscapePolicy (wallpaperLandscape);
    buttonsLayout->setPortraitPolicy (wallpaperPortrait);
    mainLayout->addItem (buttonsLayout);
    mainLayout->addItem (m_ImageList);
    mainLayout->setStretchFactor (m_ImageList, 1);

    retranslateUi ();
    
    connect (m_WallpaperBusinessLogic, SIGNAL(imageEditRequested()),
            this, SLOT(slotImageActivated()));
    SYS_DEBUG ("--- End   ------------");
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
WallpaperWidget::retranslateUi ()
{
    //% "Get more from Ovi Store"
    m_OviItem->setTitle(qtTrId("qtn_wall_get_more_from_ovi"));
   
    #ifdef HAVE_CONTENT_MANAGER
    //% "Pick from My Gallery"
    m_GalleryItem->setTitle(qtTrId("qtn_wall_pick_from_gallery"));
    #endif
}


void 
WallpaperWidget::oviActivated ()
{
    SYS_DEBUG ("Executing %s", oviCommand);
    system (oviCommand);
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
        connect (m_ImageBrowser, SIGNAL(selectingContentItem(const QString &)),
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
