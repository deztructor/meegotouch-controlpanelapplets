/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "systemuigconf.h"

#include <DuiGConfItem>
#include <QDebug>

//#define DEBUG
#include "../debug.h"

SystemUIGConf::SystemUIGConf (
		QObject* parent) :
	QObject(parent)
{
    // init the gconf keys
    duiGConfItems.insert (SystemUIGConf::BatteryPSMAutoKey, 
            new DuiGConfItem (mapGConfKey(SystemUIGConf::BatteryPSMAutoKey)));

    duiGConfItems.insert (SystemUIGConf::BatteryPSMThresholdKey, 
            new DuiGConfItem (
                mapGConfKey(SystemUIGConf::BatteryPSMThresholdKey)));
    
    duiGConfItems.insert (SystemUIGConf::LedAllEnabled, 
            new DuiGConfItem (
                mapGConfKey(SystemUIGConf::LedAllEnabled)));
    
    duiGConfItems.insert (SystemUIGConf::LedEventsEnabled, 
            new DuiGConfItem (
                mapGConfKey(SystemUIGConf::LedEventsEnabled)));
    
    duiGConfItems.insert (SystemUIGConf::LedIlluminationEnabled, 
            new DuiGConfItem (
                mapGConfKey(SystemUIGConf::LedIlluminationEnabled)));

    QHash<SystemUIGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i)
        connect (i.value (), SIGNAL (valueChanged()), 
                this, SLOT (keyValueChanged()));

}

SystemUIGConf::~SystemUIGConf()
{
    QHash<SystemUIGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i) {
        delete i.value();
        i.value() = NULL;
    }
}


int
SystemUIGConf::keyCount (
        SystemUIGConf::GConfKeyGroup keyGroup)
{
    DuiGConfItem duiGConfItem(mapGConfKeyGroup(keyGroup));
    QList<QString> list = duiGConfItem.listEntries();

    return list.size();
}

/*!
 * Sets a configuration item value by its numerical id.
 */
void
SystemUIGConf::setValue (
        SystemUIGConf::GConfKey key, 
        QVariant                value)
{
    duiGConfItems.value(key)->set (value);
}

/*!
 * Returns a configuration item value by its numerical ID and default value.
 */
QVariant 
SystemUIGConf::value (
        SystemUIGConf::GConfKey key, 
        QVariant                def)
{
    return duiGConfItems.value(key)->value (def);
}

/*!
 * Callback function that is called when the value of some GConf item has been
 * changed. 
 */
void 
SystemUIGConf::keyValueChanged ()
{
    DuiGConfItem *item = static_cast<DuiGConfItem *>(this->sender());
    emit valueChanged(duiGConfItems.key(item), item->value());
}

/*!
 * Maps a numerical configuration group id to a gconf key.
 */
QString
SystemUIGConf::mapGConfKeyGroup (
        SystemUIGConf::GConfKeyGroup keyGroup)
{
    QString keyGroupStr;

    switch (keyGroup) {
        case SystemUIGConf::Battery:
            keyGroupStr = "/systemui/settings/battery";
            break;

        case SystemUIGConf::Led:
            keyGroupStr = "/systemui/settings/leds";
            break;

        default:
            break;
    }

    return keyGroupStr;
}

/*!
 * Maps a numerical configuration id to a GConf key used to store the value.
 */
QString
SystemUIGConf::mapGConfKey (
        SystemUIGConf::GConfKey key)
{
    QString keyStr;

    switch (key) {
        case SystemUIGConf::BatteryPSMAutoKey:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Battery) +
                    "/batteryPSMAuto";
            break;

        case SystemUIGConf::BatteryPSMThresholdKey:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Battery) +
                    "/batteryPSMThreshold";
            break;

        case SystemUIGConf::LedAllEnabled:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/allLedsEnabled";
            break;
        
        case SystemUIGConf::LedEventsEnabled:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/eventsLedEnabled";
            break;
        
        case SystemUIGConf::LedIlluminationEnabled:
            keyStr = mapGConfKeyGroup(SystemUIGConf::Led) +
                    "/illuminationLedEnabled";
            break;

        default:
            break;
    }

    SYS_DEBUG ("Returning '%s'", SYS_STR(keyStr));
    return keyStr;
}

