/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERCURRENTDESCRIPTOR_H
#define WALLPAPERCURRENTDESCRIPTOR_H

#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"

class MDesktopEntry;

class Q_DECL_EXPORT WallpaperCurrentDescriptor : public WallpaperDescriptor {
    Q_OBJECT

public:
    static WallpaperCurrentDescriptor *instance ();

    virtual bool isCurrent () const;
    virtual int version () const;
    
    virtual QString suggestedOutputFilename (M::Orientation orientation) const; 
    virtual QString originalImageFile (M::Orientation orientation) const;
    QString editedFilename (M::Orientation orientation) const;

    bool setFromDestopFile (QString desktopFileName);
    WallpaperITrans iTrans (M::Orientation orientation) const;
    
protected:
    WallpaperCurrentDescriptor ();

private:
    bool getValue (
            const QString   &group,
            const QString   &key, 
            QString         &value);

    bool getValue (
            const QString   &group,
            WallpaperITrans &value);

    bool getValue (
            const QString   &group,
            const QString   &key, 
            qreal           *value);
    
private:
    static WallpaperCurrentDescriptor *sm_Instance;

    MDesktopEntry         *m_DesktopEntry;
    WallpaperITrans        m_LandscapeTrans;
    WallpaperITrans        m_PortraitTrans;
    QString                m_LandscapeOriginalFile;
    QString                m_PortraitOriginalFile;
    QString                m_landscapeEditedFile;
    QString                m_portraitEditedFile;
    int                    m_Version;
};

#endif
