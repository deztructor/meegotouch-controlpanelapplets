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
    QString labelText ();
    QString licenseText ();

private slots:
    void refresh ();

protected:
    virtual void retranslateUi ();

private:
    QPointer<AboutBusinessLogic>    m_AboutBusinessLogic;
    MLabel                         *m_InfoLabel;
    MLabel                         *m_LicenseLabel;
    #ifdef UNIT_TEST
    friend class Ut_AboutApplet;
    #endif
};

#endif
