/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEDESCRIPTOR_H
#define THEMEDESCRIPTOR_H

#include <QObject>

class QString;
class DuiDesktopEntry;

/*!
 * A simple class to handle all the information we need to know about one
 * specific theme.
 */
class ThemeDescriptor : public QObject
{
    Q_OBJECT

public:
    ThemeDescriptor (
            const QString &directoryPath,
            const QString &codeName);
    ~ThemeDescriptor ();

    bool isValid () const;
    bool isVisible () const;
    QString name() const;
    QString codeName() const;
    QString iconName() const;
    
private:
    bool              m_Valid;
    bool              m_Visible;
    QString           m_CodeName;
    QString           m_Name;
    QString           m_IconName;
    DuiDesktopEntry  *m_DesktopEntry;
};

#endif
