/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themecontainer.h"
#include "themedescriptor.h"
#include "themebusinesslogic.h"

#include <QString>
#include <MLabel>
#include <MImageWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <QGraphicsSceneMouseEvent>

//#define DEBUG
#include "../debug.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(ThemeContainer)


ThemeContainer::ThemeContainer (
        ThemeDescriptor     *descr,
        ThemeBusinessLogic  *logic,
        MWidget           *parent) :
    MWidget (parent),
    m_ThemeDescriptor (descr),
    m_ThemeBusinessLogic (logic)
{
    createWidgets ();
    setActive (!current());

    if (m_ThemeBusinessLogic) 
        connect (m_ThemeBusinessLogic, SIGNAL(themeChanged(QString)),
                this, SLOT(themeChanged(QString)));
}

ThemeContainer::~ThemeContainer ()
{
}

void
ThemeContainer::createWidgets ()
{
    MLayout              *layout;
    MLinearLayoutPolicy  *layoutPolicy;
         
    layout = new MLayout (this);

    layoutPolicy = new MLinearLayoutPolicy (layout, Qt::Horizontal);
    layout->setPolicy (layoutPolicy);
    /*
     * An icon representing the theme.
     */
    m_Icon = new MImageWidget (m_ThemeDescriptor->iconName());
    m_Icon->setObjectName ("ThemeIcon");
    m_Icon->setActive (!current());
    layoutPolicy->addItem (m_Icon, Qt::AlignLeft | Qt::AlignVCenter);
    layoutPolicy->setStretchFactor (m_Icon, 0);

    /*
     * A label with the name of the theme. 
     */
    m_NameLabel = new MLabel (m_ThemeDescriptor->name());
    m_NameLabel->setObjectName ("ThemeNameLabel");
    m_NameLabel->setActive (!current());
    layoutPolicy->addItem (m_NameLabel, Qt::AlignRight | Qt::AlignVCenter);
    layoutPolicy->setStretchFactor (m_NameLabel, 1);

    this->setLayout (layout);
}

bool
ThemeContainer::current ()
{
    bool    thisIsCurrent = false;

    if (m_ThemeBusinessLogic) {
        QString current = m_ThemeBusinessLogic->currentThemeCodeName();
        thisIsCurrent = *m_ThemeDescriptor == current;
    }

    return thisIsCurrent;
}

void
ThemeContainer::themeChanged (QString themeCodeName)
{
    bool thisIsCurrent = *m_ThemeDescriptor == themeCodeName;

    SYS_DEBUG ("*** thisIsCurrent = %s", SYS_BOOL(thisIsCurrent));
    setActive (!thisIsCurrent);
    if (m_Icon)
        m_Icon->setActive (!thisIsCurrent);
    if (m_NameLabel)
        m_NameLabel->setActive (!thisIsCurrent);
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

    if (QRectF(QPointF(), size()).contains(event->pos()) &&
            !current()) {
        SYS_DEBUG ("Emitting activated('%s')", 
                SYS_STR(m_ThemeDescriptor->codeName()));
        emit activated(m_ThemeDescriptor);
    }
}
