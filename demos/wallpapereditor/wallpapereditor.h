/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef TEST_H
#define TEST_H

#include <QObject>

class MyApplication : public QObject {
    Q_OBJECT
    public:
        MyApplication ();
};
#endif
