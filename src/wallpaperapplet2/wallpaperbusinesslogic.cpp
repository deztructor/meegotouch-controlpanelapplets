/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
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
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
** Authors: David Kedves <dkedves@blumsoft.eu>
**          Laszlo Pere <lpere@blumsoft.eu>
**
****************************************************************************/


#include "wallpaperbusinesslogic.h"
#include "wallpaperutils.h"
#include "wallpaperconfiguration.h"

#include "wallpapergconf.h"
#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"

#include <QString>
#include <QStringList>
#include <QProcessEnvironment>
#include <QPainter>
#include <QDBusConnection>
#include <QDBusError>
#include <MTheme>
#include <MGConfItem>
#include <MApplication>
#include <MApplicationWindow>

#define DEBUG
#define WARNING
#include "../debug.h"

static const QString wallpaperDir = ".wallpapers";
static const QString wallpapersDir = "MyDocs/.wallpapers";
static const QString destopFileName = "wallpaper.desktop";
static const QString backupExtension = ".BAK";
static const QString saveFileExtension = ".png";
static const QString saveFileMimeType = "image/png";
static const QString nl = "\n";

WallpaperBusinessLogic::WallpaperBusinessLogic() :
    m_OrientationLocked (false)
{
    m_PPItem = new MGConfItem (Wallpaper::CurrentPortraitKey, this);
    m_POItem = new MGConfItem (Wallpaper::OriginalPortraitKey, this);

    connect (m_PPItem, SIGNAL(valueChanged ()),
            this, SLOT(portraitGConfChanged()));

    connect (m_POItem, SIGNAL(valueChanged ()),
            this, SLOT(portraitGConfChanged()));




#if 0
    MApplication               *application = MApplication::instance();
    MApplicationWindow         *window = 0;
    WallpaperCurrentDescriptor *currentDesc;
    QString                     desktopFile = dirPath() + destopFileName;
    QString                     landscapeString;
    QString                     portraitString;
    bool                        success;
    
    if (application) {
        window = application->activeApplicationWindow ();
    }

    if (window) {
        m_OrientationLocked = window->isOrientationLocked ();
        if (m_OrientationLocked)
            m_LockedOrientation = window->orientation ();
    } else {
        SYS_WARNING ("No window");
    }

    SYS_WARNING ("m_OrientationLocked = %s", SYS_BOOL(m_OrientationLocked));
    SYS_WARNING ("supportsLandscape() = %s", SYS_BOOL(supportsLandscape()));
    SYS_WARNING ("supportsPortrait()  = %s", SYS_BOOL(supportsPortrait()));

    /*
     * Getting the GConf items storing the current settings.
     */
    m_LandscapeGConfItem = new MGConfItem (WALLPAPER_LANDSCAPE_KEY);
    m_PortraitGConfItem = new MGConfItem (WALLPAPER_PORTRAIT_KEY);
    m_EditedImage = 0;
    m_EditedImageOurs = false;

    /*
     * In case if GConf keys are set to "", we have to unset it to use the
     * schema values
     */
    if (m_LandscapeGConfItem->value ().toString ().isEmpty ())
        m_LandscapeGConfItem->unset ();
    if (m_PortraitGConfItem->value ().toString ().isEmpty ())
        m_PortraitGConfItem->unset ();

    currentDesc = WallpaperCurrentDescriptor::instance ();
    /*
     * Trying to load the current wallpaper from the files saved by the theme
     * applet. Will load only if the GConf contains our filenames.
     */
    if (supportsLandscape())
        landscapeString = m_LandscapeGConfItem->value().toString();
    if (supportsPortrait())
        portraitString = m_PortraitGConfItem->value().toString();

    SYS_DEBUG ("*** landscape = '%s'", SYS_STR(landscapeString));
    SYS_DEBUG ("*** portrait  = '%s'", SYS_STR(portraitString));

    success = currentDesc->setFromDesktopFile (
            desktopFile,
            true, landscapeString, portraitString);
    
    /*
     * If not successfull we try to load the files from the GConf database. Will
     * be successfull if both GConf keys reference to images with full path.
     */
    if (!success) {
        SYS_DEBUG ("Loading of %s failed. Trying image files from GConf.",
                SYS_STR(desktopFile));
        success = currentDesc->setFromFilenames (
                landscapeString, 
                portraitString);
    }

    /*
     * Trying to interpret the values as icon IDs. Please note, that this
     * setFromIDs() method will handle the case when one of the values is an
     * image file and the other is an icon ID. 
     */
    if (!success) {
        success = currentDesc->setFromIDs (
                landscapeString,
                portraitString);
    }

    /*
     * Now we failed. But it does not happen, and when it happens nothing
     * serious, we just have no wallpaper.
     */
    if (!success) {
        SYS_WARNING ("Current wallpaper was not found.");
    }

    connect (&m_FutureWatcher, SIGNAL (finished()),
            this, SLOT (startEditThreadEnded()));

    /*
     * Connecting to the DBus database.
     */
    connect (m_LandscapeGConfItem, SIGNAL(valueChanged()),
            this, SLOT(valueChanged()));
    connect (m_PortraitGConfItem, SIGNAL(valueChanged()),
            this, SLOT(valueChanged()));
    
    /*
     * Connecting to DBus.
     */
    QDBusConnection bus = QDBusConnection::sessionBus ();
    
    success = bus.connect("", "", 
            WALLPAPER_DBUS_INTERFACE, WALLPAPER_DBUS_EDIT_SIGNAL, 
            this, SLOT(editRequestArrived(QString, QString)));
    if (!success) {
        QDBusError lastError = bus.lastError();
        SYS_WARNING ("Connecting to DBus failed: %s", 
                SYS_STR(lastError.message()));
    }
#endif
}

