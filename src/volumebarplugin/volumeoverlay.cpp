#include "volumeoverlay.h"
#include <DuiSceneWindow>
#include <DuiLabel>
#include <DuiSlider>

#define DEBUG
#include "../debug.h"

// FIXME: TBD
#define HIDE_TIMEOUT 1000

VolumeOverlay::VolumeOverlay (QGraphicsItem *parent) :
    DuiOverlay (parent),
    m_timer (0)
{
    m_timer = new QTimer;
    m_timer->setInterval (HIDE_TIMEOUT);
    connect (m_timer, SIGNAL (timeout ()),
             this, SLOT (hideMe ()));

    setObjectName ("VolumeOverlay");
    setManagedManually (true);

    setWidget (new DuiSlider);
}

VolumeOverlay::~VolumeOverlay ()
{
    delete m_timer;
    m_timer = 0;
}

void
VolumeOverlay::UpdateVolume (int percentage)
{
    SYS_DEBUG ("percentage = %d", percentage);

    Q_UNUSED (percentage);

    m_timer->stop ();

    // TODO: update the contents with the new percentage value...

    appear (DuiSceneWindow::KeepWhenDone);
    m_timer->start ();
}

void
VolumeOverlay::hideMe ()
{
    SYS_DEBUG ("");

    m_timer->stop ();
    disappear ();
}


