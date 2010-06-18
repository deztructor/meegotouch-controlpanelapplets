/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include <DcpWidgetTypes>
#include "ut_offlineapplet.h"
#include "offlineapplet.h"
#include "offlinebrief.h"
#include "qmdevicemode_stub.h"
#include "signalchecker.h"

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_offlineApplet implementation. 
 */
void 
Ut_OfflineApplet::init()
{
}

void 
Ut_OfflineApplet::cleanup()
{
}

void 
Ut_OfflineApplet::initTestCase()
{
    m_Applet = new OfflineApplet;
    
    m_Applet->init ();

}

void 
Ut_OfflineApplet::cleanupTestCase()
{
    m_Applet->deleteLater ();
}

void 
Ut_OfflineApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (title.isEmpty());
}

void 
Ut_OfflineApplet::testMenu ()
{
    QVector<MAction *> menu = m_Applet->viewMenuItems();
    QVERIFY (menu.isEmpty());
}
    
void 
Ut_OfflineApplet::testConstructWidget ()
{
    DcpWidget* widget = m_Applet->constructWidget(1);
    QVERIFY (!widget);
}

void 
Ut_OfflineApplet::testBrief ()
{    
    DcpBrief* widget;

    gQmDeviceModeStub->stubReset ();

    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Normal);
    widget = m_Applet->constructBrief(1);
    QVERIFY (widget);
    QCOMPARE (int(widget->widgetTypeID()), int(DcpWidgetType::Button));
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_activate"));
    delete widget;

    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Flight);
    widget = m_Applet->constructBrief(1);
    QVERIFY (widget);
    QCOMPARE (int(widget->widgetTypeID()), int(DcpWidgetType::Button));
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_deactivate"));

    OfflineBrief *brief = dynamic_cast<OfflineBrief*> (widget);

    QVERIFY (brief);
    SignalChecker m_sChecker(brief);
    m_sChecker.addSignalChecker(SIGNAL(valuesChanged()));

    m_sChecker.increaseSigCounter(SIGNAL(valuesChanged()));
    brief->devModeChanged(Maemo::QmDeviceMode::Flight);
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_deactivate"));
    m_sChecker.check();

    m_sChecker.increaseSigCounter(SIGNAL(valuesChanged()));
    brief->devModeChanged(Maemo::QmDeviceMode::Normal);
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_activate"));
}
void 
Ut_OfflineApplet::testSetToggle ()
{
    DcpBrief* widget;

    gQmDeviceModeStub->stubReset ();

    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Normal);
    widget = m_Applet->constructBrief(1);
    QVERIFY (widget);
    QCOMPARE (int(widget->widgetTypeID()), int(DcpWidgetType::Button));
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_activate"));

    OfflineBrief *brief = dynamic_cast<OfflineBrief*> (widget);

    QVERIFY (brief);
    SignalChecker m_sChecker(brief);
    m_sChecker.addSignalChecker(SIGNAL(valuesChanged()));

    // This should not change the text
    brief->setToggle(true);
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_activate"));
    QCOMPARE (gQmDeviceModeStub->stubCallCount("setMode"), 1);
    QCOMPARE (gQmDeviceModeStub->stubLastParameters<Maemo::QmDeviceMode::DeviceMode> (0), Maemo::QmDeviceMode::Flight); 

    m_sChecker.increaseSigCounter(SIGNAL(valuesChanged()));
    brief->devModeChanged(Maemo::QmDeviceMode::Flight);
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_deactivate"));
    m_sChecker.check();

    // This should not change the text
    brief->setToggle(true);
    QCOMPARE (widget->valueText(), qtTrId("qtn_offl_deactivate"));
    QCOMPARE (gQmDeviceModeStub->stubCallCount("setMode"), 2);
    QCOMPARE (gQmDeviceModeStub->stubLastParameters<Maemo::QmDeviceMode::DeviceMode> (0), Maemo::QmDeviceMode::Normal); 
}

QTEST_APPLESS_MAIN(Ut_OfflineApplet)


