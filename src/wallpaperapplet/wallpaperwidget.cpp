/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperwidget.h"
#include "wallpaperlist.h"

#include <QGraphicsLinearLayout>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiContainer>

#define DEBUG
#include "../debug.h"

static const int MaxColumns = 2;

WallpaperWidget::WallpaperWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic)
{
    createWidgets ();
    retranslateUi ();
}

WallpaperWidget::~WallpaperWidget ()
{
}

void
WallpaperWidget::createWidgets ()
{
    QGraphicsLinearLayout *mainLayout;

    m_LocalContainer = createContainer (WallpaperWidget::ThemeLocal);
    m_OviContainer = createContainer (WallpaperWidget::ThemeOvi);

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->addItem (m_LocalContainer);
    mainLayout->addItem (m_OviContainer);

    this->setLayout (mainLayout);
}

DuiContainer * 
WallpaperWidget::createContainer (
        WallpaperWidget::ThemeCategoryId category)
{
    DuiLinearLayoutPolicy *policy;
    DuiLayout *layout = new DuiLayout();

    policy = new DuiLinearLayoutPolicy (layout, Qt::Vertical);
    layout->setPolicy (policy);
    
    DuiContainer *container = new DuiContainer ();
    container->centralWidget()->setLayout (layout);

    switch (category) {
        case WallpaperWidget::ThemeLocal:
            m_LocalLayoutPolicy = policy;

            m_LocalList = new WallpaperList (m_WallpaperBusinessLogic);
            m_LocalList->setDataSourceType (WallpaperList::DataSourceLocal);
            connect (
                    m_LocalList, SIGNAL(imageActivated(WallpaperDescriptor *)),
                    this, SLOT(slotImageActivated(WallpaperDescriptor *)));

            policy->addItem (m_LocalList);
            break;

        case WallpaperWidget::ThemeOvi:
            m_OviLayoutPolicy = policy;
    }

    return container;
}

void
WallpaperWidget::retranslateUi ()
{
    if (m_LocalContainer) {
        /*
         * FIXME: This is not an official locale id.
         */
        //% "Available wallpapers"
        m_LocalContainer->setTitle (qtTrId ("qtn_wallpapers_locale"));
    }

    if (m_OviContainer) {
        /*
         * FIXME: This is not an official locale id.
         */
        //% "Ovi wallpapers"
        m_OviContainer->setTitle (qtTrId ("qtn_wallpapers_ovi"));
    }
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
