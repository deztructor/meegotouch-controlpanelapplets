#include "volumebar.h"
#include "volumeoverlay.h"
#include "volumebarlogic.h"

#define DEBUG
#include "../debug.h"

#include <MStatusIndicatorMenuPluginInterface>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <MSlider>
#include <MLabel>
#include <MLayout>
#include <MLocale>
#include <MTheme>

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

    quint32 current_volume = m_logic->getVolume ();
    if (current_volume > 0)
        m_bar->setMinLabelIconID ("icon-m-common-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-common-volume-off");

    m_bar->setRange (0, (int) m_logic->getMaxVolume () - 1);
    m_bar->setValue (current_volume);

    connect (m_bar, SIGNAL (valueChanged (int)),
             this, SLOT (sliderChanged (int)));

    connect (m_logic, SIGNAL (volumeChanged (quint32, quint32)),
             this, SLOT (volumeChanged (quint32, quint32)));

    connect (m_overlay, SIGNAL (VolumeChanged (int)),
             this, SLOT (overlayChanged (int)));

    m_hwkeys = new QmKeys (this);

    connect (m_hwkeys,
             SIGNAL (keyEvent (QmKeys::Key, QmKeys::State)),
             this,
             SLOT (hwKeyEvent (QmKeys::Key, QmKeys::State)));
}

VolumeBar::~VolumeBar ()
{
    //Free the resources here
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
VolumeBar::volumeChanged (quint32 val, quint32 max)
{
    SYS_DEBUG ("val = %d, max = %d", val, max);

    m_bar->blockSignals (true); // to avoid busy loop

    m_bar->setValue (0);
    m_bar->setRange (0, (int) max - 1);
    m_bar->setValue ((int) val);

    m_overlay->UpdateVolume ((int) val, (int) max);

    m_bar->blockSignals (false);
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

void
VolumeBar::hwKeyEvent (QmKeys::Key key, QmKeys::State state)
{
    SYS_DEBUG ("key = %d, state = %d");
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

