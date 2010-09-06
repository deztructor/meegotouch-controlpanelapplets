/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperwidget.h"
#include "wallpaperlist.h"

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <QTimer>
#include <MContentItem>
#include <SelectSingleContentItemPage.h>
#include <QDebug>
#include <QtTracker/Tracker>

#define DEBUG
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
     * The gallery item.
     */
    m_GalleryItem = new MContentItem (MContentItem::IconAndSingleTextLabel);
    m_GalleryItem->setObjectName ("GalleryItem");
    m_GalleryItem->setItemMode (MContentItem::Single);
    m_GalleryItem->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    wallpaperLandscape->addItem (m_GalleryItem);
    wallpaperPortrait->addItem (m_GalleryItem);

    connect (m_GalleryItem, SIGNAL(clicked()),
            this, SLOT(galleryActivated()));

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
    
    m_WallpaperBusinessLogic->setEditedImage (desc);
    m_WallpaperBusinessLogic->startEdit ();
    
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
   
    //% "Pick from My Gallery"
    m_GalleryItem->setTitle(qtTrId("qtn_wall_pick_from_gallery"));
}


void 
WallpaperWidget::oviActivated ()
{
    SYS_DEBUG ("Executing %s", oviCommand);
    system (oviCommand);
}

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

void 
WallpaperWidget::galleryImageSelected (
        const QString &uri)
{
    m_WallpaperBusinessLogic->addImageFromGallery (uri);
}

