/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <QWidget>
#include <QDebug>
#include <QString>

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MImageWidget>
#include <MContentItem>
#include <MGConfItem>
#include <MButton>
#include <WallpaperBusinessLogic>
#include <WallpaperEditorWidget>
#include <WallpaperDescriptor>

#include "wallpapereditor.h"

MyApplication::MyApplication() :
    QObject()
{

}

/******************************************************************************
 * Other functions.
 */
int
main (int argc, char **argv)
{
    MApplication             app (argc, argv);
    MApplicationWindow       mainwindow;
    MApplicationPage         page1;
    MApplicationPage         page2;
    WallpaperBusinessLogic  *businessLogic;
    WallpaperEditorWidget   *testWidget;
    WallpaperDescriptor    *desc;
    QString                 file1;
    QString                 file2;
    MButton                 button ("Edit...");

    // Checking the command line arguments.
    if (!argv[1]) {
        fprintf (stderr, "Usage: \n  %s <imagefile1> [<imagefile2>]\n", 
                argv[0]);
        return 1;
    }

    file1 = argv[1];
    if (argv[2])
        file2 = argv[2];
    else
        file2 = file1;
   
    // Initializing.
    businessLogic = new WallpaperBusinessLogic;
    desc = new WallpaperDescriptor;

    // Setting the wallpaper here.
    desc->setFilename (file1, WallpaperDescriptor::Landscape);
    desc->setFilename (file2, WallpaperDescriptor::Portrait);

    qInstallMsgHandler (0);
    businessLogic = new WallpaperBusinessLogic();
    businessLogic->setEditedImage (desc);

    page1.setCentralWidget (&button);
    page1.appear ();

    testWidget = new WallpaperEditorWidget(businessLogic);
    page2.setCentralWidget (testWidget);
    page2.appear();

    mainwindow.show ();
    
    return app.exec ();
}

