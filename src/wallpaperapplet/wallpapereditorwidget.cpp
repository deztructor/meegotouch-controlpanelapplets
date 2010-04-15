/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapereditorwidget.h"

#include <QGraphicsLinearLayout>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiContainer>

#define DEBUG
#include "../debug.h"

WallpaperEditorWidget::WallpaperEditorWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic)
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
#if 0
    QGraphicsLinearLayout *mainLayout;

    m_LocalContainer = createContainer (WallpaperWidget::ThemeLocal);
    m_OviContainer = createContainer (WallpaperWidget::ThemeOvi);

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->addItem (m_LocalContainer);
    mainLayout->addItem (m_OviContainer);

    this->setLayout (mainLayout);
#endif
}


void
WallpaperEditorWidget::retranslateUi ()
{
}

