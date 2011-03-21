/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
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

#include <QDebug>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MImageWidget>
#include <MLabel>
#include <MSeparator>
#include <MApplicationPage>
#include <QTimer>

#include "alerttoneappletwidget.h"
#include "alerttonewidget.h"
#include "gconfstringcombo.h"
#include "profileintcombo.h"
#include "alerttonevolume.h"
#include "profiledatainterface.h"
#include "profilecontainer.h"

#define DEBUG
#define WARNING
#include "../debug.h"

/******************************************************************************
 * Helper functions.
 */
static MContainer *
createEmptyContainer(
        QGraphicsWidget      *parent, 
        MLinearLayoutPolicy **p_policy, 
        QGraphicsWidget     **p_centralWidget)
{
	MContainer *container = new MContainer (parent);
	MLayout    *layout;

    container->setContentsMargins (0., 0., 0., 0.);
    container->setHeaderVisible (false);
	(*p_centralWidget) = container->centralWidget();

	layout = new MLayout((*p_centralWidget));
    layout->setContentsMargins (0., 0., 0., 0.);

	(*p_policy) = new MLinearLayoutPolicy(layout, Qt::Vertical);
	layout->setLandscapePolicy((*p_policy));
	layout->setPortraitPolicy((*p_policy));

	return container;
}

/******************************************************************************
 * AlertToneAppletWidget implementation
 */
AlertToneAppletWidget::AlertToneAppletWidget (
        QList<AlertTone *>    alertTones, 
        QGraphicsWidget      *parent):
	AlertToneToplevel (parent),
	m_alertTones(alertTones),
    m_ProfileIf (new ProfileDataInterface),
	m_tones (0),
	m_feedback (0),
    m_Title (0),
    m_EventTonesLabel (0),
    m_FeedbackLabel (0)
{
    /*
     * This should be called later, not from the constructor.
     */
    createContents ();
}

AlertToneAppletWidget::~AlertToneAppletWidget ()
{
    delete m_ProfileIf;
}

void
AlertToneAppletWidget::createContents()
{
	QGraphicsWidget       *centralWidget = this/*->centralWidget()*/;
	MLayout               *mainLayout;
	MLinearLayoutPolicy   *policy;
    MSeparator            *spacer;

	mainLayout = new MLayout (centralWidget);

	policy = new MLinearLayoutPolicy(mainLayout, Qt::Vertical);
    policy->setContentsMargins (0., 0., 0., 0.);
    policy->setSpacing (0.);
	mainLayout->setLandscapePolicy(policy);
	mainLayout->setPortraitPolicy(policy);

    /*
     * The main title
     */
    m_Title = addTitleLabel (
            centralWidget, policy,
            "CommonApplicationHeaderInverted");

    /*
     * Adding a spacer.
     */
    spacer = new MSeparator;
    spacer->setStyleName ("CommonHeaderDividerInverted");
    policy->addItem (spacer);

    /*
     * Well, this is not the profile slider, that one is not implemented yet.
     */
    AlertToneVolume *slider;
    slider = new AlertToneVolume (centralWidget);
    policy->addItem (slider);

    /*
     * A subtitle that shows 'Profile vibration'
     */
    m_VibrationLabel = addTitleLabel (
            centralWidget, policy, 
            "CommonHeaderPanelInverted", 
            "CommonGroupHeaderInverted");
    /*
     *
     */
    createProfileSwitches (policy, centralWidget);

    /*
     * A secondary title, it says 'Tones'
     */
    m_EventTonesLabel = addTitleLabel (
            centralWidget, policy, 
            "CommonHeaderPanelInverted", 
            "CommonGroupHeaderInverted");

    /*
     * A list with the sound file setting widgets.
     */
	m_tones = createAlertTonesList(centralWidget);
	policy->addItem(m_tones);

    /*
     * Adding a spacer.
     */
    spacer = new MSeparator;
    spacer->setStyleName ("CommonHeaderDividerInverted");
    policy->addItem (spacer);
    
    /*
     * An other secondary title.
     */
    m_FeedbackLabel = addTitleLabel (
            centralWidget, policy, 
            "CommonHeaderPanelInverted", 
            "CommonGroupHeaderInverted");

    /*
     * The list with the feedback setting widgets.
     */
	m_feedback = createFeedbackList(centralWidget);
	policy->addItem(m_feedback);

	retranslateUi();
}
        
