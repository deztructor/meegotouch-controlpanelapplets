/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themewidget.h"
#include "themedescriptor.h"
#include "themelistcontainer.h"
#include "themecontentitem.h"
#include "themedialog.h"

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <MApplication>
#include <MApplicationWindow>

#include "../debug.h"

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

    m_LocalContainer = new ThemeListContainer();
    m_OviContainer = new ThemeListContainer();

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->addItem (m_LocalContainer);
    mainLayout->addItem (m_OviContainer);

    this->setLayout (mainLayout);
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
    QList<ThemeDescriptor *> themeList = m_ThemeBusinessLogic->availableThemes ();

    #if 0
    // Just for testing...
    QList<ThemeDescriptor *> themeList1 = m_ThemeBusinessLogic->availableThemes ();
    themeList << themeList1;
    #endif

    /*
     * Now we are putting the widgets in place.
     */
    foreach (ThemeDescriptor *theme, themeList) {
        ThemeContentItem *contentitem;
        
        if (!theme->isVisible()) {
            delete theme;
            continue;
        }

        contentitem = m_LocalContainer->addThemeDescriptor (
                m_ThemeBusinessLogic, theme);

        connect (contentitem, SIGNAL(activated(ThemeDescriptor *)),
                this, SLOT(themeActivated(ThemeDescriptor *)));
    }

    /*
     * Now that we added all the items we can calculate the visual appearance
     * for them.
     */
    m_LocalContainer->recalculateItemModes ();
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

