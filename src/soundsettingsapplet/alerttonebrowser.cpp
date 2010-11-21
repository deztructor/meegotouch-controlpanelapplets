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

#include "alerttonebrowser.h"
#include "alerttoneappletmaps.h"
#include "trackerconnection.h"
#include "drilldownitem.h"

#include <stdlib.h>

#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QtTracker/Tracker>
#include <MTheme>
#include <MLayout>
#include <MList>
#include <MListFilter>
#include <QGraphicsLinearLayout>
#include <MAction>
#include <MTextEdit>
#include <MButton>
#include <MWidgetAction>
#include <MImageWidget>

#define M_UNDO_PROPERTY "enabled"

/*
 * Temporary macro to test the contentpicker under scrachbox where it does not
 * work. :-(
 */
//#define EMULATE_PICKER

#define DEBUG
#define WARNING
#include "debug.h"

static const int filterEditorPosition = 2;


struct  MCustomContentItem: public MContentItem
{
	QString fullPath;
	MCustomContentItem (MContentItem::ContentItemStyle itemStyle=MContentItem::IconAndTwoTextLabels, QGraphicsItem *parent=0)
	:MContentItem(itemStyle,parent),fullPath("") {}
};

/******************************************************************************
 * AlertToneBrowser implementation.
 */
AlertToneBrowser::AlertToneBrowser(AlertTone *tone, QGraphicsWidget *parent):
	AlertToneToplevel(parent),
	m_tone(tone),
	m_current(NULL),
	m_defaults(NULL),
	m_preview(NULL),
    m_DoneButton (0),
    m_CancelButton (0)
{
    /*
     * FIXME: Why do we need to set the title?
     */
	setProperty ("title", AlertToneAppletMaps::mapToUiString(m_tone->key()));
#ifndef RUN_STANDALONE
	createContent();
#endif /* !RUN_STANDALONE */
}

AlertToneBrowser::~AlertToneBrowser()
{
    SYS_DEBUG ("");
    stopPlayingSound ();
}

void
AlertToneBrowser::createContent()
{
#ifdef RUN_STANDALONE
	QGraphicsWidget *centralWidget = this->centralWidget();
#else /* !RUN_STANDALONE */
	QGraphicsWidget *centralWidget = this/*->centralWidget()*/;
#endif /* RUN_STANDALONE */

    m_MainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    m_MainLayout->setContentsMargins (0., 0., 0., 0.);
    m_MainLayout->setSpacing (0.);
    centralWidget->setLayout (m_MainLayout);
	
    //% "Pick from My Music"
	m_my_music = new DrillDownItem;
    m_my_music->setLayoutPosition (M::VerticalTopPosition);
	m_my_music->imageWidget()->setImage("icon-l-music");
	m_my_music->setObjectName("MContentItem_pickFromMyMusic");
    m_MainLayout->addItem (m_my_music);
	connect(m_my_music, SIGNAL(clicked()), 
            this, SLOT(launchMusicBrowser()));

	//% "Get more from Ovi store"
	m_ovi_store = new DrillDownItem;
    m_ovi_store->setLayoutPosition (M::VerticalBottomPosition);
	m_ovi_store->imageWidget()->setImage("icon-m-common-ovi");
	m_ovi_store->setObjectName("MContentItem_getMoreFromOviStore");
    m_MainLayout->addItem (m_ovi_store);
	connect (m_ovi_store, SIGNAL(clicked()), 
            this, SLOT(launchOviStore()));

	//% "Current tone"
	m_current = new MCustomContentItem(MContentItem::TwoTextLabels, centralWidget);
	m_current->setObjectName("AlertToneCurrent");
	connect(m_current, SIGNAL(clicked()), 
            this, SLOT(currentClicked()));

    /*
     * The list with the available sound files.
     */
	m_defaults = new AlertToneDefaults(m_tone, 0);
    m_defaults->filtering()->setEnabled (true);
    m_defaults->filtering()->setFilterMode (
            MListFilter::FilterAsBeginningOfLine);
    m_LiveFilterEditor = m_defaults->filtering()->editor();
	m_MainLayout->addItem(m_defaults);
	connect(m_defaults, SIGNAL(displayEntered()), 
            this, SLOT(defaultsDisplayEntered()));
    
    // We need this stretch to keep the widgets growing in size when too much
    // lines are filtered out from the list.
    m_MainLayout->addStretch ();
		
	retranslateUi();
    
    m_defaults->selectAndScroll (m_tone->fileName(), m_tone->niceName());

    connect (m_LiveFilterEditor, SIGNAL(textChanged()),
            this, SLOT(textChanged ()));

	connect(m_defaults, SIGNAL(defaultItemClicked(const QString &)), 
            this, SLOT(defaultItemClicked(const QString &)));	
}


