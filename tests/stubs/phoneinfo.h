/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
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
