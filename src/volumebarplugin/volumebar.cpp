#include "volumebar.h"
#include "volumeoverlay.h"
#include "volumebarlogic.h"

#undef DEBUG
#include "../debug.h"

#include <MStatusIndicatorMenuPluginInterface>
#include <MApplication>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <MSlider>
#include <MLabel>
#include <MLayout>
#include <MLocale>
#include <MTheme>
#include <QTimer>

#include <QGraphicsLinearLayout>

#define SYSTEMUI_TRANSLATION "systemui-applets"

VolumeBar::VolumeBar (MStatusIndicatorMenuInterface &statusIndicatorMenu,
                      QGraphicsItem *parent) :
        MWidget (parent),
        m_bar (0),
        m_logic (0),
        m_overlay (0),
        m_hwkeys (0)
{
    SYS_DEBUG ("");
    Q_UNUSED(statusIndicatorMenu)

    m_logic = new VolumeBarLogic;
    m_overlay = new VolumeOverlay;

    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);

    setLayout (mainLayout);
    mainLayout->setContentsMargins (0, 0, 0, 0);

    MContainer *container = new MContainer;

    container->setHeaderVisible (false);

    MLayout *layout = new MLayout;
    MLinearLayoutPolicy *hbox =
        new MLinearLayoutPolicy (layout, Qt::Horizontal);
    container->setLayout (layout);

    //% "Volume"
    MLabel *label = new MLabel (qtTrId ("qtn_volume"));

    m_bar = new MSlider;
    m_bar->setMinLabelVisible (true);

    hbox->addItem (label, Qt::AlignLeft);
    hbox->addItem (m_bar, Qt::AlignRight);

    mainLayout->addItem (container);

    connect (m_bar, SIGNAL (valueChanged (int)),
             this, SLOT (sliderChanged (int)));

    connect (m_logic, SIGNAL (volumeChanged (quint32, quint32, bool)),
             this, SLOT (volumeChanged (quint32, quint32, bool)));

    connect (m_overlay, SIGNAL (VolumeChanged (int)),
             this, SLOT (overlayChanged (int)));

    m_hwkeys = new QmKeys (this);

    m_hwkeyResource = new ResourceSet ("event");
    m_hwkeyResource->setAlwaysReply ();

    ScaleButtonResource *volumeKeys = new ScaleButtonResource;

    m_hwkeyResource->addResourceObject (volumeKeys);

    connect (m_hwkeyResource, SIGNAL (resourcesGranted (QList<ResourcePolicy::ResourceType>)),
             this, SLOT (hwKeyResourceAcquired ()));
    connect (m_hwkeyResource, SIGNAL (lostResources ()),
             this, SLOT (hwKeyResourceLost ()));

    m_hwkeyResource->acquire ();

#ifdef TEST_OVERLAY
    QTimer::singleShot (2000, this, SLOT (testOverlay ()));
#endif
}

VolumeBar::~VolumeBar ()
{
    //Free the resources here
    m_hwkeyResource->deleteResource (ScaleButtonType);

    delete m_hwkeys;
    m_hwkeys = 0;

    delete m_logic;
    m_logic = 0;

    delete m_overlay;
    m_overlay = 0;
}

void
VolumeBar::sliderChanged (int val)
{
    m_logic->setVolume ((quint32) val);

    if (val > 0)
        m_bar->setMinLabelIconID ("icon-m-common-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-common-volume-off");

#if defined (i386) && defined (DEBUG)
    m_overlay->UpdateVolume (val, (int) m_logic->getMaxVolume ());
#endif
}

void
VolumeBar::volumeChanged (quint32 val, quint32 max, bool init)
{
    SYS_DEBUG ("val = %d, max = %d (, init = %s)", val, max, SYS_BOOL (init));

    m_bar->blockSignals (true); // to avoid busy loop

    m_bar->setValue (0);
    m_bar->setRange (0, (int) max - 1);
    m_bar->setValue ((int) val);

    m_bar->blockSignals (false);

    if (val > 0)
        m_bar->setMinLabelIconID ("icon-m-common-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-common-volume-off");

    // Do not show volume overlay on volume-change events,
    // it is enough to show on hw-key-press events...
#if 0
    if (init == false)
        m_overlay->UpdateVolume ((int) val, (int) max);
#endif
}

void
VolumeBar::overlayChanged (int val)
{
    // Set the volume value
    m_logic->setVolume ((quint32) val);

    // And then update the slider and its icon
    m_bar->blockSignals (true);
    m_bar->setValue (val);
    m_bar->blockSignals (false);

    if (val > 0)
        m_bar->setMinLabelIconID ("icon-m-common-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-common-volume-off");
}

#ifdef TEST_OVERLAY
void
VolumeBar::testOverlay ()
{
    hwKeyEvent (QmKeys::VolumeUp, QmKeys::KeyDown);
    QTimer::singleShot (5000, this, SLOT (testOverlay ()));
}
#endif

void
VolumeBar::hwKeyEvent (QmKeys::Key key, QmKeys::State state)
{
    int change_val = 0;

    if (state == QmKeys::KeyUp)
        return;

    switch (key)
    {
        // TODO: check the orientation stuff from UI specs.
        case QmKeys::VolumeUp:
            change_val++;
            break;
        case QmKeys::VolumeDown:
            change_val--;
            break;
        default:
            // no-op for other hw-keys...
            return;
            break;
    }

    if (MApplication::instance ()->activeWindow ()
        ->orientation () == M::Landscape)
    {
        change_val *= -1;
    }

    int current_volume = (int) m_logic->getVolume ();
    int max_volume = (int) m_logic->getMaxVolume ();

    current_volume += change_val;

    if (current_volume < 0)
        current_volume = 0;
    else if (current_volume >= max_volume)
        current_volume = max_volume - 1;

    // This sets the volume and update the slider ...
    overlayChanged (current_volume);
    // ... and show the overlay
    m_overlay->UpdateVolume (current_volume, max_volume);
}

void
VolumeBar::hwKeyResourceAcquired ()
{
    SYS_DEBUG ("");

    // Disconnect from everything first
    m_hwkeys->disconnect ();

    connect (m_hwkeys,
             SIGNAL (keyEvent (QmKeys::Key, QmKeys::State)),
             this,
             SLOT (hwKeyEvent (QmKeys::Key, QmKeys::State)));
}

void
VolumeBar::hwKeyResourceLost ()
{
    SYS_DEBUG ("");
    m_hwkeys->disconnect ();
}

