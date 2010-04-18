/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themelistcontainer.h"
#include "themebusinesslogic.h"
#include "themecontentitem.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>

#include "../debug.h"

static const int MaxColumns = 2;

ThemeListContainer::ThemeListContainer (
        QGraphicsItem *parent) :
    MContainer (parent)
{
    m_Columns = 
        MApplication::activeApplicationWindow()->orientation() == M::Landscape ? 
        MaxColumns : 1;
    
    createLayout ();
    connect(MApplication::activeApplicationWindow(),
            SIGNAL(orientationChanged(M::Orientation)),
            this, SLOT(orientationChanged(M::Orientation)));
}


ThemeContentItem *
ThemeListContainer::addThemeDescriptor (
        ThemeBusinessLogic  *themeBusinessLogic,
        ThemeDescriptor     *theme)
{
    ThemeContentItem *contentitem;
    int               n, x, y;
    
    /*
     *
     */
    contentitem = new ThemeContentItem (theme, themeBusinessLogic);
    m_ItemList.append (contentitem);
    
    /*
     * Calculating the place of the new item and adding it to the policies.
     */
    n = m_ItemList.size() - 1;
    x = n % MaxColumns;
    y = n / MaxColumns;

    m_GridLayoutPolicy->addItem (contentitem, y, x);
    m_LinearLayoutPolicy->addItem (contentitem);

    return contentitem;
}

void 
ThemeListContainer::createLayout ()
{
    MLayout *layout = new MLayout();
    
    m_GridLayoutPolicy = new MGridLayoutPolicy (layout);
    m_GridLayoutPolicy->setColumnStretchFactor (0, 1);
    m_GridLayoutPolicy->setColumnStretchFactor (1, 1);

    m_LinearLayoutPolicy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    layout->setLandscapePolicy (m_GridLayoutPolicy);
    layout->setPortraitPolicy (m_LinearLayoutPolicy);

    centralWidget()->setLayout (layout);
}

MContentItem::ContentItemMode 
ThemeListContainer::calculateItemMode (
        int length, 
        int x, 
        int y)
{
    bool singleRowMode = length <= m_Columns;
    bool inFirstColumn = x == 0;
    bool inLastColumn  = (x + 1) >= m_Columns;
    bool firstRow      = y == 0; 
    bool lastRow       = (x + y * m_Columns) >= (length - m_Columns);
    bool singleColumn  = m_Columns == 1;

    SYS_DEBUG ("**********************");
    SYS_DEBUG ("*** m_Columns       = %d", m_Columns);
    SYS_DEBUG ("*** length          = %d", length);
    SYS_DEBUG ("*** x               = %d", x);
    SYS_DEBUG ("*** y               = %d", y);
    SYS_DEBUG ("*** inLastColumn    = %s", inLastColumn ? "yes" : "no");
    SYS_DEBUG ("*** inFirstColumn   = %s", inFirstColumn ? "yes" : "no");
    SYS_DEBUG ("*** lastRow         = %s", lastRow ? "yes" : "no");
    
    if (singleColumn) {
        if (y == 0)
            return MContentItem::SingleColumnTop;
        else if (y + 1 == length)
            return MContentItem::SingleColumnBottom;
        else 
            return MContentItem::SingleColumnCenter;
    }

    if (singleRowMode) {
        if (inFirstColumn) {
            return MContentItem::SingleRowLeft;
        } else if (inLastColumn) {
            return MContentItem::SingleRowRight;
        } else {
            return MContentItem::SingleColumnCenter;
        }
    }

    if (firstRow) {
        if (inFirstColumn) {
            return MContentItem::TopLeft;
        } else if (inLastColumn) {
            return MContentItem::TopRight;
        } else {
            return MContentItem::Top;
        }
    }
    
    if (lastRow) {
        if (inFirstColumn) {
            return MContentItem::BottomLeft;
        } else if (inLastColumn) {
            return MContentItem::BottomRight;
        } else {
            return MContentItem::Bottom;
        }
    }

    return MContentItem::Default;
}

void 
ThemeListContainer::recalculateItemModes ()
{
    MContentItem::ContentItemMode itemMode;
    int    n, nitems;
    int    x, y;


    nitems = m_ItemList.size(); 
    for (n = 0; n < nitems; ++n) {
        x = n % m_Columns;
        y = n / m_Columns;
        
        itemMode = calculateItemMode (nitems, x, y);
        m_ItemList[n]->setItemMode (itemMode);
    }
}

void
ThemeListContainer::orientationChanged (
        M::Orientation orientation)
{
    SYS_DEBUG ("");
    
    switch (orientation) {
        case M::Portrait:
            SYS_DEBUG ("Turned to portrait");
            m_Columns = 1;
            recalculateItemModes ();
            break;

        case M::Landscape:
            SYS_DEBUG ("Turned to landscape");
            m_Columns = MaxColumns;
            recalculateItemModes ();
            break;
    }
}

