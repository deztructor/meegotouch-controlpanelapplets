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

#include "ut_displaywidget.h"
#include "displaywidget.h"

#include "qmdisplaystate.h"
#ifndef QMDISPLAYSTATE_STUB_H
#  error "QmDisplayState should be stubbed, can nopt continue."
#endif

#include "displaybusinesslogic.h"

#include <MApplication>
#include <MGConfItem>
#include <MSlider>
#include <MContentItem>
#include <MBasicListItem>
#include <MDialog>
#include <MPopupList>
#include <MButton>
#include <MLabel>

#include <QList>
#include <QVariant>

#define DEBUG
#include "../../src/debug.h"

static bool     AcceptDialogs = false;
static int      SelectIndex = 0;
static QString  LastDialogTitle;
int
MDialog::exec (
        MWindow *window)
{
    Q_UNUSED(window);

    MPopupList         *popuplist = qobject_cast<MPopupList *>(this);
    QAbstractItemModel *model;
    QModelIndex         index;

    SYS_DEBUG ("*** this      = %p", this);
    SYS_DEBUG ("*** title     = %s", SYS_STR(title()));
    SYS_DEBUG ("*** popuplist = %p", popuplist);
    LastDialogTitle = title();
    if (!popuplist)
        goto finalize;

    model = popuplist->itemModel();
    index = model->index (SelectIndex, 0);
    popuplist->setCurrentIndex (index);
    
finalize:
    return AcceptDialogs ? MDialog::Accepted : MDialog::Rejected;
}

/******************************************************************************
 * Stubbing the MGConfItem.
 */
static bool dimTimeoutsExist = true;

QVariant
MGConfItem::value () const
{
    SYS_DEBUG ("*** key() = %s", SYS_STR(key()));

    if (key() == "/meegotouch/debug/show_async_requests")
        return QVariant (true);

    if (key() == "/meegotouch/target/name")
        return QVariant ("Default");

    if (key() != "/system/osso/dsm/display/possible_display_dim_timeouts") {
        SYS_DEBUG ("%s ???", SYS_STR(key()));
        goto return_invalid;
    }

    if (dimTimeoutsExist) {
        QList<QVariant> list;

        list << 
            QVariant (15) << 
            QVariant (30) << 
            QVariant (60) << 
            QVariant (120) <<
            QVariant (180);

        return QVariant (list);
    }

return_invalid:
    return QVariant ();
}

/******************************************************************************
 * Ut_DisplayWidget implementation. 
 */
void 
Ut_DisplayWidget::init()
{
}

void 
Ut_DisplayWidget::cleanup()
{
}


int argc = 1;
char* app_name = (char*) "./Ut_DisplayWidget";

void 
Ut_DisplayWidget::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
}

void 
Ut_DisplayWidget::cleanupTestCase()
{
    delete m_App;
}

/*!
 * This test will move the brightness slider through the possible brightness
 * values and check if the slider is updating the brightness with the
 * DisplayBusinessLogic.
 */
void 
Ut_DisplayWidget::testBrightnessSlider ()
{
    DisplayWidget *widget;
    QList<int>     brightnessValues;

    widget = new DisplayWidget;
    QVERIFY (widget->m_logic);
    QVERIFY (widget->m_brightnessSlider);
    brightnessValues = widget->m_logic->brightnessValues();

    for (int n = 0; n < brightnessValues.size(); ++n) {
        widget->m_brightnessSlider->setValue (n);
        QVERIFY (widget->m_logic->selectedBrightnessValueIndex() == n);
    }

    delete widget;
}


/*!
 * This test will check that when the screen timeout widget is clicked a dialog
 * will be shown with the possible values and when a value is selected the
 * screen light value has been set for the businesslogic.
 */
void 
Ut_DisplayWidget::testScreenTimeout ()
{
    DisplayWidget *widget;
    int            screenLightsValue;

    widget = new DisplayWidget;
    QVERIFY (widget->m_logic);
    QVERIFY (widget->m_screenTimeout);
   
    /*
     * Go through the values by accepting every one of them.
     */
    AcceptDialogs = true;
    for (SelectIndex = 0; SelectIndex <= 4; ++SelectIndex) {
        LastDialogTitle = "";
        widget->m_screenTimeout->click();
        QVERIFY (LastDialogTitle == "qtn_disp_screenoff");

        screenLightsValue = widget->m_logic->selectedScreenLightsValue ();
        SYS_DEBUG ("*** screenLightsValue = %d", screenLightsValue);
        QVERIFY (screenLightsValue == SelectIndex);
    }

    /*
     * Then the same with rejecting the dialog.
     */
    AcceptDialogs = false;
    for (SelectIndex = 0; SelectIndex <= 4; ++SelectIndex) {
        LastDialogTitle = "";
        widget->m_screenTimeout->click();
        QVERIFY (LastDialogTitle == "qtn_disp_screenoff");

        screenLightsValue = widget->m_logic->selectedScreenLightsValue ();
        SYS_DEBUG ("*** screenLightsValue = %d", screenLightsValue);
        QVERIFY (screenLightsValue == 4);
    }

    delete widget;
}

/*!
 * This test will check if the blank inhibit button is properly initialized and
 * it is actually setting the value in the DisplayBusinessLogic class.
 */
void 
Ut_DisplayWidget::testBlankInhibit ()
{
    DisplayWidget *widget;

    widget = new DisplayWidget;

    QVERIFY (widget->m_logic);
    QVERIFY (widget->m_blankInhibitButton);
    QVERIFY (widget->m_blankInhibitButton->isChecked() == 
            widget->m_logic->blankInhibitValue ());

    widget->m_blankInhibitButton->click();
    QVERIFY (widget->m_blankInhibitButton->isChecked() == 
            widget->m_logic->blankInhibitValue ());
}

void 
Ut_DisplayWidget::testTranslation ()
{
    DisplayWidget *widget;

    widget = new DisplayWidget;
    widget->retranslateUi ();

    // FIXME: Should stub the qtTrId() method.
    QVERIFY (widget->m_screenTimeout->title() == "qtn_disp_screenoff");
    QVERIFY (widget->m_brightnessLabel->text() == "qtn_disp_bright");
    QVERIFY (widget->m_blankInhibitLabel->text() == "qtn_disp_screenon");
}

QTEST_APPLESS_MAIN(Ut_DisplayWidget)
