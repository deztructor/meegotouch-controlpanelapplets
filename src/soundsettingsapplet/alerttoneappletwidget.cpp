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
#include "alerttoneappletwidget.h"

#include <QSet>
#include <MApplicationExtensionArea>
#include <MLabel>
#include <QGraphicsLinearLayout>
#include <MSeparator>
#include <MApplicationPage>
#include <QSystemDeviceInfo>
#include <QTimer>

using namespace QtMobility;

#include "soundsettingsutils.h"
#include "alerttonewidget.h"
#include "gconfstringcombo.h"
#include "profileintcombo.h"
#include "profiledatainterface.h"
#include "profilecontainer.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(AlertToneAppletWidget)

#include "../styles.h"

//#define DEBUG
#define WARNING
#include "../debug.h"

#include <time.h>

/******************************************************************************
 * Helper functions.
 */
static MWidgetController *
createEmptyContainer(
        QGraphicsWidget          *parent,
        QGraphicsLinearLayout   **layout)
{
    MWidgetController       *container =
        new MWidgetController (parent);

    container->setContentsMargins (0., 0., 0., 0.);

    (*layout) = new QGraphicsLinearLayout (Qt::Vertical);
    (*layout)->setContentsMargins (0., 0., 0., 0.);
    container->setLayout (*layout);

    return container;
}

static void
addSubTitle (
        QGraphicsWidget         *parent,
        QGraphicsLinearLayout   *targetLayout,
        const QString           &subTitle)
{
    MWidgetController       *container;
    QGraphicsLinearLayout   *layout;
    MLabel                  *label;
    MSeparator              *separator;

    container = new MWidgetController (parent);
    container->setContentsMargins (0., 0., 0., 0.);
    container->setStyleName (SUBTITLE_PANEL_STYLE_NAME);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);
    /*
     *
     */
    separator = new MSeparator;
    separator->setStyleName (SUBTITLE_DIVIDER_STYLE_NAME);

    /*
     *
     */
    label = new MLabel (subTitle);
    label->setStyleName (SUBTITLE_LABEL_STYLE_NAME);

    layout->addItem (separator);
    layout->setStretchFactor (separator, 2);

    layout->addItem (label);
    layout->setAlignment (label, Qt::AlignLeft);
    layout->setStretchFactor (label, 0);

    container->setLayout (layout);
    targetLayout->addItem (container);
}

static MLabel *
addTitleLabel (
        QGraphicsWidget         *parent,
        QGraphicsLinearLayout   *targetLayout,
        const char              *labelStyleName)
{
    Q_UNUSED (parent);
    MLabel                  *label;

    label = new MLabel;
    label->setStyleName (labelStyleName);

    targetLayout->addItem (label);
    return label;
}

/******************************************************************************
 * AlertToneAppletWidget implementation
 */

AlertToneAppletWidget::AlertToneAppletWidget (
        QList<AlertTone *>    alertTones,
        QGraphicsWidget      *parent):
    DcpStylableWidget (parent),
    m_alertTones(alertTones),
    m_ProfileIf (new ProfileDataInterface),
    m_tones (0),
    m_feedback (0)
{
    setContentsMargins (0., 0., 0., 0.);

    /*
     * This should be called later, not from the constructor.
     */
    createContents ();
}

AlertToneAppletWidget::~AlertToneAppletWidget ()
{
    /*
     * Removing the unused files. This is not the best place, but we have no
     * business-logic, so we do it here.
     */
    QSet<QString> files;
    for (int n = 0; n < m_alertTones.size(); ++n) {
        files += m_alertTones[n]->fileName();
    }

    SoundSettings::removeUnusedFiles (files);

    delete m_ProfileIf;
    m_ProfileIf = 0;

    delete m_volumeExtension;
    m_volumeExtension = 0;
}

void
AlertToneAppletWidget::delayedInit ()
{
    DEBUG_CLOCK_START;
    m_volumeExtension->init ();
    DEBUG_CLOCK_END("Initializing slider.");
}

void
AlertToneAppletWidget::createContents ()
{
    DEBUG_CLOCK_START;
    QGraphicsLinearLayout   *layout;
    MLabel                  *label;

    setContentsMargins (0., 0., 0., 0);
    layout = new QGraphicsLinearLayout (Qt::Vertical);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);

    setLayout (layout);

    /*
     * The main title
     */
#ifndef MEEGO
    label = addTitleLabel (
            this, layout,
            APP_TITLE_LABEL_STYLE_NAME);
    //% "Alert tones"
    label->setText(qtTrId("qtn_sond_sounds"));
