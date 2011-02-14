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
#include "alerttonepreview.h"
#include "alerttonedefaults.h"
#include "alerttoneappletmaps.h"
#include "trackerconnection.h"
#include "drilldownitem.h"

#include <QtTracker/Tracker>
#include <QGraphicsLinearLayout>

#ifdef HAVE_CONTENT_MANAGER
/*
 * Defining this for now...
 * it seems it crashes somehow..
 * and i couldn't get any usable backtrace as
 * gdb is crashes too :-S
 */
#define SINGLE_CONTENTITEM
#include <SelectSingleContentItemPage.h>
#include <ContentItemsPage.h>
#endif

#include <MList>
#include <MListFilter>
#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MWindow>
#include <MAction>
#include <MTextEdit>
#include <MImageWidget>
#include <MContentItem>

//#define DEBUG
#define WARNING
#include "../debug.h"

static const int filterEditorPosition = 2;


class MCustomContentItem: public MContentItem
{
public:
    MCustomContentItem (MContentItem::ContentItemStyle itemStyle=MContentItem::IconAndTwoTextLabels, QGraphicsItem *parent=0)
    :MContentItem(itemStyle,parent),fullPath("") {}

    QString fullPath;
};

/******************************************************************************
 * AlertToneBrowser implementation.
 */
AlertToneBrowser::AlertToneBrowser(AlertTone *tone, QGraphicsWidget *parent):
    AlertToneToplevel (parent),
    m_tone (tone),
    m_defaults (0),
    m_preview (0),
    m_MusicBrowser (0),
    m_DoneAction (0),
    m_CancelAction (0)
{
    /*
     * FIXME: Why do we need to set the title?
     */
    setProperty ("title", AlertToneAppletMaps::mapToUiString (m_tone->key ()));

    createContent();
}

AlertToneBrowser::~AlertToneBrowser()
{
    SYS_DEBUG ("");
    stopPlayingSound ();
}

void
AlertToneBrowser::createContent()
{
    m_MainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    m_MainLayout->setContentsMargins (0., 0., 0., 0.);
    m_MainLayout->setSpacing (0.);
    setLayout (m_MainLayout);

#ifdef HAVE_CONTENT_MANAGER
    // "Pick from My Music"
    m_my_music = new DrillDownItem;
    m_my_music->setLayoutPosition (M::VerticalTopPosition);
    m_my_music->imageWidget()->setImage("icon-l-music");
    m_my_music->setObjectName("MContentItem_pickFromMyMusic");
    m_MainLayout->addItem (m_my_music);
    connect (m_my_music, SIGNAL (clicked ()), SLOT (launchMusicBrowser ()));
#endif

    // "Get more from Ovi store"
    m_ovi_store = new DrillDownItem;
    m_ovi_store->setLayoutPosition (M::VerticalBottomPosition);
    m_ovi_store->imageWidget()->setImage("icon-m-common-ovi");
    m_ovi_store->setObjectName("MContentItem_getMoreFromOviStore");
    m_MainLayout->addItem (m_ovi_store);
    connect (m_ovi_store, SIGNAL (clicked ()), SLOT (launchOviStore ()));

    /*
     * The list with the available sound files.
     */
    m_defaults = new AlertToneDefaults(m_tone, 0);
    m_defaults->filtering()->setEnabled (true);
    m_defaults->filtering()->setFilterMode (
            MListFilter::FilterAsBeginningOfLine);
    m_LiveFilterEditor = m_defaults->filtering()->editor();
    m_MainLayout->addItem(m_defaults);
    connect (m_defaults, SIGNAL (displayEntered ()),
             SLOT (defaultsDisplayEntered ()));

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
    SYS_DEBUG ("");

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
#ifdef HAVE_CONTENT_MANAGER
    m_my_music->setProperty ("title", qtTrId("qtn_sond_pick_music"));
#endif
    m_ovi_store->setProperty("title", qtTrId("qtn_sond_store"));

    if (m_DoneAction)
        //% "Done"
        m_DoneAction->setText (qtTrId("qtn_comm_command_done"));

    if (m_CancelAction)
        //% "Cancel"
        m_CancelAction->setText (qtTrId("qtn_comm_cancel"));
}

void
AlertToneBrowser::cancel()
{
    SYS_DEBUG ("");
    m_defaults->toneChanged ();
    emit closePage();
}

void
AlertToneBrowser::accept()
{
    SYS_DEBUG ("");
    if (!currSelectedFile.isEmpty())
        m_tone->set(currSelectedFile);

    emit closePage();
}

