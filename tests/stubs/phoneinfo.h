/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef PHONEINFO_H
#define PHONEINFO_H
#define PHONEINFO_STUBBED_H

#include <QObject>

#define FAKE_IMEI_NUMBER "FakeImeiNumber"

class QString;

namespace Cellular
{
    class PhoneInfo : public QObject {
        Q_OBJECT
        public:
            QString imeiNumber() const;
    };
}
#endif
