/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themecontainer.h"
#include "themedescriptor.h"

#include <QString>
#include <DuiLabel>
#include <DuiImageWidget>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <QGraphicsSceneMouseEvent>

#define DEBUG
#include "../debug.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET_NO_CREATE(ThemeContainer)


ThemeContainer::ThemeContainer (
        ThemeDescriptor *descr,
        DuiWidget       *parent) :
    DuiWidget (parent),
    m_ThemeDescriptor (descr)
{
    createWidgets ();
}

ThemeContainer::~ThemeContainer ()
{
}

void
ThemeContainer::createWidgets ()
{
    DuiLayout        *layout;
    DuiLinearLayoutPolicy  *layoutPolicy;

    layout = new DuiLayout (this);

    layoutPolicy = new DuiLinearLayoutPolicy (layout, Qt::Horizontal);
    layout->setPolicy (layoutPolicy);
    /*
     * An icon representing the theme.
     */
    m_Icon = new DuiImageWidget (m_ThemeDescriptor->iconName());
    m_Icon->setObjectName ("ThemeIcon");
    layoutPolicy->addItem (m_Icon, Qt::AlignLeft | Qt::AlignVCenter);

    /*
     * A label with the name of the theme. 
     */
    m_NameLabel = new DuiLabel (m_ThemeDescriptor->name());
    m_NameLabel->setObjectName ("ThemeNameLabel");
    layoutPolicy->addItem (m_NameLabel, Qt::AlignRight | Qt::AlignVCenter);

    this->setLayout (layout);
}


void 
ThemeContainer::mousePressEvent (
        QGraphicsSceneMouseEvent *event)
{
    //SYS_DEBUG ("");
    Q_UNUSED(event);
    // this function is only for becoming the mouse grabber item
    // so that release event is working
    //
    // visual feedback for user can be implemented here
}

void 
ThemeContainer::mouseReleaseEvent (
        QGraphicsSceneMouseEvent *event)
{
    //SYS_DEBUG ("");

    if (QRectF(QPointF(), size()).contains(event->pos())) {
        SYS_DEBUG ("Emitting activated('%s')", 
                SYS_STR(m_ThemeDescriptor->codeName()));
        emit activated(m_ThemeDescriptor->codeName());
    }
}
