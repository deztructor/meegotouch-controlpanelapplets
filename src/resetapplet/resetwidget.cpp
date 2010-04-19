/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "resetwidget.h"

#define DEBUG
#include "../debug.h"

static const int MaxColumns = 2;

ResetWidget::ResetWidget (
        ResetBusinessLogic     *resetBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_ResetBusinessLogic (resetBusinessLogic)
{
}

ResetWidget::~ResetWidget ()
{
}

#if 0
void
ResetWidget::createContent ()
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
#endif


