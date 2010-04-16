/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperwidget.h"
#include "wallpaperlist.h"

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <QTimer>
#define DEBUG
#include "../debug.h"

static const int MaxColumns = 2;

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
    QTimer::singleShot(500, this, SLOT(createContent()));
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
