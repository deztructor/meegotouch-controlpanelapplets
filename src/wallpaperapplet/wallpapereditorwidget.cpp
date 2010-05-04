/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapereditorwidget.h"
#include "wallpapercurrentdescriptor.h"

#include <QStyle>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QPixmap>
#include <QTimer>

#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QPanGesture>
#include <QPinchGesture>
#include <QTapGesture>
#include <QSwipeGesture>

#include <MLayout>
#include <MLinearLayoutPolicy>

#include <MAction>
#include <MApplicationWindow>
#include <MApplication>
#include <MApplicationPage>

//#include "mwidgetcreator.h"
//M_REGISTER_WIDGET_NO_CREATE(WallpaperEditorWidget)

#define DEBUG
#include "../debug.h"

static const int ExtraMargin = 10;

WallpaperEditorWidget::WallpaperEditorWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic),
    m_DoneAction (0),
    m_NoTitlebar (false),
    m_Gesture (false)
{
    MApplication::activeApplicationWindow()->showFullScreen();
    
    setObjectName ("WallpaperEditorWidget");
    QTimer::singleShot (0, this, SLOT(createContent()));
    createActions ();

    connect(MApplication::activeApplicationWindow(),
            SIGNAL(orientationChanged(M::Orientation)),
            this, SLOT(orientationChanged(M::Orientation)));

    /*
     * Enabling two finger gestures.
     */
    //setAcceptTouchEvents(true);
    //grabGesture(Qt::PinchGesture);

    SYS_DEBUG ("Emulating two finger gestures: %s", 
            SYS_BOOL(MApplication::emulateTwoFingerGestures()));
}

WallpaperEditorWidget::~WallpaperEditorWidget ()
{
    MApplication::activeApplicationWindow()->showNormal();
}

void
WallpaperEditorWidget::paint (
        QPainter                          *painter,
        const QStyleOptionGraphicsItem    *option,
        QWidget                           *widget)
{
    bool portrait = (geometry().height() > geometry().width());
  
    #if 0
    SYS_DEBUG ("*** expected = %d, %d", 
            m_Trans.expectedWidth (),
            m_Trans.expectedHeight ());
    SYS_DEBUG ("*** scale    = %g", m_Trans.scale());
    SYS_DEBUG ("*** bgsize   = %dx%d", 
            m_Trans.expectedWidth (), 
            m_Trans.expectedHeight ());
    #endif

    painter->fillRect (
            -ExtraMargin, -ExtraMargin, 
            m_Trans.expectedWidth (),
            m_Trans.expectedHeight (),
            QColor ("black"));

    if (portrait) {
        painter->drawPixmap (
                imageDX(), imageDY(),
                m_Trans * m_bgPortrait.width (),
                m_Trans * m_bgPortrait.height (),
                m_bgPortrait);
    } else if (!portrait) {
        painter->drawPixmap (
                imageDX(), imageDY(),
                m_Trans * m_bgLandscape.width (),
                m_Trans * m_bgLandscape.height (),
                m_bgLandscape);
    }

    //DcpWidget::paint (painter, option, widget);
    MWidget::paint (painter, option, widget);
}

