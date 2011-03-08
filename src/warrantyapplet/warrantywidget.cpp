/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
** Authors: David Kedves <dkedves@blumsoft.eu>
**          Laszlo Pere <lpere@blumsoft.eu>
**
****************************************************************************/

#include "warrantywidget.h"

#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MContainer>
#include <MSeparator>

//#define DEBUG
#include "../debug.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE (WarrantyWidget);

WarrantyWidget::WarrantyWidget (
        WarrantyBusinessLogic  *warrantyBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpStylableWidget (parent),
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
    layout->setContentsMargins (0,0,0,0);

    policy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    policy->setContentsMargins (0., 0., 0., 0.);
    policy->setSpacing (0.);

    // Add the title-bar
    addHeaderContainer (policy);

    // The label that shows the expiration date
    m_labelExpiration = new MLabel;
    m_labelExpiration->setObjectName ("WarrantyAppletExpirationLabel");
    m_labelExpiration->setStyleName ("CommonGroupHeaderInverted");
    m_labelExpiration->setWordWrap (true);

    // The label that shows the terms of the warranty
    m_labelTerms = new MLabel;
    m_labelTerms->setObjectName ("WarrantyAppletTermsLabel");
    m_labelTerms->setStyleName ("CommonBodyTextInverted");
    m_labelTerms->setWordWrap (true);
    
    /*
     * Adding the two labels with a new container
     */
    addLabelContainer (policy, m_labelExpiration, m_labelTerms);

    policy->addStretch ();

    retranslateUi ();

    layout->setPolicy (policy);
    setLayout (layout);
}

void 
WarrantyWidget::addStretcher (
        MLinearLayoutPolicy *mainLayout,
        const QString       &styleName)
{
    MSeparator *stretcher;

    Q_ASSERT (mainLayout);

    stretcher = new MSeparator ();
    stretcher->setStyleName (styleName);
    mainLayout->addItem (stretcher);
}

void 
WarrantyWidget::addHeaderContainer (
        MLinearLayoutPolicy *mainLayout)
{
    Q_ASSERT (mainLayout);

    /*
     * The label that we use as title.
     */
    //% "Warranty"
    m_TitleLabel = new MLabel (qtTrId("qtn_warr_title"));
    m_TitleLabel->setStyleName ("CommonApplicationHeaderInverted");

    /*
     * Adding the whole row to the main container.
     */
    mainLayout->addItem (m_TitleLabel);
    mainLayout->setStretchFactor (m_TitleLabel, 0);
}

void 
WarrantyWidget::addLabelContainer (
        MLinearLayoutPolicy *mainLayout,
        MLabel              *label1,
        MLabel              *label2)
{
    Q_ASSERT (mainLayout);

    /*
     * Adding the label to the layout.
     */
    mainLayout->addItem (label1);
    mainLayout->setAlignment (label1, Qt::AlignLeft);

    MSeparator *separator = new MSeparator;
    separator->setStyleName ("CommonSpacer");

    mainLayout->addItem (separator);

    mainLayout->addItem (label2);
    mainLayout->setAlignment (label2, Qt::AlignLeft);
}

/*
 * gconftool-2 -t string -s /meegotouch/i18n/language hu_HU
 */
void
WarrantyWidget::retranslateUi ()
{
    int expirationDays = m_WarrantyBusinessLogic->getExpirationDays ();

    /*
     * Please check NB#220856 for details. The translation is currently wrong,
     * but this code should be like this.
     */
    SYS_DEBUG ("*** expirationDays = %d", expirationDays);
    SYS_DEBUG ("*** text = '%s'", 
            SYS_STR(qtTrId ("qtn_warr_expiration", expirationDays)));
    SYS_DEBUG ("*** text = '%s'", 
            SYS_STR(qtTrId ("qtn_warr_expiration")));
    if (expirationDays > 0) {
      //% "Product warranty will expire in <b>%Ln</b> day."
      m_labelExpiration->setText (
              qtTrId ("qtn_warr_expiration", expirationDays).arg(expirationDays));
    } else if (expirationDays == 0) {
      //% "Product warranty is expired."
      m_labelExpiration->setText (qtTrId ("qtn_warr_expired"));
    } else {
      //% "Warranty timer error."
      m_labelExpiration->setText (qtTrId ("qtn_warr_error_timer"));
    }

    //% "(insert terms of warranty here)"
    m_labelTerms->setText (qtTrId ("qtn_warr_terms"));
}

