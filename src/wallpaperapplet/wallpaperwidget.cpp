/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperwidget.h"

#include <QGraphicsLinearLayout>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
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

    //DuiLayout *layout = new DuiLayout();
    
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
    DuiGridLayoutPolicy *policy;
    DuiLayout *layout = new DuiLayout();
    
    policy = new DuiGridLayoutPolicy (layout);
    policy->setColumnStretchFactor (0, 1);
    policy->setColumnStretchFactor (1, 1);
    layout->setPolicy (policy);
    
    DuiContainer *container = new DuiContainer ();
    container->centralWidget()->setLayout (layout);

    switch (category) {
        case WallpaperWidget::ThemeLocal:
            m_LocalLayoutPolicy = policy;
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

void
WallpaperWidget::readLocalThemes ()
{
#if 0
    Q_ASSERT (m_ThemeBusinessLogic != 0);
    Q_ASSERT (m_LocalLayoutPolicy != 0);

    QList<ThemeDescriptor *> themeList = 
        m_ThemeBusinessLogic->availableThemes ();
    int n = 0;
    foreach (ThemeDescriptor *theme, themeList) {
        ThemeContainer *themeContainer;
        int x = n / MaxColumns;
        int y = n % MaxColumns;

        if (!theme->isVisible()) {
            delete theme;
            continue;
        }
        //SYS_DEBUG ("Theme name[%d] = %s", n, SYS_STR(themeName));
        themeContainer = new ThemeContainer (theme, m_ThemeBusinessLogic);
        m_LocalLayoutPolicy->addItem (themeContainer, x, y);

        /*
         * FIXME: This should be some pre-select so we can show the dialog.
         */
        connect (themeContainer, SIGNAL(activated(ThemeDescriptor *)),
                this, SLOT(themeActivated(ThemeDescriptor *)));
        ++n;
    }
#endif
}
#if 0
void 
WallpaperWidget::themeActivated (
        ThemeDescriptor *themeDescr)
{
    ThemeDialog *dialog;
    SYS_DEBUG ("Theme '%s' activated", SYS_STR(themeDescr->name()));

    dialog = new ThemeDialog (m_ThemeBusinessLogic, themeDescr);
    dialog->showDialog ();
}
#endif
