#include "ut_volumebarlogic.h"
#include <volumebarlogic.h>

#define DEBUG
#include "../../src/debug.h"

void
Ut_VolumeBarLogic::init ()
{
}

void
Ut_VolumeBarLogic::cleanup ()
{
}

void
Ut_VolumeBarLogic::initTestCase ()
{
    m_Api = new VolumeBarLogic;
    m_Api->stepsUpdated (30, 100);
}

void
Ut_VolumeBarLogic::cleanupTestCase ()
{
    delete m_Api;
}

// Check if setting / getting works correctly
void
Ut_VolumeBarLogic::testVolumeSetGet ()
{
    quint32 val = 5;

    m_Api->setVolume (val);

    Q_VERIFY (m_Api->getVolume () == val)  ;
}

void
Ut_VolumeBarLogic::testVolumeChangeByPa ()
{
    quint32 currentstep = 10;
    quint32 stepcount   = 20;

    // A D-bus message calls this slot [inside the logic],
    // now we call this outside :
    m_Api->stepsUpdated (currentstep, stepcount);

    // Check the current ...
    Q_VERIFY (m_Api->getVolume () == currentstep);
    // .. and the maximal values
    Q_VERIFY (m_Api->getMaxVolume () == stepcount);
}

void
Ut_VolumeBarLogic::testSignaling ()
{
    quint32 currentstep = 0;
    quint32 stepcount   = 13;

    // Whe PulseAudio sends the D-Bus signal about stepcount/currentstep
    // changes logic should emit itself volumeChanged signal
    QSignalSpy spy (m_Api, SIGNAL (volumeChanged (quint32, quint32)));

    // Do what PulseAudio do [of course Pa doing this indirectly...]
    m_Api->stepsUpdated (currentstep, stepcount);

    qWait (500); // wait for a little time

    QList<QVariant> arguments = spy.takeFirst ();
    // Verify the signal parameters
    QVERIFY (arguments.at (0).toUInt () == currentstep);
    QVERIFY (arguments.at (1).toUInt () == stepcount);
}

QTEST_MAIN(Ut_VolumeBarLogic)
