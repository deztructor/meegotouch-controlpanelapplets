/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapereditorwidget.h"

#include <QGraphicsLinearLayout>
#include <DuiButton>
#include <DuiImageWidget>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

#include <DuiAction>
#include <DuiApplicationWindow>
#include <DuiApplication>
#include <DuiApplicationPage>

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
    QGraphicsLinearLayout *mainLayout;

    m_Image = new DuiImageWidget;
    m_WallpaperBusinessLogic->editedImage()->loadImage();
    m_Image->setImage (m_WallpaperBusinessLogic->editedImage()->image());

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->addItem (m_Image);


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
    DuiApplicationWindow *window = 
        DuiApplication::instance()->activeApplicationWindow();

    m_DoneAction = new DuiAction(
            "icon-m-framework-done",
            //% "Done"
            qtTrId("qtn_wall_done"), 
            this);
    m_DoneAction->setLocation(DuiAction::ToolBarLocation);
    window->addAction(m_DoneAction);
    m_DoneAction->setVisible(true);

    connect(m_DoneAction, SIGNAL(triggered()), 
            this, SLOT(slotDoneActivated()));
}


void
WallpaperEditorWidget::slotDoneActivated ()
{
    SYS_DEBUG ("");
    m_WallpaperBusinessLogic->setBackground();
    changeWidget (0);
}
