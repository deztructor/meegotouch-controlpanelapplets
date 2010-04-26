/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"

//#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QProcessEnvironment>
#include <QPainter>

#include <MTheme>
#include <MGConfItem>

#define DEBUG
#include "../debug.h"

static const QString PortraitKey = "/desktop/meego/background/landscape";
static const QString LandscapeKey = "/desktop/meego/background/portrait";

static const QString wallpaperDir = ".wallpapers";
static const QString destopFileName = "wallpaper.desktop";
static const QString backupExtension = ".BAK";

WallpaperBusinessLogic::WallpaperBusinessLogic()
{
    m_LandscapeGConfItem = new MGConfItem (LandscapeKey);
    m_PortraitGConfItem = new MGConfItem (PortraitKey);
    m_EditedImage = 0;
}

WallpaperBusinessLogic::~WallpaperBusinessLogic()
{
    delete m_LandscapeGConfItem;
    delete m_PortraitGConfItem;
}

bool
WallpaperBusinessLogic::hasWallpaperFileName (
        bool     portrait)
{
    QString filename = WallpaperFileName(portrait);

    return !filename.isEmpty();
}

QString
WallpaperBusinessLogic::WallpaperFileName (
        bool     portrait)
{
    return portrait ? 
        m_PortraitGConfItem->value().toString() :
        m_LandscapeGConfItem->value().toString();
}

WallpaperDescriptor
WallpaperBusinessLogic::Wallpaper (
        bool portrait)
{
    QString filename = WallpaperFileName (portrait);
    return WallpaperDescriptor (filename);
}

void
WallpaperBusinessLogic::setBackground (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc)
{
    SYS_DEBUG ("");
    if (desc == 0)
        desc = m_EditedImage;

    Q_ASSERT (landscapeITrans);
    Q_ASSERT (portraitITrans);
    Q_ASSERT (desc);

    ensureHasDirectory ();
    createBackupFiles ();
    writeDestopFiles (landscapeITrans, portraitITrans, desc);
}

/*!
 * This function does not support image manipulatios, it is deprecated.
 * FIXME: To remove this function.
 */
void
WallpaperBusinessLogic::setBackground (
        WallpaperDescriptor *desc)
{
    if (desc == 0)
        desc = m_EditedImage;

    Q_ASSERT (m_PortraitGConfItem != 0);
    Q_ASSERT (m_LandscapeGConfItem != 0);

    SYS_DEBUG ("*** filename = %s", SYS_STR(desc->filename()));
    m_PortraitGConfItem->set (desc->filename());
    m_LandscapeGConfItem->set (desc->filename());
}



/*!
 * Returns a list of filenames (full path filenames to be precise) of the
 * available wallpapers. These are the raw image files, before they were edited.
 *
 * FIXME: This function needs some polishing.
 */
QStringList
WallpaperBusinessLogic::availableWallpapers () const
{
    QStringList list;
#if 1
    list <<
        "/usr/share/themes/base/meegotouch/images/duiapplicationpage-background.png" <<
        "/usr/share/themes/plankton/meegotouch/images/duiapplicationpage-background.png" <<
        "/usr/share/themes/plankton/meegotouch/images/duiapplicationpage-portrait-background.png" <<
        "/usr/share/themes/plankton/meegotouch/images/duiapplicationpage-portrait-background.png";
#endif
    /*
     * FIXME:
     * I have no idea how we sould get the available images, this will do as a
     * test.
     */
    QString             home = getenv("HOME");
    QString 	        dirname = home + "/MyDocs";
    QStringList         filterList;


    filterList << "*.jpg" << "*.JPG" << "*.jpeg" << "*.JPEG" << "*.png" <<
        "*.PNG";

    SYS_DEBUG ("*** dirname = %s", SYS_STR(dirname));

    QDir imageDir (dirname);
    foreach (QString imageFile, imageDir.entryList(filterList, QDir::Files)) {
        SYS_DEBUG ("*** imageFile = %s", SYS_STR(imageFile));
        list << (dirname + "/" + imageFile);
    }

    return list;
}

void
WallpaperBusinessLogic::setEditedImage (
        WallpaperDescriptor  *desc)
{
    SYS_DEBUG ("*** desc = %s", SYS_STR(desc->filename()));
    m_EditedImage = desc;
}

