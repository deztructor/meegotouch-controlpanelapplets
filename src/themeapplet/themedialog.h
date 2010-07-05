/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEDIALOG_H
#define THEMEDIALOG_H

#include <QObject>
#include <MDialog>
#include <QPointer>

class ThemeBusinessLogic;
class ThemeDescriptor;
class QString;
class MButton;

class ThemeDialog: public MDialog
{
Q_OBJECT

public:
    ThemeDialog (
            ThemeBusinessLogic *themeBusinessLogic,
            ThemeDescriptor    *themeDesc);
    void showDialog ();

private slots:
    void acceptClicked ();
    void cancelClicked ();
    void themeChanged (QString themeId);

private:
    QPointer<ThemeBusinessLogic>  m_ThemeBusinessLogic;
    ThemeDescriptor              *m_ThemeDesc;
    MButton                      *m_SelectButton;
    MButton                      *m_CancelButton;
    #ifdef UNIT_TEST
    friend class Ut_ThemeDialog;
    #endif
};

#endif
