/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_WALLPAPEREDITORWIDGET_H
#define UT_WALLPAPEREDITORWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

class WallpaperEditorWidget;
class WallpaperBusinessLogic;
class WallpaperDescriptor;
class MApplication;

class Ut_WallpaperEditorWidget : public QObject 
{
Q_OBJECT
public:
    typedef enum {
        DescriptorNone,
        DescriptorCurrent,
        DescriptorNotCurrent,
    } DescriptorType;

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testCreateContentFail ();
    void testCreateContentCurrent ();
    void testCreateContentNonCurrent ();
    void testMouseEvents ();
    
private:
    void createObjects (DescriptorType descType = DescriptorNone);
    void dropObjects ();
    
    MApplication              *m_App;
    WallpaperBusinessLogic    *m_BusinessLogic;
    WallpaperEditorWidget     *m_Widget;
};

#endif


