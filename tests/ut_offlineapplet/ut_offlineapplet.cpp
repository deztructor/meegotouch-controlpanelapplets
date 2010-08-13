/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include "ut_offlineapplet.h"
#include <DcpWidgetTypes>
#include <DcpWidget>
#include <MMessageBox>
#include <MDialog>
#include <MApplication>
#include <MBanner>

#include "offlineapplet.h"
#include "offlinebrief.h"
#include "qmdevicemode_stub.h"
#include "signalchecker.h"

#define DEBUG
#include "../../src/debug.h"

static const char *signalValuesChanged = SIGNAL(valuesChanged());

int   argc = 1;
char *argv[] = {
    (char *) "./ut_volumeoverlay",
    NULL };
/******************************************************************************
 * Stub for MMessageBox
 */
static QString mmessageBoxText;
static int mmessageBoxApereance;
MMessageBox::MMessageBox(const QString &title,
                const QString          &text,
                M::StandardButtons      buttons)
{
    mmessageBoxText = text;
}

int MDialog::result () const
{
    return MDialog::Accepted;
}

void MDialog::appear (MSceneWindow::DeletionPolicy policy)
{
    mmessageBoxApereance = true;
}

/******************************************************************************
 * Stub for MBanner
 */
static QString mbannerSubtitle;
static bool mbannerAppereance;

void
MBanner::setSubtitle (const QString &text)
{
    mbannerSubtitle = text;
}

void
MSceneWindow::appear (MSceneWindow::DeletionPolicy policy)
{
    mbannerAppereance = true;
}

/******************************************************************************
 * Ut_offlineApplet implementation. 
 */
void 
Ut_OfflineApplet::init()
{
    mmessageBoxText = "";
    mmessageBoxApereance = false;
    mbannerSubtitle = "";
    mbannerAppereance = false;
}

void 
Ut_OfflineApplet::cleanup()
{
}

void 
Ut_OfflineApplet::initTestCase()
{
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

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
Ut_OfflineApplet::testBriefConstruct ()
{
    DcpBrief* widget;

    gQmDeviceModeStub->stubReset ();

    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Normal);
    widget = m_Applet->constructBrief(1);

    QVERIFY (widget);
    QCOMPARE (int(widget->widgetTypeID()), int(DcpWidgetType::Button));
    QVERIFY(dynamic_cast<OfflineBrief*> (widget));
    delete widget;
}

void 
Ut_OfflineApplet::testCurrentText ()
{
    OfflineBrief brief;

    brief.m_LastMode = Maemo::QmDeviceMode::Normal;
    QCOMPARE (brief.currentText(), qtTrId("qtn_offl_activate"));

    brief.m_LastMode = Maemo::QmDeviceMode::Flight;
    QCOMPARE (brief.currentText(), qtTrId("qtn_offl_deactivate"));

    brief.m_LastMode = Maemo::QmDeviceMode::Error;
    QVERIFY (brief.currentText().isEmpty());
}


void 
Ut_OfflineApplet::testBriefInit ()
{
    OfflineBrief *brief;

    gQmDeviceModeStub->stubReset ();

    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Normal);

    brief = new OfflineBrief();
    QVERIFY (brief);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_activate"));
    delete brief;

    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Flight);
    brief = new OfflineBrief();
    QVERIFY (brief);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_deactivate"));
    delete brief;
}

void 
Ut_OfflineApplet::testBriefValueText ()
{    
    OfflineBrief *brief;

    gQmDeviceModeStub->stubReset ();
    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Normal);

    brief = new OfflineBrief();
    QVERIFY (brief);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_activate"));

    SignalChecker m_sChecker(brief);
    m_sChecker.addSignalChecker(signalValuesChanged);

    m_sChecker.increaseSigCounter(signalValuesChanged);
    brief->devModeChanged(Maemo::QmDeviceMode::Flight);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_deactivate"));
    m_sChecker.check();

    m_sChecker.increaseSigCounter(signalValuesChanged);
    brief->devModeChanged(Maemo::QmDeviceMode::Normal);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_activate"));
    delete brief;
}

void 
Ut_OfflineApplet::testBriefSetToggle ()
{
    OfflineBrief *brief;

    gQmDeviceModeStub->stubReset ();
    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Normal);

    brief = new OfflineBrief();
    QVERIFY (brief);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_activate"));

    SignalChecker m_sChecker(brief);
    m_sChecker.addSignalChecker(signalValuesChanged);

    // This should not change the text
    brief->setToggle(true);
    QVERIFY (mbannerAppereance);
    QCOMPARE (mbannerSubtitle, qtTrId("qtn_offl_entering"));

    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_activate"));
    QCOMPARE (gQmDeviceModeStub->stubCallCount("setMode"), 1);
    QCOMPARE (gQmDeviceModeStub->stubLastParameters<Maemo::QmDeviceMode::DeviceMode> (0), Maemo::QmDeviceMode::Flight);

    m_sChecker.increaseSigCounter(signalValuesChanged);
    brief->devModeChanged(Maemo::QmDeviceMode::Flight);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_deactivate"));
    m_sChecker.check();


    // This should not change the text nor the QmDeviceMode
    brief->setToggle(true);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_deactivate"));
    QCOMPARE (gQmDeviceModeStub->stubCallCount("setMode"), 1);
    QCOMPARE (gQmDeviceModeStub->stubLastParameters<Maemo::QmDeviceMode::DeviceMode> (0), Maemo::QmDeviceMode::Flight);

    delete brief;
}

void
Ut_OfflineApplet::testProcessDialogResult()
{
    OfflineBrief *brief;

    gQmDeviceModeStub->stubReset ();
    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::DeviceMode> (
        "getMode", Maemo::QmDeviceMode::Flight);

    brief = new OfflineBrief();
    QVERIFY (brief);
    QCOMPARE (brief->valueText(), qtTrId("qtn_offl_deactivate"));

    SignalChecker m_sChecker(brief);
    m_sChecker.addSignalChecker(signalValuesChanged);

    brief->setToggle(true);
    QCOMPARE(mmessageBoxText, qtTrId("qtn_offl_exiting"));
    QVERIFY(mmessageBoxApereance);
    brief->processDialogResult();
    QCOMPARE (gQmDeviceModeStub->stubCallCount("setMode"), 1);
    QCOMPARE (gQmDeviceModeStub->stubLastParameters<Maemo::QmDeviceMode::DeviceMode> (0), Maemo::QmDeviceMode::Normal);

    delete brief;
}

QTEST_APPLESS_MAIN(Ut_OfflineApplet)


