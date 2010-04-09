#include "volumeoverlay.h"
#include <DuiSceneWindow>
#include <DuiImageWidget>
#include <DuiSlider>
#include <QTimer>

#undef DEBUG
#include "../debug.h"

// FIXME: TBD
#define HIDE_TIMEOUT 1500

VolumeOverlay::VolumeOverlay (QGraphicsItem *parent) :
    DuiOverlay (parent),
    m_timer (0),
    m_slider (0)
{
    m_timer = new QTimer;
    m_timer->setInterval (HIDE_TIMEOUT);
    connect (m_timer, SIGNAL (timeout ()),
             this, SLOT (hideMe ()));

    setObjectName ("VolumeOverlay");

    constructUi ();
}

VolumeOverlay::~VolumeOverlay ()
{
    delete m_timer;
    m_timer = 0;
}

void
VolumeOverlay::constructUi ()
{
    m_slider = new DuiSlider;

    m_slider->setOrientation (Qt::Vertical);
    m_slider->setMaxLabelIconID (QString ("icon-m-common-volume"));

    connect (m_slider, SIGNAL (valueChanged (int)),
             this, SIGNAL (VolumeChanged (int)));

    connect (m_slider, SIGNAL (valueChanged (int)),
             this, SLOT (updateSliderIcon (int)));

    // Stop the timeout when the slider is in pressed state ...
    connect (m_slider, SIGNAL (sliderPressed ()),
             m_timer, SLOT (stop ()));
    // ...a ndre-start it when slider released.
    connect (m_slider, SIGNAL (sliderReleased ()),
             m_timer, SLOT (start ()));

    setWidget (m_slider);
}

void
VolumeOverlay::UpdateVolume (int val, int max)
{
    m_timer->stop ();

    updateSliderIcon (val);

    m_slider->blockSignals (true);

    m_slider->setValue (0);
    m_slider->setRange (0, max - 1);
    m_slider->setValue (val);

    m_slider->blockSignals (false);

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

void
VolumeOverlay::updateSliderIcon (int val)
{
    // update the icon-id
    if (val == 0)
        m_slider->setMaxLabelIconID (QString ("icon-m-common-volume-off"));
    else
        m_slider->setMaxLabelIconID (QString ("icon-m-common-volume"));
}

