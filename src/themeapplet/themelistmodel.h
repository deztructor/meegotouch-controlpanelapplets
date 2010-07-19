/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMELISTMODEL_H__
#define THEMELISTMODEL_H__

#include <QStringList>
#include <QString>
#include <QAbstractListModel>

class ThemeDescriptor;

class ThemeListModel : public QAbstractListModel
{
    Q_OBJECT

    public:
        ThemeListModel(QObject *parent = 0);

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
        void setThemeList(const QList<ThemeDescriptor *> &themeList);
        QModelIndex indexOfCodeName(const QString &codeName) const;
        QString changingTheme () const;

        typedef enum {
            ThemeColumnCodeName = 0,
            ThemeColumnName,
            ThemeColumnIcon,
            ThemeColumnLast
        } ThemeColumnIndex;
        
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

    private:
        /*
         * FIXME: This model should hold theme descriptors instead of just
         * strings. This is not robust enough!
         */
        QModelIndex  indexByThemeCodeName (QString themeCodeName);
        QString            m_ChangingTheme;
        QList<QStringList> m_Rows;
};

#endif
