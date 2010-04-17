/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themewidget.h"
#include "themedescriptor.h"
#include "themecontentitem.h"
#include "themedialog.h"

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MContainer>

#define DEBUG
#include "../debug.h"

static const int MaxColumns = 2;

ThemeWidget::ThemeWidget (
        ThemeBusinessLogic *themeBusinessLogic, 
        QGraphicsWidget    *parent) :
    DcpWidget (parent),
    m_ThemeBusinessLogic (themeBusinessLogic)
{
    createWidgets ();
    retranslateUi ();
    readLocalThemes ();
}

ThemeWidget::~ThemeWidget ()
{
}

void
ThemeWidget::createWidgets ()
{
    QGraphicsLinearLayout *mainLayout;

    //MLayout *layout = new MLayout();
    
    m_LocalContainer = createContainer (ThemeWidget::ThemeLocal);
    m_OviContainer = createContainer (ThemeWidget::ThemeOvi);

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->addItem (m_LocalContainer);
    mainLayout->addItem (m_OviContainer);

    this->setLayout (mainLayout);
}

MContainer * 
ThemeWidget::createContainer (
        ThemeWidget::ThemeCategoryId category)
{
    MGridLayoutPolicy *policy;
    MLayout *layout = new MLayout();
    
    policy = new MGridLayoutPolicy (layout);
    policy->setColumnStretchFactor (0, 1);
    policy->setColumnStretchFactor (1, 1);
    layout->setPolicy (policy);
    
    MContainer *container = new MContainer ();
    container->centralWidget()->setLayout (layout);

    switch (category) {
        case ThemeWidget::ThemeLocal:
            m_LocalLayoutPolicy = policy;
            break;

        case ThemeWidget::ThemeOvi:
            m_OviLayoutPolicy = policy;
    }

    return container;
}

void
ThemeWidget::retranslateUi ()
{
    if (m_LocalContainer) {
        /*
         * FIXME: This is not an official locale id.
         */
        //% "Available themes"
        m_LocalContainer->setTitle (qtTrId ("qtn_themes_locale"));
    }

    if (m_OviContainer) {
        /*
         * FIXME: This is not an official locale id.
         */
        //% "Ovi"
        m_OviContainer->setTitle (qtTrId ("qtn_themes_ovi"));
    }
}

void
ThemeWidget::readLocalThemes ()
{
    Q_ASSERT (m_ThemeBusinessLogic != 0);
    Q_ASSERT (m_LocalLayoutPolicy != 0);

    QList<ThemeDescriptor *> themeList = 
        m_ThemeBusinessLogic->availableThemes ();
    int n = 0;
    foreach (ThemeDescriptor *theme, themeList) {
        //ThemeContainer *themeContainer;
        int x = n / MaxColumns;
        int y = n % MaxColumns;

        if (!theme->isVisible()) {
            delete theme;
            continue;
        }

        ThemeContentItem *contentitem;

        contentitem = new ThemeContentItem (theme, m_ThemeBusinessLogic);
        connect (contentitem, SIGNAL(activated(ThemeDescriptor *)),
                this, SLOT(themeActivated(ThemeDescriptor *)));
        m_LocalLayoutPolicy->addItem (contentitem, x, y);

        ++n;
    }
}

void 
ThemeWidget::themeActivated (
        ThemeDescriptor *themeDescr)
{
    ThemeDialog *dialog;
    SYS_DEBUG ("Theme '%s' activated", SYS_STR(themeDescr->name()));

    dialog = new ThemeDialog (m_ThemeBusinessLogic, themeDescr);
    dialog->showDialog ();
}