WallpaperBusinessLogic::~WallpaperBusinessLogic()
{
}

/*!
 * \param landscapeITrans Image transformations for the landscape orientation.
 * \param portraitITrans Image transformations for the portrait orientation.
 * \param desc The image descriptor that holds the image(s).
 *
 * A high level method to set the current wallpaper. This method will load the
 * image file(s), apply the image transformations to create the portrait and
 * landscape variants then will save the modified images and set them as
 * wallpaper image(s). This method will also save some metadata that makes it
 * possible to re-edit the images using the image transformations as default
 * values for the image editor widget.
 */
void
WallpaperBusinessLogic::setBackground (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc)
{
#if 0
    bool success;

    if (desc == 0)
        desc = m_EditedImage;

    Q_ASSERT (landscapeITrans);
    Q_ASSERT (portraitITrans);
    Q_ASSERT (desc);

    success = ensureHasDirectory ();
    if (!success)
        return;

    createBackupFiles ();

    success = writeFiles (landscapeITrans, portraitITrans, desc);
    if (!success) {
        // FIXME: Should restore backup files here.
        return;
    }
   
    deleteBackupFiles ();

    /*
     * Re-reading the current wallpaper as it is saved into the user's home.
     */
    WallpaperCurrentDescriptor *currentDesc;
    currentDesc = WallpaperCurrentDescriptor::instance ();
    currentDesc->setFromDesktopFile (dirPath() + destopFileName);
    emit wallpaperChanged ();
#endif
}

/*!
 * Returns a list wallpaper descriptors with the available wallpapers. The
 * current wallpaper is the first element of the available wallpapers.
 *
 * FIXME: This function needs some polishing.
 */
QList<WallpaperDescriptor *>
WallpaperBusinessLogic::availableWallpapers () const
{
    QList<WallpaperDescriptor *>   list;

    SYS_DEBUG ("We have %d wallpapers.", list.size());
    return list;
}

/*!
 * \param ours if true the descriptor will be destroyed when not needed any more
 *
 * While a wallpaper image is edited the WallpaperBusinessLogic holds a
 * descriptor on it. This function is used to set this wallpaper descriptor.
 */
