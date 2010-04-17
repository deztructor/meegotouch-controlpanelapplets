/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "themecontentitem.h"
#include "themedescriptor.h"
#include "themebusinesslogic.h"

#include <MImageWidget>
#include <QString>

#define DEBUG
#include "../debug.h"

ThemeContentItem::ThemeContentItem (
        ThemeDescriptor     *descr,
        ThemeBusinessLogic  *logic,
        MWidget             *parent) :
    MContentItem (MContentItem::IconAndTwoTextLabels, parent),
    m_ThemeDescriptor (descr),
    m_ThemeBusinessLogic (logic)
{
    if (m_ThemeBusinessLogic) 
        connect (m_ThemeBusinessLogic, SIGNAL(themeChanged(QString)),
                this, SLOT(themeChanged(QString)));

    /*
     * This is where we refresh the widget from the theme descriptor.
     */
    setTitle (m_ThemeDescriptor->name());
    if (current())
        setSubtitle ("Current theme");

    /*
     * This is how we set an icon in the contentitem so that oit will not crash
     * on a theme change.
     */
    const QPixmap *pixmap;
    pixmap = MTheme::pixmapCopy (m_ThemeDescriptor->iconName());
    setPixmap (*pixmap);

    connect (this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ThemeContentItem::~ThemeContentItem ()
{
}

void
ThemeContentItem::slotClicked()
{
    if (current())
        return;

    SYS_DEBUG ("Emitting activated('%s')", 
                SYS_STR(m_ThemeDescriptor->codeName()));
    emit activated(m_ThemeDescriptor);
}

bool
ThemeContentItem::current ()
{
    bool    thisIsCurrent = false;

    if (m_ThemeBusinessLogic) {
        QString current = m_ThemeBusinessLogic->currentThemeCodeName();
        thisIsCurrent = *m_ThemeDescriptor == current;
    }

    return thisIsCurrent;
}

void
ThemeContentItem::themeChanged (QString themeCodeName)
{
    bool thisIsCurrent = *m_ThemeDescriptor == themeCodeName;

    SYS_DEBUG ("*** thisIsCurrent = %s", SYS_BOOL(thisIsCurrent));
    
    if (thisIsCurrent)
        setSubtitle ("Current theme");
    else
        setSubtitle ("");
}


