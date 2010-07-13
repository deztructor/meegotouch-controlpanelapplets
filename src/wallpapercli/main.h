/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef CLI_MAIN_H
#define CLI_MAIN_H

#include <QObject>
#include <QString>

class DuiControlPanelIf;

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
    bool                m_OptionStartApplet;
    DuiControlPanelIf  *m_DcpIf;
    QString             m_ErrorString;
    int                 m_ExitCode;
};

#endif
