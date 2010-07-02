/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef QMBATTERY_H
#define QMBATTERY_H
#define QMBATTERY_STUBBED_H

#ifndef UNIT_TEST
#  error "This header should be used only in unit tests."
#endif
#ifdef FUNCTIONAL_TEST
#  error "This header should not be used in a functional test."
#endif

#include <QObject>

namespace Maemo
{
    class QmBattery : public QObject {
        Q_OBJECT
        Q_ENUMS(ChargerType)
        Q_ENUMS(BatteryState)
        Q_ENUMS(ChargingState)
        Q_ENUMS(RemainingTimeMode)

        public:
            QmBattery (QObject *parent = 0);
            ~QmBattery ();


        //! The type of charger connected
        enum ChargerType {
            Unknown = -1,        //!< Unknown charger
            None,                //!< No  charger connected
            Wall,                //!< Wall charger
            USB_500mA,           //!< USB with 500mA output
            USB_100mA            //!< USB with 100mA output
        };

        enum BatteryState {
            StateEmpty = 0,  //!< Battery empty (system being shut down)
            StateLow,        //!< Battery low (warnings issued to the user)
            StateOK,         //!< Battery OK
            StateFull,       //!< Battery full
            StateError       //!< Battery state not known
        };

        enum ChargingState {
            StateNotCharging = 0, //!< Not charging
            StateCharging,        //!< Charging
            StateChargingFailed   //!< Charging error, e.g. unsupported charger
        };

        enum RemainingTimeMode {
            NormalMode,       //!< In normal mode
            PowersaveMode     //!< In powersave mode
        };

        int getRemainingCapacityPct() const;
        ChargingState getChargingState() const;
        int getRemainingIdleTime(RemainingTimeMode mode) const;
        BatteryState getBatteryState() const;

        int getRemainingTalkTime(RemainingTimeMode mode) const;
        ChargerType getChargerType() const;

        int getMaxBars() const;

    signals:
        void batteryStateChanged (Maemo::QmBattery::BatteryState batteryState);
        void chargerEvent (Maemo::QmBattery::ChargerType chargerType);
        void chargingStateChanged (Maemo::QmBattery::ChargingState chargingState);
        void batteryRemainingCapacityChanged (int percentage, int bars);

    public: // Low level helper functions that are not in the original.
        void connectCharger (ChargerType chargerType);
        void modifyBatteryState (BatteryState BatteryState, int percentage);
        void setChargingState (ChargingState newState);

        
        QString ChargerName (ChargerType chargerType) const;
        QString BatteryStateName (BatteryState  batteryState) const;
        QString ChargingStateName (ChargingState state) const;

    private:
        ChargerType   m_ChargerType;
        BatteryState  m_BatteryState;
        ChargingState m_ChargingState;
        int           m_Percentage;
    };
}
#endif