void
WallpaperBusinessLogic::setEditedImage (
        WallpaperDescriptor  *desc, 
        bool                  ours)
{
#if 0
    SYS_DEBUG ("*** desc = %s", 
            desc ? SYS_STR(desc->filename()) : "NULL");

    if (m_EditedImage == desc)
        return;

    if (m_EditedImage && m_EditedImageOurs)
        delete m_EditedImage;
    
    if (m_EditedImage)
        m_EditedImage->unCache ();

    m_EditedImage = desc;
    m_EditedImageOurs = ours;
#endif
}

void
WallpaperBusinessLogic::startEdit (
        WallpaperDescriptor *desc)
{
#if 0
    bool threadActive = m_FutureWatcher.isRunning();

    /*
     * If the user double clicked on the same image we are currently loading in
     * the other thread. Somehow we got this signal twice because of the DBus
     * interface implementation... no problem though.
     */
    if (threadActive && m_EditedImage && m_EditedImage == desc) {

        SYS_DEBUG ("The thread is already active for the same image.");
        return;
    }

    if (threadActive) {
        SYS_DEBUG ("The thread is already active.");
        return;
    }

    /*
     * If we already have and image we are editing and the user clicks on an
     * other image we remove the loading indicator progress bar from the old
     * one. This is kind a cosmetic fix since we should abort the other thread.
     */
    if (desc && m_EditedImage && desc != m_EditedImage) {
        SYS_DEBUG ("We have an edited image already.");
        return;
    }

    /*
     * If we got an image descriptor we start editing that, if not we start
     * editing the editedimage set before.
     */
    if (desc) {
        setEditedImage (desc);
    } else {
        desc = m_EditedImage;
    }

    desc->setLoading (true);

    /*
     * The loadAll() method is safe to call from outside the GUI thread.
     */
    QFuture<void> future = QtConcurrent::run (
            desc, 
            &WallpaperDescriptor::loadAll);
    m_FutureWatcher.setFuture (future);
#endif
}

void 
WallpaperBusinessLogic::startEditThreadEnded ()
{
    SYS_DEBUG ("");
    /*
     * Calling from loadall, this time from the GUI thread.
     */
#if 0
    m_EditedImage->loadAll ();
    m_EditedImage->setLoading (false);
    emit imageEditRequested();
#endif
}


/*!
 * Returns the wallpaper that is being edited.
 */
WallpaperDescriptor *
WallpaperBusinessLogic::editedImage ()
{
    return m_EditedImage;
}

/*********************************************************************************
 * Low level file handling functions.
 */

/*!
 * \param downloadDir If the directory where the images are downloaded should be
 *     returned.
 *
 * Returns the directory path that is used to store the information about the
 * edited wallpaper. This is the data storing directory for the wallpaper
 * applet.
 */
QString
WallpaperBusinessLogic::dirPath (
        bool       downloadDir) const
{
    QString dirPath;
#if 0
    QString homeDir (getenv("HOME"));
    
    if (homeDir.isEmpty())
        homeDir = "/usr/home";

    if (downloadDir)
        dirPath = homeDir + WallpaperDir::separator() + 
            wallpapersDir + WallpaperDir::separator();
    else
        dirPath = homeDir + WallpaperDir::separator() + 
            wallpaperDir + WallpaperDir::separator();
#endif
    return dirPath;
}


/*!
 * \returns true if the directory exists or could be created
 *
 * If the data store directory does not exists this method will create it.
 */
bool
WallpaperBusinessLogic::ensureHasDirectory ()
{
#if 0
    QString path = dirPath();
    WallpaperDir  dir (path);

    if (dir.exists()) {
        SYS_DEBUG ("Directory %s already exists.", SYS_STR(path));
        return true;
    }

    if (!dir.mkpath(path)) {
        SYS_WARNING ("Unable to create %s directory.", SYS_STR(path));
        return false;
    }
#endif
    return true;
}

/*!
 * Takes the desktop file, the saved landscape image file and the saved portrait
 * file and moves/renames them to create a backup version of each of them.
 */
void
WallpaperBusinessLogic::createBackupFiles ()
{
#if 0
    QString  path = dirPath();
    QString  desktopPath = path + destopFileName;
    QString  filename;
    
    makeBackup (desktopPath);

    filename = WallpaperCurrentDescriptor::instance()->editedFilename (
            M::Portrait);
    if (!filename.isEmpty())
        makeBackup (filename);

    filename = WallpaperCurrentDescriptor::instance()->editedFilename (
            M::Landscape);
    if (!filename.isEmpty())
        makeBackup (filename);
#endif
}

