/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QPointer>
#include "dcpwidget.h"

class ThemeWidget : public DcpWidget
{
    Q_OBJECT

public:
    ThemeWidget (QGraphicsWidget *parent = 0);
    ~ThemeWidget ();

};

#endif