#endif

    DEBUG_CLOCK_END("First widgets.");
    SYS_DEBUG ("%s: constructing volumeExtension", SYS_TIME_STR);
    /*
     * Try to add the Status-Menus volume/profile chooser widget here
     */
    m_volumeExtension = new MApplicationExtensionArea ("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0");
    m_volumeExtension->setInProcessFilter (QRegExp("/statusindicatormenu-volume.desktop$"));
    m_volumeExtension->setOutOfProcessFilter (QRegExp("$^"));
    m_volumeExtension->setObjectName ("VolumeExtensionArea");
    m_volumeExtension->setStyleName ("VolumeExtensionArea");
    // Moved to delayedInit for testing.
    //m_volumeExtension->init ();
    //QTimer::singleShot(500, this, SLOT(delayedInit()));

    layout->addItem (m_volumeExtension);
    SYS_DEBUG ("%s DONE: constructing volumeExtension", SYS_TIME_STR);
    DEBUG_CLOCK_END("volume extension");

    /*
     * A subtitle that shows 'Profile vibration'
     */
    addSubTitle (
            this, layout,
            //% "Vibration profile"
            qtTrId("qtn_prof_vibration"));

    /*
     * Add the profie vibration switches...
     */
    createProfileSwitches (layout, this);

    /*
     * A secondary title, it says 'Tones'
     */
    addSubTitle (
            this, layout,
            //% "Alert tones"
            qtTrId("qtn_sond_event_tones"));

    /*
     * A list with the sound file setting widgets.
     */
    m_tones = createAlertTonesList (this);
    layout->addItem (m_tones);

    /*
     * An other secondary title, that says 'Feedback'.
     */
    addSubTitle (
            this, layout,
            //% "Feedback"
            qtTrId("qtn_sond_feedback"));

    /*
     * The list with the feedback setting widgets.
     */
    m_feedback = createFeedbackList (this);
    layout->addItem (m_feedback);

    retranslateUi ();
    DEBUG_CLOCK_END("Whole content");
}

void
AlertToneAppletWidget::createProfileSwitches (
        QGraphicsLinearLayout   *layout,
        QGraphicsWidget         *parent)
{
    Q_UNUSED (parent);

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
                 this, SLOT (vibrationChanged(bool)));

        layout->addItem (widget);
    }
}

void
AlertToneAppletWidget::vibrationChanged (
        bool enabled)
{
    //NOTE: MButton->isChecked() method returns the state before the
    // press at this point
    ProfileContainer *profile =
        static_cast<ProfileContainer*> (this->sender ());

    m_ProfileIf->setVibration (profile->id (), enabled);
}

MWidgetController *
AlertToneAppletWidget::createFeedbackList(
        QGraphicsWidget *parent)
{
    MWidgetController       *container;
    QGraphicsLinearLayout   *layout;

    container = createEmptyContainer (parent, &layout);

    ProfileIntCombo *picombo = 0;

    QSystemDeviceInfo devInfo;
    QSystemDeviceInfo::KeyboardTypeFlags keybFlags = devInfo.keyboardTypes ();

    /*
     * Show the keyboard tones only if the device have hardware keyboard
     */
    if ((keybFlags & QSystemDeviceInfo::FlipKeyboard) ||
        (keybFlags & QSystemDeviceInfo::FullQwertyKeyboard) ||
        (keybFlags & QSystemDeviceInfo::HalfQwertyKeyboard) ||
        (keybFlags & QSystemDeviceInfo::ITUKeypad))
    {
        picombo = new ProfileIntCombo (
            "keypad.sound.level", true,
            container);

        picombo->setObjectName("ProfileIntCombo_keypad.sound.level");
        picombo->setStyleName ("CommonComboBoxInverted");
        layout->addItem (picombo);
    }

    picombo = new ProfileIntCombo ("system.sound.level", true, container);
    picombo->setObjectName("ProfileIntCombo_system.sound.level");
    picombo->setStyleName ("CommonComboBoxInverted");
    layout->addItem (picombo);

    GConfStringCombo *combo = new GConfStringCombo(
        "/meegotouch/input_feedback/volume/priority2/pulse",
        QStringList() << "off" << "low" << "medium" << "high",
        container);
    combo->setObjectName("GConfStringCombo_pulse");
    combo->setStyleName ("CommonComboBoxInverted");
    layout->addItem (combo);

    combo = new GConfStringCombo(
        "/meegotouch/input_feedback/volume/priority2/vibra",
        QStringList() << "off" << "low" << "medium" << "high",
        container);
    combo->setObjectName("GConfStringCombo_vibra");
    combo->setStyleName ("CommonComboBoxInverted");
    layout->addItem (combo);

    container->setObjectName("MConcreateAlertTonePreviewtainer_feedback");
    container->setStyleName ("CommonLargePanelInverted");
    return container;
}


MWidgetController *
AlertToneAppletWidget::createAlertTonesList (QGraphicsWidget *parent)
{
    MWidgetController       *container;
    QGraphicsLinearLayout   *layout;
    AlertToneWidget         *alertToneWidget;

    container = createEmptyContainer (parent, &layout);

    /*
     * And then the list...
     */
    for (int i = 0; i < m_alertTones.size (); i++)
    {
        alertToneWidget = new AlertToneWidget (
                m_alertTones[i], i, container);
        alertToneWidget->setObjectName (
                "AlertToneWidget_" + m_alertTones[i]->key());

        // connect the widgets showWidget signal
        // If we send the changeWidget() signal, DCP will show the widget in an
        // MApplicationPage, if we send the showWidget, SoundSettingsApplet will
        // show the widget in a sheet.
        connect (alertToneWidget, SIGNAL (showWidget (int)),
                 this, SIGNAL (showWidget (int)));

        layout->addItem (alertToneWidget);
    }

    container->setObjectName ("MWidgetController_tones");
    container->setStyleName ("CommonLargePanelInverted");
    return container;
}


/*
 * This virtual method is called when the MApplicationPage for the widget is
 * already there, so we can initialize it.
 */
void
AlertToneAppletWidget::polishEvent ()
{
    DEBUG_CLOCK_START;

    QGraphicsWidget  *parent;
    MApplicationPage *page = 0;

    // testing 
    //m_volumeExtension->init ();
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

    if (page)
        page->setComponentsDisplayMode (
                MApplicationPage::HomeButton,
                MApplicationPageModel::Hide);
    
    QTimer::singleShot(2000, this, SLOT(delayedInit()));
    DEBUG_CLOCK_END("polish event");
}

