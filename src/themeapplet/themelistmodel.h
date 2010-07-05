#ifndef THEMELISTMODEL_H__
#define THEMELISTMODEL_H__

#include <QStringList>
#include <QAbstractListModel>

class ThemeDescriptor;

/*
 * FIXME: This swhould be inside the class namespace.
 */
enum ThemeColumnIndex {
    ThemeColumnCodeName = 0,
    ThemeColumnName,
    ThemeColumnIcon,
    ThemeColumnLast
};


class ThemeListModel : public QAbstractListModel
{
    Q_OBJECT

    public:
        ThemeListModel(QObject *parent = 0);

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
        void setThemeList(const QList<ThemeDescriptor *> &themeList);
        QModelIndex indexOfCodeName(const QString &codeName) const;

    private:
	/*
	 * FIXME: This model should hold theme descriptors instead of just
	 * strings. This is not robust enough!
	 */
        QList<QStringList> m_Rows;
};

#endif
