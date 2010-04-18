/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMELISTCONTAINER_H
#define THEMELISTCONTAINER_H

#include <QList>
#include <MContainer>
#include <MContentItem>

class MGridLayoutPolicy;
class MLinearLayoutPolicy;
class ThemeDescriptor;
class ThemeBusinessLogic;
class ThemeContentItem;
class ThemeDescriptor;

/*!
 * A simple list-like widget to show a set of themes represented by
 * ThemeContentItem objects.
 */
class ThemeListContainer : public MContainer
{
    Q_OBJECT

public:
    ThemeListContainer (QGraphicsItem *parent = 0);

    ThemeContentItem *addThemeDescriptor (
            ThemeBusinessLogic  *themeBusinessLogic,
            ThemeDescriptor     *theme);
    void recalculateItemModes ();

public slots:
    void orientationChanged (M::Orientation orientation);

private:
    void createLayout ();
    MContentItem::ContentItemMode calculateItemMode (
        int length, 
        int x, 
        int y);

    int                     m_Columns;
    MGridLayoutPolicy      *m_GridLayoutPolicy;
    MLinearLayoutPolicy    *m_LinearLayoutPolicy;
    QList<MContentItem *>   m_ItemList;
};
#endif
