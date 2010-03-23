/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "volumebarlogic.h"

#include <QVariant>
#include <QString>

#define DEBUG
#define WARNING
#include "../debug.h"

VolumeBarLogic::VolumeBarLogic () :
    QObject (),
    m_currentvolume (0),
    m_currentmax (0)
{
    m_if = new QDBusInterface ("com.Nokia.MainVolume1",
                               "/com/nokia/mainvolume1",
                               "com.Nokia.MainVolume1");

    SYS_DEBUG ("com.Nokia.MainVolume interface revision: %d",
               m_if->property ("InterfaceRevision").toInt ());

    m_currentmax = m_if->property ("StepCount").toInt ();
    m_currentvolume = m_if->property ("CurrentStep").toInt ();

    connect (m_if, SIGNAL (StepsUpdated (int, int)),
             this, SIGNAL (volumeChanged (int, int)));
}

VolumeBarLogic::~VolumeBarLogic ()
{
    delete m_if;
}

void
VolumeBarLogic::setVolume (int value)
{
    SYS_DEBUG ("value = %d", value);

    m_if->setProperty ("CurrentStep", QVariant (value));
}

int
VolumeBarLogic::getVolume ()
{
    SYS_DEBUG ("volume = %d", m_currentvolume);

    return m_currentvolume;
}

int
VolumeBarLogic::getMaxVolume ()
{
    SYS_DEBUG ("max = %d", m_currentmax);

    return m_currentmax;
}