WallpaperDescriptor *
WallpaperBusinessLogic::editedImage ()
{
    SYS_DEBUG ("*** m_EditedImage = %s", SYS_STR(m_EditedImage->filename()));
    return m_EditedImage;
}

QString
WallpaperBusinessLogic::dirPath () const
{
    QString homeDir (getenv("HOME"));
    SYS_DEBUG ("*** homeDir     = %s", SYS_STR(homeDir));
    QString dirPath = homeDir + "/" + wallpaperDir + "/";
    SYS_DEBUG ("*** dirPath     = %s", SYS_STR(dirPath));

    return dirPath;
}

void
WallpaperBusinessLogic::ensureHasDirectory ()
{
    QString path = dirPath();
    QDir    dir (path);

    if (dir.exists()) {
        SYS_DEBUG ("Directory %s already exists.", SYS_STR(path));
        return;
    }

    if (!dir.mkpath(path)) {
        SYS_WARNING ("Unable to create %s directory.", SYS_STR(path));
    }
}

void
WallpaperBusinessLogic::createBackupFiles ()
{
    QString  path = dirPath();
    QString  desktopPath = path + destopFileName;

    makeBackup (desktopPath);
}

void
WallpaperBusinessLogic::writeDestopFiles (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc)
{
    Q_ASSERT (landscapeITrans);
    Q_ASSERT (portraitITrans);
    Q_ASSERT (desc);

    QString  path = dirPath();
    QString  desktopPath = path + destopFileName;
    QFile    file (desktopPath);
    QString  basename = desc->basename();
    QString  portraitFilePath =
        path + basename + "-portrait." + desc->extension();
    QString  landscapeFilePath =
        path + basename + "-landscape." + desc->extension();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        SYS_DEBUG ("Opening file %s for writing failed.", SYS_STR(desktopPath));
        return;
    }

    QTextStream out(&file);
    out << "[Desktop Entry]\n";
    out << "Type=WallpaperImage\n";
    out << "Name=" << basename << "\n";
    out << "\n";

    out << "[DCP Landscape Wallpaper]\n";
    out << "OriginalFile=" << desc->filename() << "\n";
    out << "EditedFile=" << landscapeFilePath << "\n";
    out << "HorOffset=" << landscapeITrans->x() << "\n";
    out << "VertOffset=" << landscapeITrans->y() << "\n";
    out << "Scale=" << landscapeITrans->scale() << "\n";
    out << "\n";

    out << "[DCP Portrait Wallpaper]\n";
    out << "OriginalFile=" << desc->filename() << "\n";
    out << "EditedFile=" << portraitFilePath << "\n";
    out << "HorOffset=" << portraitITrans->x() << "\n";
    out << "VertOffset=" << portraitITrans->y() << "\n";
    out << "Scale=" << portraitITrans->scale() << "\n";
    out << "\n";

    makeImageFile (portraitFilePath, desc, portraitITrans);
    makeImageFile (landscapeFilePath, desc, landscapeITrans);
    
    m_PortraitGConfItem->set (portraitFilePath);
    m_LandscapeGConfItem->set (landscapeFilePath);
}

void
WallpaperBusinessLogic::makeImageFile (
            const QString        &filePath,
            WallpaperDescriptor  *desc,
            WallpaperITrans      *transformations)
{
    QPixmap  pixmap (transformations->expectedSize());
    QPixmap  original (desc->filename());
    QPainter painter (&pixmap);
    qreal    scale = transformations->scale();

    pixmap.fill (QColor("black"));
    //SYS_DEBUG ("drawPixmap (%d, %d, %d, %d, pixmap)");
    painter.drawPixmap (
                transformations->x(), transformations->y(),
                scale * transformations->expectedWidth (),
                scale * transformations->expectedHeight (),
                original);

    pixmap.save (filePath);
}

/*!
 * Takes a full path file name, removes its backup file if there is one, renames
 * the file to create a backup file.
 */
void 
WallpaperBusinessLogic::makeBackup (
        const QString &filePath)
{
    QString  backupFilePath = filePath + backupExtension;
    QFile    file (filePath);
    QFile    backupFile (backupFilePath);

    if (!file.exists())
        return;
    
    if (backupFile.exists()) {
        if (!backupFile.remove()) {
            SYS_WARNING ("Unable to remove %s backup file.", 
                    SYS_STR(backupFilePath));
            return;
        }
    }

    file.rename (backupFilePath);
}

