/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef RESETBUSINESSLOGIC_H
#define RESETBUSINESSLOGIC_H

#include <QList>
#include <QObject>

#include <QPointer>

/*!
 * Provides the 'restore factory settings' and 'clear user data' operations for
 * the reset applet. These operations are implemented in the 'clean-device'
 * package, so one might want to install them:
 *
 * sudo apt-get install clean-device
 *
 * (This package is added to the dependency list of the
 * duicontrolpanel-resetapplet package.) The class will execute the following
 * shell command lines to perform the operations.
 * 
 * /usr/sbin/clean-device.sh --rfs
 * /usr/sbin/clean-device.sh --cud
 *
 * TODO: To implement the screen lock pin code query. Some info:
 * (1) apt-get install libdevlock1
 * (2) git clone git@dvcs.projects.maemo.org:security/duicontrolpanel-devicelockapplet
 * (3) libqmsystem Maemo::QmLocks Class Reference
 */
class ResetBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ResetBusinessLogic ();
    ~ResetBusinessLogic ();

public slots:
    void performRestoreSettings ();
    void performClearData ();
};

#endif