void 
AlertToneAppletWidget::createProfileSwitches (
        MLinearLayoutPolicy   *policy,
        QGraphicsWidget       *parent)
{
    QList<ProfileDataInterface::ProfileData> profileDataList = 
        m_ProfileIf->getProfilesData();

    for (int i = 0; i < profileDataList.size(); ++i) {
        ProfileDataInterface::ProfileData  d = profileDataList[i];
        ProfileContainer                  *widget;
        
        widget = new ProfileContainer (
                d.profileId, d.profileName, d.vibrationEnabled);

        // For testability driver: set some object name...
        widget->setObjectName (ProfileDataInterface::mapId (d.profileId));

        connect (widget, SIGNAL (toggled(bool)), 
                widget, SLOT (vibrationChanged(bool)));

        policy->addItem(widget);
    }

}

MContainer *
AlertToneAppletWidget::createFeedbackList(
        QGraphicsWidget *parent)
{
	MContainer *container;
	MLinearLayoutPolicy *policy;
	QGraphicsWidget *centralWidget;

	container = createEmptyContainer(parent, &policy, &centralWidget);

	ProfileIntCombo *picombo = new ProfileIntCombo(
		"keypad.sound.level", true,
		centralWidget);
	picombo->setObjectName("ProfileIntCombo_keypad.sound.level");
    picombo->setStyleName ("CommonComboBoxInverted");
	policy->addItem(picombo);

	picombo = new ProfileIntCombo(
		"system.sound.level", true,
		centralWidget);
	picombo->setObjectName("ProfileIntCombo_system.sound.level");
    picombo->setStyleName ("CommonComboBoxInverted");
	policy->addItem(picombo);

	GConfStringCombo *combo = new GConfStringCombo(
		"/meegotouch/input_feedback/volume/priority2/pulse",
		QStringList() << "off" << "low" << "medium" << "high",
		centralWidget);
	combo->setObjectName("GConfStringCombo_pulse");
    combo->setStyleName ("CommonComboBoxInverted");
	policy->addItem(combo);

	combo = new GConfStringCombo(
		"/meegotouch/input_feedback/volume/priority2/vibra",
		QStringList() << "off" << "low" << "medium" << "high",
		centralWidget);
	combo->setObjectName("GConfStringCombo_vibra");
    combo->setStyleName ("CommonComboBoxInverted");
	policy->addItem(combo);

	container->setObjectName("MConcreateAlertTonePreviewtainer_feedback");
    container->setStyleName ("CommonLargePanelInverted");
	return container;
}


MContainer *
AlertToneAppletWidget::createAlertTonesList(QGraphicsWidget *parent)
{
	MContainer *container;
	MLinearLayoutPolicy *policy;
	QGraphicsWidget *centralWidget;
	AlertToneWidget *alertToneWidget;

	container = createEmptyContainer(parent, &policy, &centralWidget);


    /*
     * And then the list...
     */
	for (int Nix = 0; Nix < m_alertTones.size(); Nix++) {
		alertToneWidget = new AlertToneWidget (
                m_alertTones[Nix], Nix, this, centralWidget);
		alertToneWidget->setObjectName (
                "AlertToneWidget_" + m_alertTones[Nix]->key());
		policy->addItem(alertToneWidget);
	}

	container->setObjectName("MContainer_tones");
    container->setStyleName ("CommonLargePanelInverted");
	return container;
}

void
AlertToneAppletWidget::retranslateUi()
{
    if (m_VibrationLabel)
        //% "Profile vibration"
        m_VibrationLabel->setText (qtTrId("qtn_sond_missing_logical_id2"));

    if (m_EventTonesLabel)
        //% "Event Tones"
        m_EventTonesLabel->setText (qtTrId("qtn_sond_event_tones"));

    if (m_FeedbackLabel)
        //% "Feedback"
        m_FeedbackLabel->setText(qtTrId("qtn_sond_feedback"));

    if (m_Title)
        //% "Sounds"
        m_Title->setText(qtTrId("qtn_sond_sounds"));
}

/*
 * This virtual method is called when the MApplicationPage for the widget is
 * already there, so we can initialize it. 
 */
void
AlertToneAppletWidget::polishEvent ()
{
    QGraphicsWidget  *parent;
    MApplicationPage *page = 0;
   
    /*
     * We need to find the MApplicationPage among our parents.
     */
    parent = parentWidget();
    while (parent) {
        page = qobject_cast <MApplicationPage *>(parent);
        if (page)
            break;
        parent = parent->parentWidget();
    }

    if (!page)
        return;

    /*
     * Hiding the home button. 
     */
    page->setComponentsDisplayMode (
            MApplicationPage::HomeButton,
            MApplicationPageModel::Hide);
}