/*!
 * Removes all the files from the applet's data directory that has the file name
 * extension indicating that it is a backup file.
 */
void
WallpaperBusinessLogic::deleteBackupFiles ()
{
#if 0
    QString       path = dirPath();
    WallpaperDir  dir (path);
    QStringList   nameFilters ("*.BAK");

    dir.setNameFilters (nameFilters);
    foreach (QString backupFileName, dir.entryList (WallpaperDir::Files)) {
        SYS_DEBUG ("Removing backup file %s", SYS_STR(backupFileName));
        WallpaperFile backupFile (path + backupFileName);

        if (!backupFile.remove()) {
            SYS_WARNING ("Unable to remove %s backup file.",
                    SYS_STR((path + backupFileName)));
        }
    }
#endif
}

void
WallpaperBusinessLogic::saveOriginal (
        WallpaperDescriptor *desc)
{
#if 0
    QString imageID;
    
    imageID = desc->imageID (WallpaperDescriptor::OriginalLandscape);
    if (!imageID.isEmpty()) {
        QImage image;
        QString filename;

        image = desc->image (WallpaperDescriptor::OriginalLandscape);
        ensureHasDirectory ();
        filename = dirPath () +
            MTheme::currentTheme () + "-" +
            imageID + 
            saveFileExtension;

        image.save (filename);
        desc->setFilename (filename, WallpaperDescriptor::OriginalLandscape);
        desc->setMimeType (saveFileMimeType, WallpaperDescriptor::OriginalLandscape);
    }

    imageID = desc->imageID (WallpaperDescriptor::OriginalPortrait);
    if (!imageID.isEmpty()) {
        QImage  image;
        QString filename;

        image = desc->image (WallpaperDescriptor::OriginalPortrait);
        ensureHasDirectory ();
        filename = dirPath () +
            MTheme::currentTheme () + "-" +
            imageID + 
            saveFileExtension;

        image.save (filename);
        desc->setFilename (filename, WallpaperDescriptor::OriginalPortrait);
        desc->setMimeType (saveFileMimeType, WallpaperDescriptor::OriginalPortrait);
    }
#endif
}

/*!
 * \returns true if the files could be created and saved.
 *
 */
