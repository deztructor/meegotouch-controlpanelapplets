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

    SYS_DEBUG ("");
    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    setLayout (mainLayout);
  
    /*
     * The gallery item.
     */
    m_GalleryItem = new MContentItem (MContentItem::IconAndSingleTextLabel);
    m_GalleryItem->setObjectName ("GalleryItem");
    m_GalleryItem->setItemMode (MContentItem::Single);

    connect (m_GalleryItem, SIGNAL(clicked()),
            this, SLOT(galleryActivated()));
    /*
     * The OVI item.
     */
    m_OviItem = new MContentItem(MContentItem::IconAndSingleTextLabel);
    m_OviItem->setImageID ("icon-m-common-ovi");
    m_OviItem->setObjectName ("OviItem");
    m_OviItem->setItemMode (MContentItem::Single);

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
    mainLayout->addItem (m_GalleryItem);
    mainLayout->addItem (m_OviItem);
    mainLayout->addItem (m_ImageList);
    mainLayout->setStretchFactor (m_ImageList, 1);

    retranslateUi ();
    
    connect (m_WallpaperBusinessLogic, SIGNAL(imageEditRequested()),
            this, SLOT(slotImageActivated()));
    m_WallpaperBusinessLogic->checkForPendingSignals ();
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
    emit changeWidget (1);
}

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
    SYS_DEBUG ("");
    if (m_noImageBrowser) {
            SelectSingleContentItemPage *imageBrowser = new SelectSingleContentItemPage(QString(), QStringList() << "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image", QString());
            imageBrowser->setObjectName("SelectSingleContentItemPage_imageBrowser");
            QObject::connect(imageBrowser, SIGNAL(backButtonClicked()), imageBrowser, SLOT(dismiss()));
            QObject::connect(imageBrowser, SIGNAL(backButtonClicked()), this, SLOT(imageBrowserDismissed()));
            QObject::connect(imageBrowser, SIGNAL(selectingContentItem(const QString &)), this, SLOT(galleryImageSelected(const QString &)));
            imageBrowser->appear(MSceneWindow::DestroyWhenDismissed);
            m_noImageBrowser = false;
    }
}

void WallpaperWidget::galleryImageSelected(const QString &uri)
{
    m_WallpaperBusinessLogic->addImageFromGallery(uri);
}

void WallpaperWidget::imageBrowserDismissed()
{
    m_noImageBrowser = true;
}
