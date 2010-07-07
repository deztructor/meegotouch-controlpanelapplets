/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <QString>
#include <QModelIndex>
#include <QVariant>
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
ThemeCellCreator::updateCell(
        const QModelIndex &index, 
        MWidget *cell) const
{
    MAdvancedListItem    *contentItem;
    QVariant              data;
    const ThemeListModel *model; 
    QStringList           rowData;
    QString               title;
    QString               codeName;
    QString               iconName;
    
    contentItem = qobject_cast<MAdvancedListItem *>(cell);
    data = index.data(Qt::DisplayRole);
    model = qobject_cast<const ThemeListModel *> (index.model());
    
    rowData = data.value<QStringList>();
    
    title = rowData[ThemeColumnName];
    codeName = rowData[ThemeColumnCodeName];
    iconName = rowData[ThemeColumnIcon];

    SYS_DEBUG ("title         = %s", SYS_STR(title));
    SYS_DEBUG ("changingTheme = %s", SYS_STR(model->changingTheme()));
    SYS_DEBUG ("codeName      = %s", SYS_STR(codeName));
    SYS_DEBUG ("iconName      = %s", SYS_STR(iconName));

    // The title
    contentItem->setTitle(title);

    // The icon
    if (contentItem->imageWidget()->image() != iconName)
        contentItem->imageWidget()->setImage (iconName);

    // The spinner.
    if (!codeName.isEmpty() && model->changingTheme() == codeName) {
        contentItem->progressIndicator()->show();
    } else {
        contentItem->progressIndicator()->hide();
    }

    SYS_DEBUG ("-------------------------------------------- END ---");
}

