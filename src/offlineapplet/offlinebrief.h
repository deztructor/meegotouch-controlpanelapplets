#ifndef OFFLINEBRIEF_H
#define OFFLINEBRIEF_H

#include <DcpBrief>
#include <qmdevicemode.h>

class OfflineBrief: public DcpBrief{
    Q_OBJECT
public:
    OfflineBrief();
    ~OfflineBrief();
    virtual QString valueText() const;

    virtual void setToggle (bool toggle);
    virtual int widgetTypeID() const;

private:
    QString currentText() const;

private slots:
    void devModeChanged(Maemo::QmDeviceMode::DeviceMode mode);
private:
    Maemo::QmDeviceMode::DeviceMode m_LastMode;
    Maemo::QmDeviceMode *m_DevMode;
#ifdef UNIT_TEST
    friend class Ut_OfflineApplet;
#endif

};


#endif // OFFLINEBRIEF
