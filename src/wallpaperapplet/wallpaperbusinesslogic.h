/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERBUSINESSLOGIC_H
#define WALLPAPERBUSINESSLOGIC_H

#include <QList>
#include <QObject>

class QString;
class MGConfItem;
class WallpaperDescriptor;
class QStringList;
class WallpaperITrans;

#include <QPointer>
#include <wallpaperdescriptor.h>

class WallpaperBusinessLogic : public QObject
{
    Q_OBJECT

public:
    typedef enum {
        FieldUrl  = 0,
        FieldTitle,
        FieldMime,
    } QueryFields;

    WallpaperBusinessLogic ();
    ~WallpaperBusinessLogic ();

    QList<WallpaperDescriptor *> availableWallpapers () const;

    void setEditedImage (WallpaperDescriptor *desc, bool ours = false);
    WallpaperDescriptor *editedImage ();

    void setBackground (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc = 0);

    void checkForPendingSignals ();
    void addImageFromGallery(QString);

signals:
    void wallpaperChanged ();
    void imageEditRequested ();
    
private slots:
    void requestArrived ();

private:
    QString dirPath () const;
    bool ensureHasDirectory ();
    void createBackupFiles ();
    void deleteBackupFiles ();
    bool writeFiles (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc);

    void makeBackup (const QString &filePath);
    void makeImageFile (
            const QString        &filePath,
            WallpaperDescriptor  *desc,
            WallpaperITrans      *transformations);
    
private:
    MGConfItem   *m_LandscapeGConfItem;
    MGConfItem   *m_PortraitGConfItem;
    MGConfItem   *m_RequestGConfItem;

    QPointer<WallpaperDescriptor> m_EditedImage;
    bool                          m_EditedImageOurs;
#ifdef UNIT_TEST
    friend class Ut_WallpaperBusinessLogic;
    friend class Ft_WallpaperBusinessLogic;
#endif
};

#endif
