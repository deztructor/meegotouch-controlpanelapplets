/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMECONTAINER_H
#define THEMECONTAINER_H

#include <DuiLayout>
#include <QObject>

class QString;
class DuiWidget;
class DuiLabel;

/*!
 * A simple layout type widget that represents one theme.
 */
class ThemeContainer: public QObject, public DuiLayout
{
    Q_OBJECT

public:
    ThemeContainer (
            const QString &themeName,
            DuiWidget     *parent = 0);
    ~ThemeContainer ();

private:
    void createWidgets ();

    DuiLabel    *m_NameLabel;
    QString      m_ThemeName;
};
#endif
