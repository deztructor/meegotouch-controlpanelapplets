/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEDIALOG_H
#define THEMEDIALOG_H

#include <DuiDialog>
#include <QPointer>

class ThemeBusinessLogic;
class QString;

class ThemeDialog: public DuiDialog
{
Q_OBJECT

public:
    ThemeDialog (
            ThemeBusinessLogic *themeBusinessLogic,
            const QString      &themeName);
    void showDialog ();

private:
    QPointer<ThemeBusinessLogic> m_ThemeBusinessLogic;
    QString                      m_ThemeName;
};

#endif
