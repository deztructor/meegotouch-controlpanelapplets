/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEBUSINESSLOGIC_H
#define THEMEBUSINESSLOGIC_H

#include <QList>
#include <QStringList>
#include <QObject>
#include <QDir>
#include <MApplication>

#include "themebusinesslogicadaptor.h"
#include "themedescriptor.h"

class QString;
class ThemeDescriptor;

class ThemeBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ThemeBusinessLogic ();
    ~ThemeBusinessLogic ();
    
    QString currentThemeCodeName ();
    QString currentThemeName ();
    QString currentThemeIconName ();
    QList<ThemeDescriptor *> availableThemes ();

    ThemeDescriptor *themeByCodename (const QString &codeName);

    QString themePreviewFileName (
            const QString  &themeCodeName,
            M::Orientation orientation) const;

public slots:
    /*!
     * Sets the current theme to the theme with the given id.
     */
    void changeTheme (QString themeCodeName);
    void themeChangeCompleted ();
    void startupDBusAdaptor ();
    void stopDBusAdaptor ();
    void themeAdded (QString themeName);
    void themeRemoved (QString themeName);

signals:
    void themeChangeStarted (QString themeCodeName);
    void themeChanged (QString themeCodeName);
    void themeAboutToBeRemoved (int index);
    void themeRemoved (QList<ThemeDescriptor *> list);
    void themeAboutToBeAdded (int index);
    void themeAdded (QList<ThemeDescriptor *> list);

private:
    QPointer<ThemeBusinessLogicAdaptor> m_ThemeBusinesslogicAdaptor;
    QList<ThemeDescriptor *>            m_AvailableThemes;
    QStringList                         m_DisabledThemeNames;
};

#endif
