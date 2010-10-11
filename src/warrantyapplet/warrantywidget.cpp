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

#include "warrantywidget.h"

#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>

#undef DEBUG
#include "../debug.h"

WarrantyWidget::WarrantyWidget (
        WarrantyBusinessLogic  *warrantyBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WarrantyBusinessLogic (warrantyBusinessLogic)
{
    createContent ();
}

WarrantyWidget::~WarrantyWidget ()
{
}


void
WarrantyWidget::createContent ()
{
    MLayout             *layout;
    MLinearLayoutPolicy *policy;

    layout = new MLayout;
    policy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    m_labelExpiration = new MLabel;
    policy->addItem (m_labelExpiration);

    m_labelTerms = new MLabel;
    m_labelTerms->setWordWrap (true);
    policy->addItem (m_labelTerms);

    policy->addStretch ();

    retranslateUi ();

    layout->setPolicy (policy);
    setLayout (layout);
}

void
WarrantyWidget::retranslateUi ()
{
    int expirationDays = m_WarrantyBusinessLogic->getExpirationDays ();

    if (expirationDays > 0)
    {
      //% "Product warranty will expire in <b>%L1</b> day."
      m_labelExpiration->setText (qtTrId ("qtn_warr_expiration").arg (expirationDays));
    }
    else if (expirationDays == 0)
    {
      //% "Product warranty is expired."
      m_labelExpiration->setText (qtTrId ("qtn_warr_expired"));
    }
    else
    {
      //% "Warranty timer error."
      m_labelExpiration->setText (qtTrId ("qtn_warr_error_timer"));
    }

    //% "(insert terms of warranty here)"
    m_labelTerms->setText (qtTrId ("qtn_warr_terms"));
}

