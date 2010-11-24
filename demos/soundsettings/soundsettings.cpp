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

#include "soundsettings.h"

MyApplication::MyApplication (int &argc, char **argv) :
    MApplication (argc, argv)
{
    MButton *button;
    
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
    page2 = new MApplicationPage;
    /*
     * This is just a test.
     */
    MButton *button = new MButton ("second");
    page2->setCentralWidget (button);
    page2->appear(mainwindow);
#if 0
    connect (testWidget, SIGNAL(doneClicked()),
            this, SLOT(closeEditor()));
    connect (testWidget, SIGNAL(cancelClicked()),
            this, SLOT(closeEditor()));
#endif
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

