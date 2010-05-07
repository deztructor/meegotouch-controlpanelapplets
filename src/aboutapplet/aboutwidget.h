/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QPointer>
#include "dcpwidget.h"

#include "aboutbusinesslogic.h"

class MLabel;

class AboutWidget : public DcpWidget
{
    Q_OBJECT

public:
    AboutWidget (
            AboutBusinessLogic     *aboutBusinessLogic, 
            QGraphicsWidget        *parent = 0);
    ~AboutWidget ();

    void createContent ();
    QString labelText();

public slots:
    void dataReady ();

protected:
    virtual void retranslateUi ();

private:
    QPointer<AboutBusinessLogic>    m_AboutBusinessLogic;
    MLabel                         *m_Label1;
};

#endif
