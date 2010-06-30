/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef RESETWIDGET_H
#define RESETWIDGET_H

#include <QPointer>
#include "dcpwidget.h"

#include "resetbusinesslogic.h"

class ResetWidget : public DcpWidget
{
    Q_OBJECT

public:
    ResetWidget (
            ResetBusinessLogic     *resetBusinessLogic, 
            QGraphicsWidget        *parent = 0);
    ~ResetWidget ();

    void createContent ();

private slots:
    void restoreActivated ();
    void clearActivated ();

private:
    QPointer<ResetBusinessLogic>    m_ResetBusinessLogic;
    #ifdef UNIT_TEST
    friend class Ut_ResetApplet;
    #endif
};

#endif
