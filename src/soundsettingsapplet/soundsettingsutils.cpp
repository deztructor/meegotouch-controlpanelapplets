/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include "soundsettingsutils.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QXmlStreamWriter>

#include "trackerconnection.h"

#define DEBUG
#define WARNING
#include "../debug.h"

QString
SoundSettings::mountPoint ()
{
    QString  retval = QDir::homePath () + "/MyDocs/";

    return retval;
}

QString
SoundSettings::userSaveDir ()
{
    QString retval = "~/.ringtones";

    retval.replace ("~", QDir::homePath ());

    return retval;
}

bool 
SoundSettings::isTemporaryFile (
        const QString &filePath)
{
    bool     retval = filePath.startsWith(mountPoint());

    SYS_DEBUG ("*** filePath   = %s", SYS_STR(filePath));
    SYS_DEBUG ("*** retval     = %s", SYS_BOOL(retval));

    return retval;
}

bool 
SoundSettings::isFileCopy (
        const QString &filePath)
{
    bool     retval = filePath.startsWith(userSaveDir());

    SYS_WARNING ("returning %s", SYS_BOOL(retval));
    return retval;
}

void 
SoundSettings::suggestedXmlFilePath (
            const QString &filePath,
            QString       &xmlFilePath)
{
    QFileInfo          fileInfo (filePath);

    xmlFilePath = fileInfo.path() + QDir::separator() +
        fileInfo.baseName() + ".xml"; 
}

void
SoundSettings::suggestedTargetFilePath (
        const QString &filePath,
        QString       &baseDir,
        QString       &fileName,
        QString       &xmlFileName)
{
    QString            subDir;
    QFileInfo          fileInfo (filePath);
    QCryptographicHash hash(QCryptographicHash::Sha1);

    hash.addData (fileInfo.path().toUtf8());
    subDir = QString(hash.result().toHex());
    subDir.truncate (32);

    baseDir = userSaveDir() +
        QDir::separator() +
        subDir;

    fileName = fileInfo.baseName() + "." + fileInfo.completeSuffix();
    xmlFileName = fileInfo.baseName() + ".xml";

    SYS_DEBUG ("*** baseName()       = %s", SYS_STR(fileInfo.baseName()));
    SYS_DEBUG ("*** path()           = %s", SYS_STR(fileInfo.path()));
    SYS_DEBUG ("*** completeSuffix() = %s", SYS_STR(fileInfo.completeSuffix()));
}

bool
SoundSettings::loadXML (
        const QString   &xmlFileName,
        QString         &origFileName,
        QString         &copyFileName,
        QString         &title)
{
    bool retval = false;
    SYS_DEBUG ("*** xmlFileName = %s", SYS_STR(xmlFileName));

    return retval;
}

void
SoundSettings::saveXML (
        const QString   &xmlFileName,
        const QString   &origFileName,
        const QString   &copyFileName,
        const QString   &title)
{
    QXmlStreamWriter *writer;
    QFile             file (xmlFileName);
    
    SYS_WARNING ("-----------------------------------------------------");
    SYS_WARNING ("*** xmlFileName = %s", SYS_STR(xmlFileName));

    if (!file.open(QIODevice::WriteOnly)) {
        SYS_WARNING ("Unable to open file for writing: %s", 
                SYS_STR(xmlFileName));
        return;
    }

    /*
     *
     */
    writer = new QXmlStreamWriter();
    writer->setDevice (&file);
    writer->setAutoFormatting(true);
    writer->setCodec ("UTF-8");
    writer->writeStartDocument ();
	writer->writeStartElement ("soundsettings-applet");

    writer->writeStartElement("orig-file");
        writer->writeCharacters (origFileName);
    writer->writeEndElement ();
    
    writer->writeStartElement("copy-file");
        writer->writeCharacters (copyFileName);
    writer->writeEndElement ();
    
    writer->writeStartElement("title");
        writer->writeCharacters (title);
    writer->writeEndElement ();

    /*
     *
     */
    writer->writeEndElement();
	writer->writeEndDocument();

     
    delete writer;
    file.close ();
}

QString 
SoundSettings::saveFile (
        const QString &filePath)
{
    QFile         sourceFile (filePath);
    QString       baseDir;
    QString       fileName;
    QString       xmlFileName;
    QString       retval = filePath;
    QDir          baseDirectory;
    QString       targetFilePath;

    suggestedTargetFilePath (filePath, baseDir, fileName, xmlFileName);

    SYS_DEBUG ("*** baseDir  = %s", SYS_STR(baseDir));
    SYS_DEBUG ("*** fileName = %s", SYS_STR(fileName));
    baseDirectory = QDir (baseDir);
    if (!baseDirectory.exists()) {
        if (!QDir::root().mkpath(baseDir)) {
            SYS_WARNING ("ERROR: mkdir(%s) failed.", SYS_STR(baseDir));
            goto finalize;
        }
    }

    targetFilePath = 
        baseDir + QDir::separator() + fileName;
    xmlFileName =
        baseDir + QDir::separator() + xmlFileName;
    
    if (QFile(targetFilePath).exists()) {
        SYS_DEBUG ("The file '%s' already exists.", SYS_STR(targetFilePath));
        retval = targetFilePath;
        goto finalize;
    }

    if (sourceFile.copy(targetFilePath)) {
        SYS_DEBUG ("File copy to %s success.", SYS_STR(targetFilePath));
        retval = targetFilePath;
    } else {
        SYS_WARNING ("ERROR: Unable to copy %s -> %s: %m",
                SYS_STR(filePath),
                SYS_STR(targetFilePath));
    }

finalize:
    if (retval != filePath) {
        TrackerConnection *tracker = TrackerConnection::instance();
        QString title;

        tracker->registerFileCopy (filePath, retval);
        title = tracker->niceNameFromFileName (filePath);
        saveXML (xmlFileName, filePath, retval, title);
    }

    return retval;
}
