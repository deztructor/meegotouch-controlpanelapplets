#include <QString>
#include <QModelIndex>
#include <QVariant>
#include <MContentItem>
#include "themecellcreator.h"
#include "themelistmodel.h"

MWidget *
ThemeCellCreator::createCell(const QModelIndex &index, 
                             MWidgetRecycler &recycler) const
{
    MContentItem *cell =
        qobject_cast<MContentItem *>(recycler.take(MContentItem::staticMetaObject.className()));
    if (!cell) {
        cell = new MContentItem(MContentItem::IconAndSingleTextLabel);
    }
    updateCell(index, cell);
    return cell;
}

void
ThemeCellCreator::updateCell(const QModelIndex &index, 
                             MWidget *cell) const
{
    MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
    QVariant data = index.data(Qt::DisplayRole);
    QString codeName = data.value<QString>();
    QStringList rowData = data.value<QStringList>();
    contentItem->setTitle(rowData[ThemeColumnName]);

    const QPixmap *pixmap;
    pixmap = MTheme::pixmapCopy(rowData[ThemeColumnIcon]);
    contentItem->setPixmap (*pixmap);
}

