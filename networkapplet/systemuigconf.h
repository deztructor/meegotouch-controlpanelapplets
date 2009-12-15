#ifndef SYSTEMUIGCONF_H
#define SYSTEMUIGCONF_H

#include <QHash>
#include <QObject>
#include <QVariant>

class DuiGConfItem;

class SystemUIGConf : public QObject
{
    Q_OBJECT
public:

    enum GConfKeyGroup {
        Network = 0
    };

    enum GConfKey {        
        NetworkToggle = 0,
        NetworkRoamingState,
        NetworkRoamingUpdatesState
    };   

    SystemUIGConf(QObject* parent = 0);
    virtual ~SystemUIGConf();

    void setValue(SystemUIGConf::GConfKey key, QVariant value);
    QVariant value(SystemUIGConf::GConfKey, QVariant def = NULL);
    int keyCount(SystemUIGConf::GConfKeyGroup keyGroup);    

signals:
    void valueChanged(SystemUIGConf::GConfKey key, QVariant value);

private slots:
    void keyValueChanged();

private: //methods
    QString mapGConfKeyGroup(SystemUIGConf::GConfKeyGroup keyGroup);
    QString mapGConfKey(SystemUIGConf::GConfKey key);

private: //attributes
    QHash<SystemUIGConf::GConfKey, DuiGConfItem *> duiGConfItems;

};


#endif // SYSTEMGCONF_H
