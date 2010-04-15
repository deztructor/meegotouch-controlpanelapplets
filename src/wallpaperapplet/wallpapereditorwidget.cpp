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

#define DEBUG
#include "../debug.h"

WallpaperEditorWidget::WallpaperEditorWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic),
    m_Image (0),
    m_DoneButton (0)
{
    createWidgets ();
    retranslateUi ();
}

WallpaperEditorWidget::~WallpaperEditorWidget ()
{
}

void
WallpaperEditorWidget::createWidgets ()
{
    QGraphicsLinearLayout *mainLayout;

    m_Image = new DuiImageWidget;
    //m_WallpaperBusinessLogic->editedImage()->loadImage();
    m_Image->setImage (m_WallpaperBusinessLogic->editedImage()->image());

    //% "Done"
    m_DoneButton = new DuiButton (qtTrId("qtn_wall_done"));
    connect (m_DoneButton, SIGNAL(clicked()),
            this, SLOT(slotDoneButtonClicked()));

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->addItem (m_Image);
    mainLayout->addItem (m_DoneButton);

    this->setLayout (mainLayout);
}


void
WallpaperEditorWidget::retranslateUi ()
{
}

void
WallpaperEditorWidget::slotDoneButtonClicked ()
{
    SYS_DEBUG ("");
    m_WallpaperBusinessLogic->setBackground();
    changeWidget (0);
}
