/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <QString>

#include <QModelIndex>
#include <MSortFilterProxyModel>

#include <QVariant>
#include <MContentItem>
#include <MAdvancedListItem>
#include <MImageWidget>
#include <MProgressIndicator>
#include <QGraphicsLayout>

#include "themecellcreator.h"
#include "themelistmodel.h"

#define DEBUG
#include "../debug.h"


MWidget *
ThemeCellCreator::createCell(
        const QModelIndex &index, 
        MWidgetRecycler &recycler) const
{
    MAdvancedListItem *cell;
    
    // Title is just for debugging purposes.
    QString title;
    title = index.data (ThemeListModel::NameRole).toString();
    SYS_DEBUG ("************** %d ***************", index.row());
    SYS_DEBUG ("index.isValid() = %s", SYS_BOOL(index.isValid()));
    SYS_DEBUG ("title           = %s", SYS_STR(title));
    
    cell = qobject_cast <MAdvancedListItem *> (
            recycler.take(MAdvancedListItem::staticMetaObject.className()));

    if (!cell) {
        cell = new MAdvancedListItem (
            MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons);
        cell->setLayoutPosition(M::CenterPosition);
        cell->progressIndicator()->setViewType (
                MProgressIndicator::spinnerType);
        cell->progressIndicator()->setUnknownDuration (true);
    }

    updateCell(index, cell);

    return cell;
}

void
ThemeCellCreator::updateCell (
        const QModelIndex &index, 
        MWidget *cell) const
{
    MAdvancedListItem    *contentItem;
    QString               title;
    QString               codeName;
    QString               iconName;
    QString               changingTheme;

    if(!cell || !index.isValid()) 
        return;

    contentItem = qobject_cast<MAdvancedListItem *>(cell);
    
    changingTheme = index.data (ThemeListModel::ChangingNameRole).toString();
    title = index.data (ThemeListModel::NameRole).toString();
    codeName = index.data (ThemeListModel::CodeNameRole).toString();
    iconName = index.data (ThemeListModel::IconNameRole).toString();

    #if 1
    SYS_DEBUG ("************** %d ***************", index.row());
    SYS_DEBUG ("title         = %s", SYS_STR(title));
    SYS_DEBUG ("changingTheme = %s", SYS_STR(changingTheme));
    SYS_DEBUG ("codeName      = %s", SYS_STR(codeName));
    SYS_DEBUG ("iconName      = %s", SYS_STR(iconName));
    #endif

    // The title
    if (m_HighlightText.isEmpty()) {
        contentItem->setTitle(title);
    } else {
        int matchingIndex = title.indexOf (
                m_HighlightText, 0, Qt::CaseInsensitive);

        if (matchingIndex != -1) {
            title.insert (matchingIndex + m_HighlightText.length(), "</b>");
            title.insert (matchingIndex, "<b>");
        }
        contentItem->setTitle (title);
    }

    // The icon
    if (contentItem->imageWidget()->image() != iconName)
        contentItem->imageWidget()->setImage (iconName);

    // The spinner.
    if (!codeName.isEmpty() && changingTheme == codeName) {
        contentItem->progressIndicator()->show();
    } else {
        contentItem->progressIndicator()->hide();
    }

    updateContentItemMode(index, contentItem);
}

void 
ThemeCellCreator::updateContentItemMode (
              const QModelIndex &index, 
              MAdvancedListItem *contentItem) const
{
    #if 0
    /*
     * FIXME: It seems that there is no way to set the visual appearance for the
     * MAdvancedListItem items.
     */
    int row = index.row();
    int rows = index.model()->rowCount();

    SYS_DEBUG ("");
    if (row == 0)
        contentItem->setItemMode(MContentItem::SingleColumnTop);
    else if (row < rows - 1)
        contentItem->setItemMode(MContentItem::SingleColumnCenter);
    else 
        contentItem->setItemMode(MContentItem::SingleColumnBottom);
    #endif
}

void 
ThemeCellCreator::highlightByText (
        QString text)
{
    SYS_DEBUG ("*** text = %s", SYS_STR(text));
    m_HighlightText = text;
}

