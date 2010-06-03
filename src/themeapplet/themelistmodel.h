#ifndef THEMELISTMODEL_H__
#define THEMELISTMODEL_H__

#include <QStringList>
#include <QAbstractListModel>

class ThemeDescriptor;

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
        QList<QStringList> m_Rows;
};

#endif
