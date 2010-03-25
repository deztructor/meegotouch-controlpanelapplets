/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themedescriptor.h"

#include <QString>
#include <QFile>

#define DEBUG
#include "../debug.h"

ThemeDescriptor::ThemeDescriptor (
        const QString &directoryPath) :
    m_Valid (false)
{
    QString indexFileName (directoryPath + "/index.theme");
    QFile indexFile(indexFileName);

    if (!indexFile.exists())
        return;

    m_Valid = true;
}


bool
ThemeDescriptor::isValid () const
{
    return m_Valid;
}
