/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QPointer>
#include "dcpwidget.h"

class MContainer;
class MGridLayoutPolicy;

class TestWidget : public DcpWidget
{
    Q_OBJECT

public:
    TestWidget (
            int                 numId, 
            QGraphicsWidget    *parent = 0);
    ~TestWidget ();

    void retranslateUi ();

private slots:
    void pagingClicked ();
    void backClicked ();

private:
    void createWidgets ();
    MContainer *createContainer ();
    void createButtons ();

    MContainer                 *m_LocalContainer;
    MGridLayoutPolicy          *m_LocalLayoutPolicy;
    int                           m_NumId;
};

#endif
