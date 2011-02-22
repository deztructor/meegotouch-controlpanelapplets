/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef DRILLDOWNITEM_H
#define DRILLDOWNITEM_H

#include <QObject>
#include <MBasicListItem>

class QGraphicsWidget;
class MLinearLayoutPolicy;

/*!
 * A simple list item widget that supports inverted theme and drill down icon at
 * the right hand side.
 */
class DrillDownItem : public MBasicListItem {
Q_OBJECT
    
    public:
        DrillDownItem (
                MBasicListItem::ItemStyle style = IconWithTitle, 
                QGraphicsItem             *parent = NULL);
    protected:
        virtual QGraphicsLayout *createLayout();
};

MLabel *
addTitleLabel (
        QGraphicsWidget     *parent,
        MLinearLayoutPolicy *targetPolicy,
        const char          *panelStyleName,
        const char          *labelStyleName);

MLabel *
addTitleLabel (
        QGraphicsWidget     *parent,
        MLinearLayoutPolicy *targetPolicy,
        const char          *labelStyleName);

#endif 