void
AlertToneBrowser::defaultsDisplayEntered()
{
    /*
     * A fix for the NB#198788 - Live filtering text editor loses focus in 
     * this scenario
     */
    if (!m_LiveFilterEditor || !m_LiveFilterEditor->isOnDisplay())
    	m_defaults->setFocus();
}

void
AlertToneBrowser::retranslateUi()
{
	m_my_music->setProperty ("title", qtTrId("qtn_sond_pick_music"));
	m_ovi_store->setProperty("title", qtTrId("qtn_sond_store"));

    if (m_DoneButton)
        //% "Done"
        m_DoneButton->setText (qtTrId("qtn_comm_command_done"));

    if (m_CancelButton)
        //% "Cancel"
        m_CancelButton->setText (qtTrId("qtn_comm_cancel"));
}

void 
AlertToneBrowser::cancel()
{
	emit closePage();
}

void
AlertToneBrowser::accept()
{
    if (!currSelectedFile.isEmpty())
        m_tone->set(currSelectedFile);

    emit closePage();
}

void
AlertToneBrowser::launchMusicBrowser()
{
    #ifdef EMULATE_PICKER
    selectingMusicItem ("trackerid");
    #else
	if (!m_MusicBrowser) {
        SYS_DEBUG ("launching content picker...");
		m_MusicBrowser = new SelectSingleContentItemPage(
                QString(), 
                QStringList() << "http://www.tracker-project.org/temp/nmm#MusicPiece", 
                m_tone->trackerId());

		m_MusicBrowser->setObjectName(
                "SelectSingleContentItemPage_musicBrowser");
		connect(m_MusicBrowser, SIGNAL(backButtonClicked()), 
                this, SLOT(browserBackButtonClicked()));
		connect(m_MusicBrowser, SIGNAL(contentItemSelected(const QString &)), 
                this, SLOT(selectingMusicItem(const QString &)));

		m_MusicBrowser->appear(MSceneWindow::DestroyWhenDismissed);
	} else {
		m_MusicBrowser->appear(MSceneWindow::DestroyWhenDismissed);
    }
    #endif
}


void
AlertToneBrowser::launchOviStore()
{
	QString cmdline = "webwidgetrunner /usr/share/webwidgets/applications/d34177b1c241ea44cb132005b63ee6527c9f6040-wrt-widget.desktop -widgetparameter ringtones " /* + m_tone->key() */ + QString("&");
	qDebug() << "AlertToneBrowser(" << m_tone->key() << ")::launchOviStore:" << cmdline;
	system(cmdline.toUtf8().constData());
}

