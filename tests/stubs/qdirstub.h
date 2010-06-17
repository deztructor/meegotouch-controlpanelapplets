/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef __QDIRSTUB_FOR_THEME__
#define __QDIRSTUB_FOR_THEME__

#include "mdesktopentrystub.h"

QStringList 
entryList (
        const QDir &dir, 
        QDir::Filters filters) 
{
    MTheme       *theme = MTheme::instance();
    QString       themeName = theme->currentTheme();
    QStringList   retval;

    Q_UNUSED (dir);
    Q_UNUSED (filters);
    //SYS_DEBUG ("*** %s", SYS_STR(dir.absolutePath()));
    retval << 
	    // These are for testing if the filtering works in the
	    // ThemeBusinessLogic class.
	    "." <<
	    ".." <<
	    // These are the fake theme names from the mdesktopentrystub 
	    // test database.
	    CODENAMEDesktopFileMissing <<
	    CODENAMEDesktopFilePerfect <<
	    CODENAMEDesktopFileHidden <<
	    CODENAMEDesktopFileEmpty <<
	    // And this is the current theme name. The mdesktopentrystub will
	    // handle this, it also reads the theme name from Meego.
	    themeName;

    #if 0
    int           n;
    n = 0;
    foreach (QString debugString, retval) {
        SYS_DEBUG ("retval[%d] = %s", n, SYS_STR(debugString));
	++n;
    }
    #endif

    return retval;
}
#endif
