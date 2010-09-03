/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMELISTMODEL_H__
#define THEMELISTMODEL_H__

#include <QList>
#include <QStringList>
#include <QString>
#include <QAbstractTableModel>

#include "themebusinesslogic.h"
#include "themedescriptor.h"

class ThemeDescriptor;
class QModelIndex;

class ThemeListModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        ThemeListModel(
                ThemeBusinessLogic *businessLogic = 0,
                QObject            *parent = 0);

        void refresh ();

        int rowCount (const QModelIndex &parent = QModelIndex()) const;
        int columnCount (const QModelIndex & parent) const;

        QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
        QModelIndex indexOfCodeName(const QString &codeName) const;
        QString changingTheme () const;

        typedef enum {
            SearchRole  = Qt::UserRole + 1,
            CodeNameRole,
            NameRole,
            IconNameRole,
            ChangingNameRole,
        } Roles;

    public slots:
	    void themeChangeStarted (QString themeCodeName);
    	void themeChanged (QString themeCodeName);
        void themeAboutToBeRemoved (int index);
        void themeRemoved (QList<ThemeDescriptor *> list);
        void themeAboutToBeAdded (int index);
        void themeAdded (QList<ThemeDescriptor *> list);

    private:
        ThemeBusinessLogic  *m_ThemeBusinessLogic;
        // FIXME: This should be in the businesslogic.
        QString              m_ChangingTheme;
        QList<ThemeDescriptor *>      m_ThemeDescList;
};

#endif