bool
WallpaperBusinessLogic::writeFiles (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc)
{
#if 0
    Q_ASSERT (landscapeITrans);
    Q_ASSERT (portraitITrans);
    Q_ASSERT (desc);

    /*
     * Some last minute checking. The expected sizes are very important when we
     * save the images, and some people are using us as libraries.
     *
     * FIXME: We should not use these wired in values here.
     */
    if (landscapeITrans->expectedWidth() <= 0 ||
            landscapeITrans->expectedHeight() <= 0) {
        landscapeITrans->setExpectedSize (QSize(854, 480));
    }
    
    if (portraitITrans->expectedWidth() <= 0 ||
            portraitITrans->expectedHeight() <= 0) {
        portraitITrans->setExpectedSize (QSize(480, 854));
    }

    /*
     * These are pretty constants.
     */
    QString       path = dirPath();
    QString       desktopPath = path + destopFileName;
    WallpaperFile file (desktopPath);
    SYS_DEBUG ("*** desktopPath = %s", SYS_STR(desktopPath));
    WallpaperCurrentDescriptor *currentDesc = 
        WallpaperCurrentDescriptor::instance();

    saveOriginal (desc);

    // There is of course a reason why we use the version number from the
    // current descriptor: otherwise the re-editing of the original filename
    // might end up with the same version number.
    int      version = currentDesc->version () + 1;
    QString  versionString = QString::number(version);
    QString  portraitFilePath = path + 
        desc->suggestedOutputFilename (M::Portrait, version);
    QString  landscapeFilePath = path + 
        desc->suggestedOutputFilename (M::Landscape, version);


    /*
     * Opening the output file.
     */
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        SYS_DEBUG ("Opening file %s for writing failed.", SYS_STR(desktopPath));
        return false;
    }

    /*
     * FIXME: This is the new stuff, but in order to use it the current
     * wallpaper descriptor has to be upgraded with from the arguments of this
     * method.
     */
    QTextStream out(&file);

    /*
     * 
     */
    out << "[Desktop Entry]" << nl;
    out << "Type=WallpaperImage" << nl;
    out << "Version=" << QString::number(version) << nl;
    out << nl;

    if (supportsLandscape()) {
        out << "[DCP Landscape Wallpaper]" << nl;
        out << "OriginalFile=" << desc->originalImageFile(M::Landscape) << nl;
        out << "EditedFile=" << landscapeFilePath << nl;
        out << "MimeType=" << desc->suggestedOutputMimeType (M::Landscape) << nl;
        out << "HorOffset=" << landscapeITrans->x() << nl;
        out << "VertOffset=" << landscapeITrans->y() << nl;
        out << "Scale=" << landscapeITrans->scale() << nl;
        out << nl;
    
        makeImageFile (landscapeFilePath, desc, landscapeITrans);
        m_LandscapeGConfItem->set (landscapeFilePath);
    }

    if (supportsPortrait()) {
        out << "[DCP Portrait Wallpaper]" << nl;
        out << "OriginalFile=" << desc->originalImageFile(M::Portrait) << nl;
        out << "EditedFile=" << portraitFilePath << nl;
        out << "MimeType=" << desc->suggestedOutputMimeType (M::Portrait) << nl;
        out << "HorOffset=" << portraitITrans->x() << nl;
        out << "VertOffset=" << portraitITrans->y() << nl;
        out << "Scale=" << portraitITrans->scale() << nl;
        out << nl;
    
        makeImageFile (portraitFilePath, desc, portraitITrans);
        m_PortraitGConfItem->set (portraitFilePath);
    }
#endif
    return true;
}

/*!
 * \param filePath The path of the file to save the image into.
 * \param desc The image that should be saved.
 * \param transformations The structure that descibes how to modify the image.
 *
 * This is a low level image manupilation method that takes a wallpaper and
 * saves it into a file with the given manupilations.
 * 
 * FIXME: Maybe this method should be moved into some other class?
 */
void
WallpaperBusinessLogic::makeImageFile (
            const QString        &filePath,
            WallpaperDescriptor  *desc,
            WallpaperITrans      *transformations)
{
#if 0
    QPixmap   pixmap (transformations->expectedSize());
    QPainter  painter (&pixmap);
    qreal     scale = transformations->scale();
    QImage    image;
    qreal     ratio, ratio1;
    bool      success;

    SYS_DEBUG ("*** expectedsize = %dx%d", 
            transformations->expectedWidth(),
            transformations->expectedHeight());
    /*
     * And this is exactly why we should move this kind of stuff into the image
     * descriptor classes.
     */
    if (desc->isCurrent()) {
#if 0
        WallpaperCurrentDescriptor *cdesc;
        
        cdesc = qobject_cast<WallpaperCurrentDescriptor*> (desc);
        image.load (cdesc->originalImageFile (transformations->orientation()));
#endif
        image = desc->image (WallpaperDescriptor::OriginalPortrait);
    } else {
        image = desc->image();
    }

    ratio = 
        (qreal) transformations->expectedHeight () / 
        (qreal) image.height();

    ratio1 = 
        (qreal) transformations->expectedWidth () / 
        (qreal) image.width();
    
    if (ratio1 > ratio)
        ratio = ratio1;

    /*
     * Let's fill the pixmap with black, so the area not covered by the original
     * pixmap is initialized.
     */
    pixmap.fill (QColor("black"));
    
    /*
     * Then we draw the scaled image with the appropriate offset.
     */
    painter.drawImage (
            QRectF (transformations->x(), 
                transformations->y(),
                (scale * image.width ()) /** ratio*/,
                (scale * image.height ()) /** ratio*/),
            image);

    SYS_DEBUG ("Saving file into %s", SYS_STR(filePath));
    success = pixmap.save (filePath);
    if (!success) {
        SYS_WARNING ("Saving file to %s failed", SYS_STR(filePath));
    }
#endif
}

