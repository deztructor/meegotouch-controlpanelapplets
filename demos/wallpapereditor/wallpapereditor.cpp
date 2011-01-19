/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <QWidget>
#include <QDebug>
#include <QString>

#include <MLocale>
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

MyApplication::MyApplication (int &argc, char **argv) :
    MApplication (argc, argv)
{
    MButton *button;
    
    /*
     * Checking the command line arguments, storing the strings used as
     * filenames.
     */
    if (!argv[1]) {
        fprintf (stderr, "Usage: \n  %s <imagefile1> [<imagefile2>]\n", 
                argv[0]);
        exit (1);
    }

    file1 = argv[1];
    if (argv[2])
        file2 = argv[2];
    else
        file2 = file1;

    fprintf(stderr, "**********************************************\n");
    fprintf(stderr, "file1 = %s\n", file1.toUtf8().constData());
    fprintf(stderr, "file2 = %s\n", file2.toUtf8().constData());
    fflush (stderr);

    MLocale locale;
    locale.installTrCatalog ("wallpaper");
    locale.installTrCatalog ("meegotouchcp-applets");
    MLocale::setDefault (locale);
    /*
     *
     */
    mainwindow = new MApplicationWindow;
    page1 = new MApplicationPage;
    button = new MButton ("Edit...");
    
    page1->setCentralWidget (button);
    page1->appear (mainwindow);
    
    mainwindow->show ();

    connect (button, SIGNAL(clicked()),
            this, SLOT(startEditor()));
}

void
MyApplication::startEditor ()
{
    WallpaperDescriptor      *desc;
    WallpaperBusinessLogic   *businessLogic;
    WallpaperEditorWidget    *testWidget;
    
    /*
     * Initializing.
     */
    businessLogic = new WallpaperBusinessLogic;
    desc = new WallpaperDescriptor;

    desc->setFilename (file1, WallpaperDescriptor::Landscape);
    desc->setFilename (file2, WallpaperDescriptor::Portrait);

    businessLogic = new WallpaperBusinessLogic();
    businessLogic->setEditedImage (desc);

    page2 = new MApplicationPage;
    #if 1
    /*
     * Here we create the wallpaper editor widget.
     */
    testWidget = new WallpaperEditorWidget(businessLogic);
    page2->setCentralWidget (testWidget);
    #else
    /*
     * This is just a test.
     */
    MButton *button = new MButton ("second");
    page2->setCentralWidget (button);
    #endif

    page2->appear(mainwindow);

    connect (testWidget, SIGNAL(doneClicked()),
            this, SLOT(closeEditor()));
    connect (testWidget, SIGNAL(cancelClicked()),
            this, SLOT(closeEditor()));
}

void
MyApplication::closeEditor(
        int widgetID)
{
    if (page2)
        page2->dismiss ();
}

/******************************************************************************
 * Other functions.
 */
int
main (int argc, char **argv)
{
    MyApplication    *app;

    app = new MyApplication (argc, argv);
    return app->exec();
}

