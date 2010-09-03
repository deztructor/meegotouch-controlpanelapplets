/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <QString>

#include <QModelIndex>
#include <MSortFilterProxyModel>

#include <QVariant>
#include <MAdvancedListItem>
#include <MImageWidget>
#include <MProgressIndicator>
#include <QGraphicsLayout>

#include "themecellcreator.h"
#include "themelistmodel.h"

#define DEBUG
#include "../debug.h"

static const char *SelectionStartTag = "<font color='blue'>";
static const char *SelectionEndTag   = "</font>";

MWidget *
ThemeCellCreator::createCell(
        const QModelIndex &index, 
        MWidgetRecycler   &recycler) const
{
    MAdvancedListItem *cell;
    
    cell = qobject_cast <MAdvancedListItem *> (
            recycler.take(MAdvancedListItem::staticMetaObject.className()));

    if (!cell) {
        cell = new MAdvancedListItem (
            MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons);
        cell->progressIndicator()->setUnknownDuration (true);
        cell->sideTopImageWidget()->hide();
        cell->sideBottomImageWidget()->hide();
    }

    updateCell(index, cell);

    return cell;
}

void
ThemeCellCreator::updateCell (
        const QModelIndex &index, 
        MWidget           *cell) const
{
    MAdvancedListItem    *listItem;
    QString               title;
    QString               codeName;
    QString               iconName;
    QString               changingTheme;
    bool                  isChangingTheme;

    if(!cell || !index.isValid()) 
        return;

    listItem = qobject_cast<MAdvancedListItem *>(cell);
    
    changingTheme = index.data (ThemeListModel::ChangingNameRole).toString();
    title = index.data (ThemeListModel::NameRole).toString();
    codeName = index.data (ThemeListModel::CodeNameRole).toString();
    iconName = index.data (ThemeListModel::IconNameRole).toString();
    isChangingTheme = !codeName.isEmpty() && changingTheme == codeName;

    #if 1
    SYS_DEBUG ("************** %d ***************", index.row());
    SYS_DEBUG ("title           = %s", SYS_STR(title));
    SYS_DEBUG ("changingTheme   = %s", SYS_STR(changingTheme));
    SYS_DEBUG ("codeName        = %s", SYS_STR(codeName));
    SYS_DEBUG ("iconName        = %s", SYS_STR(iconName));
    SYS_DEBUG ("isChangingTheme = %s", SYS_BOOL(isChangingTheme));
    #endif

    // The title
    if (m_HighlightText.isEmpty()) {
        listItem->setTitle(title);
    } else {
        int matchingIndex = title.indexOf (
                m_HighlightText, 0, Qt::CaseInsensitive);

        if (matchingIndex != -1) {
            title.insert (matchingIndex + m_HighlightText.length(), 
                    SelectionEndTag);
            title.insert (matchingIndex, SelectionStartTag);
        }
        listItem->setTitle (title);
    }

    // The icon
    if (listItem->imageWidget()->image() != iconName)
        listItem->imageWidget()->setImage (iconName);

    // The spinner.
    if (isChangingTheme) {
        listItem->progressIndicator()->show();
    } else {
        listItem->progressIndicator()->hide();
    }

    updateListItemMode(index, listItem);
}

void 
ThemeCellCreator::updateListItemMode (
              const QModelIndex &index, 
              MAdvancedListItem *listItem) const
{
    int row = index.row();
    int rows = index.model()->rowCount();

    if (row == 0)
        listItem->setLayoutPosition (M::VerticalTopPosition);
    else if (row < rows - 1)
        listItem->setLayoutPosition (M::VerticalCenterPosition);
    else 
        listItem->setLayoutPosition (M::VerticalBottomPosition);
}

void 
ThemeCellCreator::highlightByText (
        QString text)
{
    m_HighlightText = text;
}

