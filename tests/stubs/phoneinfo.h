/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef PHONEINFO_H
#define PHONEINFO_H
#define PHONEINFO_STUBBED_H

#ifndef UNIT_TEST
#  error "This header should be used only in unit tests."
#endif
#ifdef FUNCTIONAL_TEST
#  error "This header should not be used in a functional test."
#endif

#include <QObject>
#define FAKE_IMEI_NUMBER "FakeImeiNumber"

class QString;

namespace Cellular
{
    class PhoneInfo : public QObject {
        Q_OBJECT
        public:
            PhoneInfo ();
            ~PhoneInfo ();
            QString imeiNumber() const;
    };
}
#endif
