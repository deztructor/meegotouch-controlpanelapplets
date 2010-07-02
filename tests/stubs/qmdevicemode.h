/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef QMDEVICEMODE_H
#define QMDEVICEMODE_H
#define QMDEVICEMODE_STUBBED_H

#ifndef UNIT_TEST
#  error "This header should be used only in unit tests."
#endif
#ifdef FUNCTIONAL_TEST
#  error "This header should not be used in a functional test."
#endif

#include <QObject>

namespace Maemo
{
    class QmDeviceMode : public QObject {
        Q_OBJECT
        Q_ENUMS(PSMState);

        public:
            QmDeviceMode (QObject *parent = 0);
            ~QmDeviceMode ();

            enum PSMState {
                PSMError = -1,    //!< Power save mode could not be figured out
                PSMStateOff = 0,  //!< Power save mode is off
                PSMStateOn        //!< Power save mode is on
            };

            PSMState getPSMState () const;
            bool setPSMState (PSMState state);
        
            int getPSMBatteryMode ();
            bool setPSMBatteryMode (int percentages);
            
        signals:
            void devicePSMStateChanged(Maemo::QmDeviceMode::PSMState state);
            
        public: // helper functions that are not in the original
            QString PSMStateName (PSMState state) const;

        private:
            PSMState  m_PSMSTate;
            int       m_PSMBatteryMode;
    };
}
#endif