void
WallpaperEditorWidget::createContent ()
{
    MWindow *win = MApplication::activeWindow ();
    WallpaperDescriptor *desc;

    desc = m_WallpaperBusinessLogic->editedImage();
    if (!desc) {
        SYS_WARNING ("There is no editedimage.");
        return;
    }

    if (desc->isCurrent()) {
        WallpaperCurrentDescriptor *cdesc;
       
        cdesc = qobject_cast<WallpaperCurrentDescriptor*> (desc);
        if (!desc) {
            SYS_WARNING ("This is the current wallpaper, but its type is"
                    "not <WallpaperCurrentDescriptor>");
            goto not_current_wallpaper;
        }
        
        SYS_DEBUG ("This image has already been edited.");
        SYS_DEBUG ("*** orig landscape       = %s", 
                SYS_STR(cdesc->originalImageFile (M::Landscape)));
        SYS_DEBUG ("*** orig portrait        = %s", 
                SYS_STR(cdesc->originalImageFile (M::Portrait)));


        m_LandscapeTrans = cdesc->iTrans (M::Landscape);
        m_PortraitTrans = cdesc->iTrans (M::Portrait);

        /*
         * Initialization of the image transformations for the current
         * wallpaper. 
         */
        m_PortraitTrans.setExpectedSize (win->visibleSceneSize (M::Portrait));
        m_PortraitTrans.setOrientation (M::Portrait);
        m_LandscapeTrans.setExpectedSize (win->visibleSceneSize (M::Landscape));
        m_LandscapeTrans.setOrientation (M::Landscape);
        
        /*
         * new stuff
         */
        QPixmap landscapePixmap (cdesc->originalImageFile (M::Landscape));
        m_bgLandscape = landscapePixmap.scaled (
                m_LandscapeTrans.expectedSize(), 
                Qt::KeepAspectRatioByExpanding);

        // FIXME: But most of the times the two images are the same!
        QPixmap portraitPixmap (cdesc->originalImageFile (M::Landscape));
        m_bgPortrait = portraitPixmap.scaled (
                m_PortraitTrans.expectedSize(), 
                Qt::KeepAspectRatioByExpanding);
    
        /*
         * Need to copy back...
         */
        m_Trans = win->orientation() == 
            M::Portrait ? m_PortraitTrans : m_LandscapeTrans;

        goto finalize;
    }

not_current_wallpaper:
    /*
     * Initializing image transformations when editing an image that is not the
     * current image.
     */
    m_PortraitTrans.setExpectedSize (win->visibleSceneSize (M::Portrait));
    m_PortraitTrans.setOrientation (M::Portrait);
    m_LandscapeTrans.setExpectedSize (win->visibleSceneSize (M::Landscape));
    m_LandscapeTrans.setOrientation (M::Landscape);
    
    m_Trans = win->orientation() == 
        M::Portrait ? m_PortraitTrans : m_LandscapeTrans;

    /*
     * Here is what we do when this is not the current image.
     * If the image is very big the handling might be slow, so we scale it
     * down.
     */
    desc->cache();
    m_bgLandscape = desc->scaled (m_LandscapeTrans.expectedSize());
    m_bgPortrait = desc->scaled (m_PortraitTrans.expectedSize());


    //this->setContentsMargins (0, 0, 0, 0);
    //this->setWindowFrameMargins (0.0, 0.0, 0.0, 0.0);
finalize:
    this->setMinimumSize (m_Trans.expectedSize());

    /*
     * Enabling two finger gestures.
     */
    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);

    redrawImage ();
}

/*
 * In this function we create the actions (currently only the 'done' action that
 * is shown in the toolbar. Unfortunatelly the 'done' action can not be added to
 * a widget (it is not shown in the toolbar if added to a widget I guess because
 * the widget is not added to the window yet), and it also can not be added to
 * the current page (the new page for this widget is not created yet), so we
 * have to get the application window and add the action to that. 
 */
void
WallpaperEditorWidget::createActions ()
{
    MApplicationWindow *window = 
        MApplication::instance()->activeApplicationWindow();

    m_DoneAction = new MAction (
            "icon-m-framework-done",
            //% "Done"
            qtTrId("qtn_wall_done"), 
            this);
    m_DoneAction->setLocation(MAction::ToolBarLocation);
    window->addAction(m_DoneAction);
    m_DoneAction->setVisible(true);

    connect(m_DoneAction, SIGNAL(triggered()), 
            this, SLOT(slotDoneActivated()));
}

/*
 * This slot is called when the user activates the 'done' action and so we have
 * to store the settings.
 */
void
WallpaperEditorWidget::slotDoneActivated ()
{
    WallpaperITrans   *ltrans, *ptrans;

    SYS_DEBUG ("");
    ltrans = m_Trans.orientation() == M::Landscape ?
        &m_Trans : &m_LandscapeTrans;
    ptrans = m_Trans.orientation() == M::Portrait ?
        &m_Trans : &m_PortraitTrans;

    m_WallpaperBusinessLogic->setBackground(ltrans, ptrans);
    changeWidget (0);
}

bool 
WallpaperEditorWidget::pagePans () const
{
    return false;
}


/*!
 * Returns the X offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageDX () const
{
    int retval = 0;

    retval -= ExtraMargin;

    retval += m_UserOffset.x();
    retval += m_Trans.x();

    return retval;
}

/*!
 * Returns the Y offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageDY () const
{
    int                 retval = 0;

    retval -= ExtraMargin;

    if (!m_NoTitlebar)
        retval -= 70;

    retval += m_UserOffset.y();
    retval += m_Trans.y();

    return retval;
}

void 
WallpaperEditorWidget::redrawImage ()
{
    /*
     * We need to update the current page and not just this widget because of
     * those damn extra margins coming from somewhere.
     */
    MApplication::activeApplicationWindow()->currentPage()->update();
}

