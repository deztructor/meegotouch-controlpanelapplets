/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
**
** This file is part of duicontrolpanel.
**
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef CLI_MAIN_H
#define CLI_MAIN_H

#include <QObject>
#include <QString>

class DuiControlPanelIf;
class MGConfItem;

class WallpaperCLI : public QObject 
{
    Q_OBJECT

public:
    WallpaperCLI ();
    ~WallpaperCLI ();

    bool parseCommandLineArguments (int argc, char *argv[]);
    bool executeCommandLineArguments ();
   
    void printError ();
    int exitCode () { return m_ExitCode; };

private:
    void printHelp ();
    void startControlPanel ();
    bool pageToWallpaperApplet ();
    bool performEditImages ();
    
private:
    char               *m_ProgramName;
    bool                m_OptionHelp;
    bool                m_OptionVerbose;
    bool                m_PrintGconf;
    bool                m_SetGconf;
    bool                m_EditImages;
    bool                m_OptionStartApplet;
    QString             m_OptionPortraitFilename;
    QString             m_OptionLandscapeFilename;

    DuiControlPanelIf  *m_DcpIf;
    MGConfItem         *m_LandscapeGConfItem;
    MGConfItem         *m_PortraitGConfItem;
    
    QString             m_ErrorString;
    int                 m_ExitCode;
};

#endif
