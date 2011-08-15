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
#ifndef SOUNDSETTINGSUTILS_H
#define SOUNDSETTINGSUTILS_H

#include <QString>

#define COPY_SOUND_FILES

namespace SoundSettings 
{
    QString mountPoint ();
    QString userSaveDir ();
    bool isTemporaryFile (const QString &filePath);

    void suggestedTargetFilePath (
            const QString &filePath,
            QString       &baseDir,
            QString       &fileName);

    QString saveFile (
            const QString &filePath);
};
#endif
