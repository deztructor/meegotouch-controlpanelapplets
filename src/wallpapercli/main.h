/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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
    void pageToWallpaperApplet ();
    
private:
    char               *m_ProgramName;
    bool                m_OptionHelp;
    bool                m_OptionVerbose;
    bool                m_PrintGconf;
    bool                m_SetGconf;
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
