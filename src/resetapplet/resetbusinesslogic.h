/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef RESETBUSINESSLOGIC_H
#define RESETBUSINESSLOGIC_H

#include <QList>
#include <QObject>

#include <QPointer>

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