static QString
trackerIdToFilename(const QString &trackerId)
{
    const QString query = "select ?u where { <" + trackerId + "> nie:url ?u }";

    #ifdef DEBUG
    /* XXX: Deprecated, causes build failure:
     * XXX: error: 'class SopranoLive::RDFService' has no member named 'setConsoleVerbosity'
     *
     * ::tracker()->setConsoleVerbosity (5);
     */
    #endif
	QVector<QStringList> result = ::tracker()->rawSparqlQuery(query);

    SYS_DEBUG ("*** query         = %s", SYS_STR(query));
    SYS_DEBUG ("*** result.size() = %d", result.size());
	for (int Nix = 0; Nix < result.size() ; Nix++) {
		for (int Nix1 = 0 ; Nix1 < result[Nix].size() ; Nix1++) {
			QUrl url(result[Nix][Nix1]);
            SYS_DEBUG ("*** result[%d][%d] = %s", 
                    Nix, Nix1, SYS_STR(result[Nix][Nix1]));
			if (url.isValid() && url.scheme() == "file")
				return QUrl::fromPercentEncoding(url.path().toUtf8());
		}
    }

	return QString("");
}

void
AlertToneBrowser::currentClicked()
{
	setAlertTone(m_current->fullPath, true);
}

/*
 * NOTE: This method will not start the playing of the sound file any more.
 */
void
AlertToneBrowser::setAlertTone (
        const QString &fname,
        bool           setGui)
{
    SYS_DEBUG ("*** fname = %s", SYS_STR(fname));

	currSelectedFile = fname;

    if (setGui) {
        /*
         * The nice name is most probably already in the cache. If not the model
         * will do receive a signal. 
         * FIXME: how could we test this? Do we need to teszt what happens when
         * this item is not cached yet?
         */
        m_defaults->selectAndScroll (fname, 
                TrackerConnection::instance()->niceNameFromFileName(fname));
#if 0
    } else {
        // FIXME: this is a temprary code, i will create an elegant solution for
        // this.
        m_current->hide();
        if (m_current == m_MainLayout->itemAt (1) ) {
            m_MainLayout->removeAt (1);
        }
#endif
    }
}

/*!
 * This slot is called when the text in the live filtering text editor has been
 * changed.
 */
void 
AlertToneBrowser::textChanged ()
{ 
    bool     visible;
    QString  text;

    visible = m_LiveFilterEditor->isOnDisplay();
    if (!m_MainLayout)
        return;

    text = m_LiveFilterEditor->text();

    if (visible && text.isEmpty()) {
         /*
          * Removing the filter editor from the layout.
          */
        m_defaults->setFocus();
        m_LiveFilterEditor->setVisible (false);
        m_MainLayout->removeItem (m_LiveFilterEditor);
    } else if (!visible && !text.isEmpty()) {
        /*
         * Adding the filter editor widget to the layout.
         */
        m_MainLayout->insertItem (filterEditorPosition, m_LiveFilterEditor);
        m_LiveFilterEditor->setVisible (true);
        m_LiveFilterEditor->setFocus();
    }


    m_MainLayout->invalidate ();
    m_defaults->setFilterText (m_LiveFilterEditor->text());
    update ();
}

/*
 * This method will start the playback of a given file. If the player is already
 * playing the very same file the playback will be started.
 */
void
AlertToneBrowser::startPlayingSound (
        const QString &filename)
{
    bool playingTheSame = m_preview && m_preview->fname() == filename;

    SYS_DEBUG ("playingTheSame = %s", SYS_BOOL(playingTheSame));
    /*
     * If we are already playing the same sound we stop the playback. This is
     * coming from the UI specification: "Second tap of the item will stop the
     * preview while the sound is playing.
     */
    if (playingTheSame) {
        delete m_preview;
    } else {
        if (m_preview) 
            delete m_preview;
        m_preview = new AlertTonePreview(filename);
    }
}

/*
 * This method will stop the playback unconditionaly.
 */
void 
AlertToneBrowser::stopPlayingSound ()
{
    if (m_preview)
        delete m_preview;
}

/*
 * This slot is activated when the user clicks on a sound file in the default
 * sound list.
 */
