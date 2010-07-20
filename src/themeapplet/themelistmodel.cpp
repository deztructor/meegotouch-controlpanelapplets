/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "themelistmodel.h"
#include "themedescriptor.h"

#include <QModelIndex>

//#define DEBUG
#include "../debug.h"

ThemeListModel::ThemeListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void
ThemeListModel::refresh()
{
    if (m_Rows.size() != 0) {
        QModelIndex first = index (0, 0);
        QModelIndex last  = index (m_Rows.size() - 1, 0);

        SYS_DEBUG ("Emitting dataChanged()");
        emit dataChanged (first, last);
    }
}

int
ThemeListModel::rowCount(
        const QModelIndex &parent) const
{
    int retval = m_Rows.size();

    if (parent.isValid())
        retval = 0;

    SYS_DEBUG ("Returning %d", retval);
    return retval;
}

int 
ThemeListModel::columnCount (
        const QModelIndex & parent) const
{
    int retval = 2;

    if (parent.isValid())
        retval = 0;

    SYS_DEBUG ("Returning %d", retval);
    return retval;
}
        
#if 0
        /*
         * FIXME:
         * This method is only for debugging purposes and will be removed soon.
         */
QModelIndex
ThemeListModel::index (
                int row, 
                int column, 
                const QModelIndex &parent) const
{
    QModelIndex retval;

    retval = QAbstractTableModel::index (row, column, parent);

    SYS_DEBUG ("*** row              = %d", row);
    SYS_DEBUG ("*** column           = %d", column);
    SYS_DEBUG ("*** parent.isValid() = %s", SYS_BOOL(parent.isValid()));
    SYS_DEBUG ("*** retval.isValid() = %s", SYS_BOOL(retval.isValid()));
    return retval;
}
#endif

/*!
 * Please note: In this function we return a value based on the role parameter
 * and we are not using the column number from the index. Well, it works...
 */
QVariant
ThemeListModel::data (
		const QModelIndex &index, 
		int                role) const
{    
    if (index.row() < 0 || index.row() >= m_Rows.size())
        return QVariant();

    QStringList row = m_Rows[index.row()];
   
    switch (role) {
        case Qt::DisplayRole:
            return QVariant (row[ThemeColumnName]);

        case ThemeListModel::SearchRole:
            return QVariant (row[ThemeColumnName]);

        case ThemeListModel::CodeNameRole:
            return QVariant(row[ThemeColumnCodeName]);

        case ThemeListModel::NameRole:
            return QVariant(row[ThemeColumnName]);
        
        case ThemeListModel::IconNameRole:
            return QVariant(row[ThemeColumnIcon]);
            
        case ThemeListModel::ChangingNameRole:
            return QVariant (m_ChangingTheme);

        default:
            SYS_WARNING ("Unhandled role: %d", role);
            SYS_DEBUG ("Returning list of %d items.", row.size());
            return QVariant(row);
    }
}

/*!
 * FIXME: This should not be forced from outside, this should be initiated from
 * inside!
 */
void
ThemeListModel::setThemeList (
        const QList<ThemeDescriptor *> &themeList)
{
    if (m_Rows.size() != 0) {
        beginRemoveColumns (QModelIndex(), 0, m_Rows.size() - 1);
        m_Rows.clear();
        endRemoveColumns ();
    }

    if (themeList.size() == 0)
        return;

    SYS_DEBUG ("Inserting elements 0--%d", themeList.size() - 1);
    beginInsertRows (QModelIndex(), 0, themeList.size() - 1);

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

    endInsertRows ();
}
 
QModelIndex
ThemeListModel::indexOfCodeName(
        const QString &codeName) const 
{
    int i = 0;

    SYS_DEBUG ("*** rows = %d", m_Rows.size());
    foreach (QStringList row, m_Rows) {
        SYS_DEBUG ("%s == %s is %s",
                SYS_STR (row[ThemeColumnCodeName]),
                SYS_STR (codeName),
                SYS_BOOL(row[ThemeColumnCodeName] == codeName));
        if (row[ThemeColumnCodeName] == codeName) {
            return index(i, 0);
        }
        ++i;
    }

    SYS_WARNING("code name not found in list model: %s", SYS_STR(codeName));
    return QModelIndex();
}

void 
ThemeListModel::themeChangeStarted (
		QString themeCodeName)
{
    QModelIndex index = indexByThemeCodeName(themeCodeName);
    
    SYS_DEBUG ("*** themeCodeName = %s", SYS_STR(themeCodeName));
    m_ChangingTheme = themeCodeName;

    if (index.isValid())
        emit dataChanged (index, index);
}

void
ThemeListModel::themeChanged (
		QString themeCodeName)
{
    QModelIndex index = indexByThemeCodeName(themeCodeName);

    SYS_DEBUG ("*** themeCodeName = %s", SYS_STR(themeCodeName));
    m_ChangingTheme = "";

    if (index.isValid())
        emit dataChanged (index, index);
}

/*!
 * \returns The name of the theme that is about to become the current theme. 
 *
 * In the UI this theme should have the spinner shown.
 */
QString 
ThemeListModel::changingTheme () const
{
    return m_ChangingTheme;
}

QModelIndex  
ThemeListModel::indexByThemeCodeName (
        QString themeCodeName)
{
    QModelIndex retval;
    
    for (int n = 0; n < m_Rows.size(); ++n) {
        if (m_Rows[n][ThemeColumnCodeName] == themeCodeName)
            retval = index (n, 0);
    }

    if (!retval.isValid()) {
        SYS_WARNING ("Theme code name %s not found.", SYS_STR(themeCodeName));
    }

    return retval;
}

