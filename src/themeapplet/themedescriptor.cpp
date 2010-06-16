/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themedescriptor.h"

#include <QString>
//#include <QFile>

#if defined(UNIT_TEST) && !defined(FUNCTIONAL_TEST)
#  include "mdesktopentrystub.h"
#else
#  include <MDesktopEntry>
#endif

#define DEBUG
#include "../debug.h"


static const QString requiredType = "X-MeeGoTouch-Metatheme";
static const QString typeKey = "Desktop Entry/Type";

static const QString nameKey = "Desktop Entry/Name";
static const QString iconNameKey = "X-MeeGoTouch-Metatheme/X-Icon";
static const QString visibleKey = "X-MeeGoTouch-Metatheme/X-Visible";

ThemeDescriptor::ThemeDescriptor (
        const QString &directoryPath,
        const QString &codeName) :
    m_Valid (false),
    m_Visible (false),
    m_CodeName (codeName),
    m_DesktopEntry (0)
{
    QString indexFileName (directoryPath + "/index.theme");
    //QFile indexFile(indexFileName);

    //if (!indexFile.exists())
    //    return;

    m_DesktopEntry = new MDesktopEntry (indexFileName);
    if (!m_DesktopEntry->isValid())
        return;

    if (m_DesktopEntry->value(typeKey) != requiredType) {
        SYS_DEBUG ("The type not right: '%s', should be '%s'", 
                SYS_STR(m_DesktopEntry->value(typeKey)),
                SYS_STR(requiredType));
        return;
    }

    m_Visible = m_DesktopEntry->value (visibleKey) == "true";
    m_Name = m_DesktopEntry->value (nameKey);
    m_IconName =  m_DesktopEntry->value (iconNameKey).
        trimmed().remove('\"');
    
    m_Valid = true;
}

bool 
ThemeDescriptor::operator== (
        const ThemeDescriptor &other) const
{
    return m_Valid && other.m_Valid && m_CodeName == other.m_CodeName;
}

bool 
ThemeDescriptor::operator== (
        const QString &name) const
{
    return m_Valid && 
        (m_CodeName == name || m_Name == name);
}

ThemeDescriptor::~ThemeDescriptor ()
{
    if (m_DesktopEntry)
        delete m_DesktopEntry;
}

bool
ThemeDescriptor::isValid () const
{
    return m_Valid;
}

bool
ThemeDescriptor::isVisible () const
{
    return m_Visible;
}

QString
ThemeDescriptor::name() const
{
    return m_Name;
}

QString
ThemeDescriptor::codeName() const
{
    return m_CodeName;
}

QString
ThemeDescriptor::iconName() const
{
    return m_IconName;
}