void 
WallpaperEditorWidget::orientationChanged (
        M::Orientation orientation)
{
    if (m_Trans.orientation() == orientation) {
        SYS_WARNING ("This is the old orientation?!");
        return;
    }

    switch (orientation) {
        case M::Portrait:
            SYS_DEBUG ("Turned to portrait");
            m_LandscapeTrans = m_Trans;
            m_Trans = m_PortraitTrans;
            this->setMinimumSize (m_Trans.expectedSize());
            break;

        case M::Landscape:
            SYS_DEBUG ("Turned to landscape");
            m_PortraitTrans = m_Trans;
            m_Trans = m_LandscapeTrans;
            this->setMinimumSize (m_Trans.expectedSize());
            break;
    }
    
    redrawImage ();
}

/******************************************************************************
 * Stuff for the normal mouse events.
 */
void 
WallpaperEditorWidget::wheelEvent (
        QGraphicsSceneWheelEvent *event)
{
    m_Trans.modScale (event->delta());
    redrawImage ();
}

void 
WallpaperEditorWidget::mouseMoveEvent (
        QGraphicsSceneMouseEvent *event)
{
    if (m_Gesture)
        return;

    SYS_DEBUG ("Mouse move");
    m_UserOffset = event->pos() - m_LastClick;
    redrawImage ();
}

void
WallpaperEditorWidget::mousePressEvent (
        QGraphicsSceneMouseEvent *event)
{
    MApplicationPage  *currentPage;

    if (m_Gesture)
        return;

    SYS_DEBUG ("Mouse press");
    /*
     * FIXME: Should add a method for this and for the point modification also
     * has to be done there.
     */
    currentPage = MApplication::activeApplicationWindow()->currentPage();
    currentPage->setComponentsDisplayMode (
            MApplicationPage::AllComponents,
            MApplicationPageModel::Hide);
    m_LastClick = event->pos();

    /*
     * FIXME: We need this, because we just hide the titlebar.
     */
    m_LastClick += QPointF (0, 70);
    m_NoTitlebar = true;
}

void
WallpaperEditorWidget::mouseReleaseEvent (
        QGraphicsSceneMouseEvent *event)
{
    MApplicationPage  *currentPage;
    
    SYS_DEBUG ("Mouse release");
    currentPage = MApplication::activeApplicationWindow()->currentPage();
    currentPage->setComponentsDisplayMode (
            MApplicationPage::AllComponents,
            MApplicationPageModel::Show);

    m_Trans += m_UserOffset;
    m_UserOffset = QPointF();
    m_NoTitlebar = false;
    
    redrawImage ();
}

/*******************************************************************************
 * Stuff for the two finger gestures.
 */
bool
WallpaperEditorWidget::event(QEvent *e)
{
    if (e->type() == QEvent::TouchBegin) {
        e->setAccepted(true);
        return true;
    }
    return DcpWidget::event(e);
}

void 
WallpaperEditorWidget::pinchGestureEvent (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    Q_UNUSED (event);
    
    if (gesture->state() == Qt::GestureStarted) {
        SYS_DEBUG ("Gesture started");
        m_OriginalScaleFactor = m_Trans.scale();
        m_LastClick = gesture->centerPoint ();
        m_Gesture = true;
        if (!m_NoTitlebar) {
            MApplicationPage  *currentPage;
            currentPage = 
                MApplication::activeApplicationWindow()->currentPage();
            currentPage->setComponentsDisplayMode (
                MApplicationPage::AllComponents,
                MApplicationPageModel::Hide);
            m_LastClick += QPointF (0, 70);
            m_NoTitlebar = true;
        }
    } else if (gesture->state() == Qt::GestureFinished) {
        SYS_DEBUG ("Gesture finished");
        //m_Trans += m_UserOffset;
        //m_UserOffset = QPointF();
        //m_NoTitlebar = false;
        m_Gesture = false;
    } else {
        //SYS_DEBUG ("Gesture update");
        m_Trans.setScale (gesture->scaleFactor() * m_OriginalScaleFactor);
        m_UserOffset = gesture->centerPoint() - m_LastClick;
    }

    event->accept(gesture);
    redrawImage ();
}

