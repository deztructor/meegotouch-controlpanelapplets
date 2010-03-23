#include "volumebar.h"
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
        m_logic (0)
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

    connect (m_bar, SIGNAL (valueChanged (int)),
             this, SLOT (sliderChanged (int)));

    int current_volume = m_logic->getVolume ();
    if (current_volume > 0)
        m_bar->setMinLabelIconID ("icon-m-common-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-common-volume-off");

    m_bar->setRange (0, m_logic->getMaxVolume ());
    m_bar->setValue (current_volume);
}

VolumeBar::~VolumeBar ()
{
    //Free the resources here
    delete m_logic;
}

void
VolumeBar::sliderChanged (int val)
{
    m_logic->setVolume (val);

    if (val > 0)
        m_bar->setMinLabelIconID ("icon-m-common-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-common-volume-off");
}

void
VolumeBar::volumeChanged (int val, int max)
{
    SYS_DEBUG ("val = %d, max = %d", val, max);

    if (val > m_bar->maximum ())
    {
        m_bar->setRange (0, max - 1);
        m_bar->setValue (val);
    }
    else
    {
        m_bar->setValue (val);
        m_bar->setRange (0, max - 1);
    }
}

