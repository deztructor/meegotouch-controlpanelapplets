#include "volumebar.h"
#include "volumeoverlay.h"
#include "volumebarlogic.h"

#define DEBUG
#include "../debug.h"

#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiLinearLayoutPolicy>
#include <DuiContainer>
#include <DuiSlider>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiTheme>

#include <QGraphicsLinearLayout>

#define SYSTEMUI_TRANSLATION "systemui-applets"

VolumeBar::VolumeBar (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
                      QGraphicsItem *parent) :
        DuiWidget (parent),
        m_bar (0),
        m_logic (0),
        m_overlay (new VolumeOverlay (parent))
{
    Q_UNUSED(statusIndicatorMenu)

    m_logic = new VolumeBarLogic;

    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);

    setLayout (mainLayout);
    mainLayout->setContentsMargins (0, 0, 0, 0);

    DuiContainer *container = new DuiContainer;

    container->setHeaderVisible (false);

    DuiLayout *layout = new DuiLayout;
    DuiLinearLayoutPolicy *hbox =
        new DuiLinearLayoutPolicy (layout, Qt::Horizontal);
    container->setLayout (layout);

    //% "Volume"
    DuiLabel *label = new DuiLabel (qtTrId ("qtn_volume"));

    m_bar = new DuiSlider;
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
}

VolumeBar::~VolumeBar ()
{
    //Free the resources here
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
    // Set the volume value / slider icon
    m_logic->setVolume ((quint32) val);
    if (val > 0)
        m_bar->setMinLabelIconID ("icon-m-common-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-common-volume-off");

    // And then update the slider
    m_bar->blockSignals (true);
    m_bar->setValue (val);
    m_bar->blockSignals (false);
}

