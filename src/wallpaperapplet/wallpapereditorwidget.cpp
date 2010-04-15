/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapereditorwidget.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <MButton>
#include <MImageWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>

#include <MAction>
#include <MApplicationWindow>
#include <MApplication>
#include <MApplicationPage>

#define DEBUG
#include "../debug.h"

WallpaperEditorWidget::WallpaperEditorWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic),
    m_Image (0),
    m_DoneAction (0)
{
    createWidgets ();
    createActions ();
}

WallpaperEditorWidget::~WallpaperEditorWidget ()
{
}

void
WallpaperEditorWidget::createWidgets ()
{
    this->setContentsMargins(0, 0, 0, 0);

    m_Image = new MImageWidget;
    m_WallpaperBusinessLogic->editedImage()->loadImage();
    m_Image->setImage (m_WallpaperBusinessLogic->editedImage()->image());

#if 0
    QGraphicsLinearLayout *mainLayout;
    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->setContentsMargins (0, 0, 0, 0);
    mainLayout->addItem (m_Image);
#else
    QGraphicsGridLayout *mainLayout = new QGraphicsGridLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addItem(m_Image, 0, 0);
#endif

    this->setLayout (mainLayout);
}

/*
 * In this function we create the actions (currently only the 'done' action that
 * is shown in the toolbar. Unfortunatelly the 'done' action can not be added to
 * a widget (it is not shown in the toolbar if added to a widget I guess because
 * the widget is not added to the window yet), and it also can not be added to
 * the current page (the new page for this widget is not created yet), so we
 * have to get the application window and add the action to that. 
 */
void
WallpaperEditorWidget::createActions ()
{
    MApplicationWindow *window = 
        MApplication::instance()->activeApplicationWindow();

    m_DoneAction = new MAction(
            "icon-m-framework-done",
            //% "Done"
            qtTrId("qtn_wall_done"), 
            this);
    m_DoneAction->setLocation(MAction::ToolBarLocation);
    window->addAction(m_DoneAction);
    m_DoneAction->setVisible(true);

    connect(m_DoneAction, SIGNAL(triggered()), 
            this, SLOT(slotDoneActivated()));
}

/*
 * This slot is called when the user activates the 'done' action and so we have
 * to store the settings.
 */
void
WallpaperEditorWidget::slotDoneActivated ()
{
    SYS_DEBUG ("");
    m_WallpaperBusinessLogic->setBackground();
    changeWidget (0);
}
