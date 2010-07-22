/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEBUSINESSLOGIC_H
#define THEMEBUSINESSLOGIC_H

#include <QList>
#include <QObject>
#include <QDir>

#include "themedescriptor.h"

class QString;
class ThemeDescriptor;

class ThemeBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ThemeBusinessLogic ();
    
    QString currentThemeCodeName ();
    QString currentThemeName ();
    QString currentThemeIconName ();
    QList<ThemeDescriptor *> availableThemes ();

public slots:
    /*!
     * Sets the current theme to the theme with the given id.
     */
    void changeTheme (QString themeCodeName);
    void themeChangeCompleted ();

signals:
    void themeChangeStarted (QString themeCodeName);
    void themeChanged (QString themeCodeName);
};

#endif