void
AlertToneBrowser::defaultItemClicked (
        const QString &filename)
{
    SYS_DEBUG ("*** filename = %s", SYS_STR(filename));
    // We don't need to set the UI, this signal came from the UI already.
    setAlertTone (filename, false);
    startPlayingSound (filename);
}

/*
 * This slot is activated when the browser back button is clicked. Stopping the
 * playback will fix NB#199653 - Music preview doesn't stop when leaving from 
 * "Select from My music" view.
 *
 * FIXME: Maybe watching the back button signal is not robus enough, the
 * playback will not be stopped when some other method is used to page back...
 */
void
AlertToneBrowser::browserBackButtonClicked ()
{
    stopPlayingSound ();
    m_MusicBrowser->dismiss ();
}

/*
 * This slot is activated when the user selects one item from the content
 * picker.
 */
void
AlertToneBrowser::selectingMusicItem (
        const QString &item)
{
	QString fname = trackerIdToFilename(item);
    
    #ifdef EMULATE_PICKER
    fname = "//usr/share/sounds/ring-tones/Xylophone.aac";
    #endif
    if (fname.isEmpty()) {
        SYS_WARNING ("TrackerID '%s' is not valid.", SYS_STR(item));
        stopPlayingSound ();
        return;
    }

    SYS_DEBUG ("*** trackerID = %s", SYS_STR(item));
    SYS_DEBUG ("*** fname     = %s", SYS_STR(fname));

	m_current->fullPath = fname;
	setAlertTone(fname, true);
    startPlayingSound (fname);
}

/*
 * This virtual method is called when the MApplicationPage for the widget is
 * already there, so we can initialize it. 
 */
void
AlertToneBrowser::polishEvent ()
{
    QGraphicsWidget  *parent;
    MApplicationPage *page = 0;
    MWidgetAction    *widgetAction;
    /*
     * Just a protection about double adding the actions.
     */
    if (m_DoneButton)
        return;
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

    SYS_WARNING ("*** page = %p", page);
    if (!page)
        return;

    /**************************************************************************
     * Hiding the home button and the escape button from the page. 
     */
    page->setComponentsDisplayMode (
            MApplicationPage::EscapeButton,
            MApplicationPageModel::Hide);
    page->setComponentsDisplayMode (
            MApplicationPage::HomeButton,
            MApplicationPageModel::Hide);

    /**************************************************************************
     * Creating the 'done' button and adding it to the page.
     */
    m_DoneButton = new MButton (
            //% "Done"
            qtTrId("qtn_comm_command_done"),
            this);
    m_DoneButton->setViewType("toolbar");
    widgetAction = new MWidgetAction (this);
    widgetAction->setLocation(MAction::ToolBarLocation);
    widgetAction->setWidget (m_DoneButton);
    page->addAction(widgetAction);
    connect(m_DoneButton, SIGNAL(clicked()), 
            this, SLOT(accept()));
    
    /**************************************************************************
     * Creating the 'cancel' button and adding it to the page.
     */
    m_CancelButton = new MButton (
            //% "Cancel"
            qtTrId("qtn_comm_cancel"),
            this);
    m_CancelButton->setViewType("toolbar");
    widgetAction = new MWidgetAction (this);
    widgetAction->setLocation(MAction::ToolBarLocation);
    widgetAction->setWidget (m_CancelButton);
    page->addAction(widgetAction);
    connect(m_CancelButton, SIGNAL(clicked()), 
            this, SLOT(cancel()));
}



#ifdef RUN_STANDALONE
#include <MApplication>
#include <MApplicationWindow>
int
main(int argc, char **argv)
{
	int i = 0;
	MApplication app(argc, argv);
	MApplicationWindow w;
	AlertTone *tone = new AlertTone("ringing.alert.tone");
	AlertToneBrowser b(tone);

	gst_init(&argc, &argv);

	w.show();
	b.appear();

	i = app.exec();

	delete tone;
	return i;
}
#endif /* RUN_STANDALONE */