void
AlertToneBrowser::launchMusicBrowser()
{
#ifdef HAVE_CONTENT_MANAGER
    SYS_DEBUG ("launching content picker...");

    if (! m_MusicBrowser)
    {
#ifdef SINGLE_CONTENTITEM
        SelectSingleContentItemPage *page =
            new SelectSingleContentItemPage (
                QString (),
                QStringList () <<
                  "http://www.tracker-project.org/temp/nmm#MusicPiece",
                m_tone->trackerId ());

        m_MusicBrowser = page;

        page->setObjectName ("SelectSingleContentItemPage_musicBrowser");
        connect (page, SIGNAL (backButtonClicked ()),
                 SLOT (browserBackButtonClicked ()));
        connect (page, SIGNAL (contentItemSelected (const QString&)),
                 SLOT (selectingMusicItem (const QString&)));
#else
        ContentItemsPage *page = new ContentItemsPage;
        page->setCommonLayoutSuffix ("Inverted");
        page->setContentTypes (
                QStringList () <<
                  "http://www.tracker-project.org/temp/nmm#MusicPiece");
        page->selectItem (m_tone->trackerId ());

        m_MusicBrowser = page;

        page->setObjectName ("SelectSingleContentItemPage_musicBrowser");
        connect (page, SIGNAL (backButtonClicked ()),
                 SLOT (browserBackButtonClicked ()));
        connect (page, SIGNAL (itemClicked (const QString&)),
                 SLOT (selectingMusicItem (const QString&)));
#endif
    }

    m_MusicBrowser->appear (MSceneWindow::DestroyWhenDismissed);
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

/*
 * NOTE: This method will not start the playing of the sound file any more.
 */
void
AlertToneBrowser::setAlertTone (
        const QString &fname,
        bool           setGui)
{
    SYS_DEBUG ("*** fname  = %s", SYS_STR(fname));
    SYS_DEBUG ("*** setGui = %s", SYS_BOOL(setGui));

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
    }
}

/*!
 * Selects the given item in the browser. This method is going to work even if 
 * called before the widget is shown.
 */
void
AlertToneBrowser::set (
        const QVariant &filename)
{
    currSelectedFile = filename.toString();
    m_tone->set(filename);
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
    bool playingTheSame = false;

    if (m_preview)
        playingTheSame = m_preview->fname() == filename;

    SYS_DEBUG ("playingTheSame = %s", SYS_BOOL(playingTheSame));

    /*
     * If we are already playing the same sound we stop the playback. This is
     * coming from the UI specification: "Second tap of the item will stop the
     * preview while the sound is playing.
     */

    delete m_preview;
    m_preview = 0;

    if (! playingTheSame) {
        m_preview = new AlertTonePreview(filename);
    }
}

/*
 * This method will stop the playback unconditionaly.
 */
void
AlertToneBrowser::stopPlayingSound ()
{
    delete m_preview;
    m_preview = 0;
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
    SYS_DEBUG ("");
    stopPlayingSound ();
#ifdef HAVE_CONTENT_MANAGER
    m_MusicBrowser->dismiss ();
    m_MusicBrowser = 0;
#endif
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

    if (fname.isEmpty()) {
        SYS_WARNING ("TrackerID '%s' is not valid.", SYS_STR(item));
        stopPlayingSound ();
        return;
    }

    SYS_DEBUG ("*** trackerID = %s", SYS_STR(item));
    SYS_DEBUG ("*** fname     = %s", SYS_STR(fname));

    #ifndef SINGLE_CONTENTITEM
    m_MusicBrowser->dismiss ();
    #endif

    setAlertTone (fname, true);
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

    SYS_DEBUG ("");
    /*
     * Just a protection about double adding the actions.
     */
    if (m_DoneAction)
        return;

    MWindow *win = MApplication::activeWindow ();
    if (win) {
        connect (win, SIGNAL(displayExited()),
                this, SLOT(stopPlayingSound()));
        connect (win, SIGNAL(switcherEntered()),
                this, SLOT(stopPlayingSound()));
    }


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

    /**************************************************************************
     * Hiding the home button and the escape button from the page.
     */
    page->setComponentsDisplayMode (
            MApplicationPage::EscapeButton,
            MApplicationPageModel::Hide);
    page->setComponentsDisplayMode (
            MApplicationPage::HomeButton,
            MApplicationPageModel::Hide);

    //% "Done"
    m_DoneAction = new MAction(qtTrId("qtn_comm_command_done"), this);
    m_DoneAction->setLocation(MAction::ToolBarLocation);
    page->addAction(m_DoneAction);
    connect(m_DoneAction, SIGNAL(triggered()), SLOT(accept()));

    //% "Cancel"
    m_CancelAction = new MAction(qtTrId("qtn_comm_cancel"), this);
    m_CancelAction->setLocation(MAction::ToolBarLocation);
    page->addAction(m_CancelAction);
    connect(m_CancelAction, SIGNAL(triggered()), SLOT(cancel()));
}

