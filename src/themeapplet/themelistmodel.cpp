#include "themelistmodel.h"
#include "themedescriptor.h"

#define DEBUG
#include "../debug.h"

ThemeListModel::ThemeListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int
ThemeListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_Rows.size();
}

/*!
 * FIXME: It is unclear why do we decided to return an unintialized QVariant
 * when the role is not Qt::DisplayRole!
 */
QVariant
ThemeListModel::data (
		const QModelIndex &index, 
		int                role) const
{    
    if (role == Qt::DisplayRole) {
        QStringList row = m_Rows[index.row()];
        return QVariant(row);
    }

    return QVariant();
}

void
ThemeListModel::setThemeList(const QList<ThemeDescriptor *> &themeList)
{
    m_Rows.clear();
    foreach (ThemeDescriptor *descr, themeList) {
        QStringList row;
        // respect ThemeColumnIndex order
        for (int i = 0; i < ThemeColumnLast; ++i) {
            row << QString();
        }
        row[ThemeColumnCodeName] =  descr->codeName();
        row[ThemeColumnName] =  descr->name();
        row[ThemeColumnIcon] = descr->iconName();
        m_Rows.append(row);
    }
}
 
QModelIndex
ThemeListModel::indexOfCodeName(const QString &codeName) const {
    int i = 0;
    foreach (QStringList row, m_Rows) {
        if (row[ThemeColumnCodeName] == codeName) {
            return index(i, 0);
        }
        ++i;
    }

    SYS_WARNING("code name not found in list model: %s", SYS_STR(codeName));
    return QModelIndex();
}



