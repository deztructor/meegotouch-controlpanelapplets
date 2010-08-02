/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "themelistmodel.h"
#include "themedescriptor.h"

#include <QModelIndex>

#define DEBUG
#include "../debug.h"

ThemeListModel::ThemeListModel (
        ThemeBusinessLogic *logic,
        QObject            *parent)
    : QAbstractTableModel (parent),
    m_ThemeBusinessLogic (logic)
{
    /*
     * If we have a businesslogic we can read the available themes.
     */
    if (logic) {
        QList<ThemeDescriptor *> list;
        list = logic->availableThemes ();

        if (list.size() > 0) {
            beginInsertRows (QModelIndex(), 0, list.size() - 1);
            m_ThemeDescList = list;
            endInsertRows ();
        }

        /*
         *
         */
        connect (logic, SIGNAL(themeAboutToBeRemoved(int)),
                    this, SLOT(themeAboutToBeRemoved(int)));
        connect (logic, SIGNAL(themeRemoved(QList<ThemeDescriptor *>)),
                this, SLOT(themeRemoved(QList<ThemeDescriptor *>)));
    }
}

/*
 * We call this function when the whole model needs to be refreshed on the
 * screen, for example when the highlight is changed because the user typed a
 * search string.
 */
void
ThemeListModel::refresh()
{
    if (m_ThemeDescList.size() != 0) {
        QModelIndex first = index (0, 0);
        QModelIndex last  = index (m_ThemeDescList.size() - 1, 0);

        SYS_DEBUG ("Emitting dataChanged()");
        emit dataChanged (first, last);
    }
}

int
ThemeListModel::rowCount(
        const QModelIndex &parent) const
{
    int retval = m_ThemeDescList.size();

    if (parent.isValid())
        retval = 0;

    //SYS_DEBUG ("Returning %d", retval);
    return retval;
}

int 
ThemeListModel::columnCount (
        const QModelIndex & parent) const
{
    int retval = 2;

    if (parent.isValid())
        retval = 0;

    //SYS_DEBUG ("Returning %d", retval);
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
    if (index.row() < 0 || index.row() >= m_ThemeDescList.size())
        return QVariant();

    ThemeDescriptor *desc = m_ThemeDescList[index.row()];
   
    switch (role) {
        case Qt::DisplayRole:
            return QVariant (desc->name());

        case ThemeListModel::SearchRole:
            return QVariant (desc->name());

        case ThemeListModel::CodeNameRole:
            return QVariant(desc->codeName());

        case ThemeListModel::NameRole:
            return QVariant(desc->name());
        
        case ThemeListModel::IconNameRole:
            return QVariant(desc->iconName());
            
        case ThemeListModel::ChangingNameRole:
            return QVariant (m_ChangingTheme);

        default:
            SYS_WARNING ("Unhandled role: %d", role);
            return QVariant();
    }
}

 
QModelIndex
ThemeListModel::indexOfCodeName(
        const QString &codeName) const 
{
    int i = 0;

    foreach (ThemeDescriptor *desc, m_ThemeDescList) {
        if (desc->codeName() == codeName) {
            return index (i, 0);
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
    QModelIndex index = indexOfCodeName (themeCodeName);
    
    SYS_DEBUG ("*** themeCodeName = %s", SYS_STR(themeCodeName));
    m_ChangingTheme = themeCodeName;

    if (index.isValid())
        emit dataChanged (index, index);
}

void
ThemeListModel::themeChanged (
		QString themeCodeName)
{
    QModelIndex index = indexOfCodeName (themeCodeName);

    SYS_DEBUG ("***********************************************************");
    SYS_DEBUG ("*** themeCodeName   = %s", SYS_STR(themeCodeName));
    SYS_DEBUG ("*** index.isValid() = %s", SYS_BOOL(index.isValid()));
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

void
ThemeListModel::themeAboutToBeRemoved (
        int index)
{
    SYS_DEBUG ("");
    beginRemoveRows (QModelIndex(), index, index);
}
 
void
ThemeListModel::themeRemoved (
        QList<ThemeDescriptor *> list)
{
    SYS_DEBUG ("");
    m_ThemeDescList = list;
    endRemoveRows ();
}


