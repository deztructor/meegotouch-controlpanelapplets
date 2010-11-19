/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _ALERT_TONE_BROWSER_H_
#define _ALERT_TONE_BROWSER_H_

#include <MAction>
#include <MContainer>
#include <QStack>
#include <MContentItem>
#include <QPointer>
#include <SelectSingleContentItemPage.h>

#include "alerttone.h"
#include "alerttonedefaults.h"
#include "alerttonetoplevel.h"

class MTextEdit;
class QGraphicsLinearLayout;
class MCustomContentItem;
class MButton;
class DrillDownItem;


/*!
 * A compound widget that holds two buttons and a list widget. The two buttons
 * can be used to pick a sound file using a content picker page and load a sound
 * file from the OVI store. The list contains the default sound files, a list
 * that is created reading some specific directories of the file system.
 */
class AlertToneBrowser : public AlertToneToplevel
{
	Q_OBJECT

public:
	AlertToneBrowser(AlertTone *tone, QGraphicsWidget *parent = 0);
	~AlertToneBrowser();

private slots:
	void defaultItemClicked (const QString &filename);
    void defaultsDisplayEntered();
	void setAlertTone (
            const QString &fname,
            bool           setGui = false);
	void launchMusicBrowser();
	void launchOviStore();
	void selectingMusicItem(const QString &item);
	void cancel();
	void accept();
	void currentClicked();
    void textChanged ();
    void browserBackButtonClicked ();

protected:
	virtual void retranslateUi();
    virtual void polishEvent ();
    
private:
    void startPlayingSound (const QString &filename);
    void stopPlayingSound ();
	virtual void createContent();

	AlertTone *m_tone;
	MCustomContentItem *m_current;
	DrillDownItem              *m_my_music;
	DrillDownItem              *m_ovi_store;

    QGraphicsLinearLayout      *m_MainLayout;
	AlertToneDefaults          *m_defaults;
    MTextEdit                  *m_LiveFilterEditor;
	QString                     currSelectedFile;
	QPointer<AlertTonePreview>            m_preview;
    QPointer<SelectSingleContentItemPage> m_MusicBrowser;

    MButton                    *m_DoneButton;
    MButton                    *m_CancelButton;

#ifdef UNIT_TEST
    friend class Ut_AlertToneBrowserTests;
#endif
};

#endif /* !_ALERT_TONE_BROWSER_H_ */