/*!
 * Takes a full path file name, removes its backup file if there is one, renames
 * the file to create a backup file.
 */
void 
WallpaperBusinessLogic::makeBackup (
        const QString &filePath)
{
#if 0
    QString  backupFilePath = filePath + backupExtension;
    WallpaperFile    file (filePath);
    WallpaperFile    backupFile (backupFilePath);

    if (!file.exists())
        return;
    
    if (backupFile.exists()) {
        if (!backupFile.remove()) {
            SYS_WARNING ("Unable to remove %s backup file.", 
                    SYS_STR(backupFilePath));
            return;
        }
    }

    SYS_DEBUG ("Moving %s -> %s", SYS_STR(filePath), SYS_STR(backupFilePath));
    file.rename (backupFilePath);
#endif
}

/*
 * This slot is activated when an image edit is requested through DBus.
 */
void 
WallpaperBusinessLogic::editRequestArrived (
        QString   portraitFileName,
        QString   landscapeFileName)
{
#if 0
    WallpaperDescriptor *desc;
    SYS_DEBUG ("*** portraitFileName  = %s", SYS_STR(portraitFileName));
    SYS_DEBUG ("*** landscapeFileName = %s", SYS_STR(landscapeFileName));

    desc = new WallpaperDescriptor ();
    desc->setFilename (landscapeFileName, WallpaperDescriptor::Landscape);
    desc->setFilename (portraitFileName, WallpaperDescriptor::Portrait);
    setEditedImage (desc, true);

    startEdit ();
#endif
}

void
WallpaperBusinessLogic::valueChanged ()
{
#if 0
    WallpaperCurrentDescriptor *currentDesc;
    QString                     desktopFile = dirPath() + destopFileName;
    bool                        success;

    currentDesc = WallpaperCurrentDescriptor::instance ();
    
    /*
     * Trying to load the current wallpaper from the files saved by the theme
     * applet. Will load only if the GConf contains our filenames.
     */
    success = currentDesc->setFromDesktopFile (
            desktopFile,
            true,
            m_LandscapeGConfItem->value().toString(),
            m_PortraitGConfItem->value().toString());
    /*
     * If not successfull we try to load the files from the GConf database. Will
     * be successfull if both GConf keys reference to images with full path.
     */
    if (!success) {
        SYS_DEBUG ("Loading of %s failed. Trying image files from GConf.",
                SYS_STR(desktopFile));
        success = currentDesc->setFromFilenames (
                m_LandscapeGConfItem->value().toString(),
                m_PortraitGConfItem->value().toString());
    }

    /*
     * Trying to interpret the values as icon IDs. Please note, that this
     * setFromIDs() method will handle the case when one of the values is an
     * image file and the other is an icon ID. 
     */
    if (!success) {
        success = currentDesc->setFromIDs (
                m_LandscapeGConfItem->value().toString(),
                m_PortraitGConfItem->value().toString());
    }

    emit wallpaperChanged ();
#endif
}

bool
WallpaperBusinessLogic::supportsLandscape () const
{
    return !m_OrientationLocked || m_LockedOrientation == M::Landscape;
}

bool
WallpaperBusinessLogic::supportsPortrait () const
{
    return !m_OrientationLocked || m_LockedOrientation == M::Portrait;
}

/******************************************************************************
 * Handling the GConf database.
 */
bool
WallpaperBusinessLogic::portraitGConfChanged ()
{
    SYS_DEBUG ("");
    emit wallpaperChanged ();
}

void
WallpaperBusinessLogic::currentWallpaper (
        QString   &currentFilePath,
        QString   &originalFilePath)
{
    currentFilePath  = m_PPItem->value().toString ();
    originalFilePath = m_POItem->value().toString ();

    currentFilePath  = Wallpaper::logicalIdToFilePath (currentFilePath);
    originalFilePath = Wallpaper::logicalIdToFilePath (originalFilePath);
}
