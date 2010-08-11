/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapereditorwidget.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpaperinfoheader.h"

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

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(WallpaperEditorWidget)

static const qreal ScaleLowerLimit = 0.15;

#define DEBUG
#define WARNING
#include "../debug.h"

static const int ExtraMargin = 10;
static const int TitleBarHeight = 60;

WallpaperEditorWidget::WallpaperEditorWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic),
    m_InfoHeader (0),
    m_DoneAction (0),
    m_NoTitlebar (false),
    m_PinchOngoing (false),
    m_MotionOngoing (false),
    m_HasPendingRedraw (false)
{
    MWindow *win = MApplication::activeWindow ();

    if (win)
        win->showFullScreen();
    
    setObjectName ("WallpaperEditorWidget");
    QTimer::singleShot (0, this, SLOT(createContent()));
    createActions ();

    if (win) {
        m_Orientation = win->orientation();
        connect (win, SIGNAL(orientationChanged(M::Orientation)),
                this, SLOT(orientationChanged(M::Orientation)));
    } else {
        SYS_WARNING ("We have a problem: We don't know the orientation! "
                    "Only in tests though...");
        m_Orientation = M::Landscape;
    }
}

WallpaperEditorWidget::~WallpaperEditorWidget ()
{
}

void
WallpaperEditorWidget::paint (
        QPainter                          *painter,
        const QStyleOptionGraphicsItem    *option,
        QWidget                           *widget)
{
    bool portrait = (geometry().height() > geometry().width());
  
    #if 0
    SYS_DEBUG ("-------------------------------------");
    SYS_DEBUG ("*** offset   = %d, %d", imageX(), imageY());
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
                imageX(), imageY(),
                //m_Trans * m_bgPortrait.width (), m_Trans * m_bgPortrait.height (),
                imageDX(), imageDY(),
                m_bgPortrait);
    } else if (!portrait) {
        painter->drawPixmap (
                imageX(), imageY(),
                //m_Trans * m_bgLandscape.width (), m_Trans * m_bgLandscape.height (),
                imageDX(), imageDY(),
                m_bgLandscape);
    }

    MWidget::paint (painter, option, widget);
}

void
WallpaperEditorWidget::createContent ()
{
    MWindow             *win = MApplication::activeWindow ();
    WallpaperDescriptor *desc;

    SYS_DEBUG ("");

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
        
        SYS_DEBUG ("This is the current image.");
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
         * FIXME: We are using the original pixmap here, but we could make this
         * be more generic, and use the original only when it differs from the
         * edited, then we could move it to the WallpaperDescriptor class...
         */
        m_bgLandscape = cdesc->scaled (
                m_LandscapeTrans.expectedSize(),
                WallpaperDescriptor::OriginalLandscape);
        
        m_bgPortrait = cdesc->scaled (
                m_PortraitTrans.expectedSize(),
                WallpaperDescriptor::OriginalPortrait);
#if 0
        QPixmap landscapePixmap = cdesc->originalPixmap (M::Landscape);
        m_bgLandscape = landscapePixmap.scaled (
                m_LandscapeTrans.expectedSize(), 
                Qt::KeepAspectRatioByExpanding);

        QPixmap portraitPixmap = cdesc->originalPixmap (M::Portrait);
        m_bgPortrait = portraitPixmap.scaled (
                m_PortraitTrans.expectedSize(), 
                Qt::KeepAspectRatioByExpanding);
#endif
    
        /*
         * Need to copy back...
         */
        m_Trans = m_Orientation == M::Portrait ? 
            m_PortraitTrans : m_LandscapeTrans;

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

    /*
     * Need to copy back.
     */
    m_Trans = m_Orientation == M::Portrait ? 
        m_PortraitTrans : m_LandscapeTrans;

    /*
     * Here is what we do when this is not the current image.
     * If the image is very big the handling might be slow, so we scale it
     * down.
     */
#if 0
    desc->cache();
    m_bgLandscape = desc->scaled (m_LandscapeTrans.expectedSize());
    m_bgPortrait = desc->scaled (m_PortraitTrans.expectedSize());
#endif
    m_bgLandscape = desc->scaled (
            m_LandscapeTrans.expectedSize(),
            WallpaperDescriptor::Landscape);
   
    m_bgPortrait = desc->scaled (
           m_PortraitTrans.expectedSize(),
           WallpaperDescriptor::Portrait);


finalize:
    SYS_DEBUG ("*** m_bgLandscape is %dx%d", 
            m_bgLandscape.width(),
            m_bgLandscape.height());
    SYS_DEBUG ("*** m_bgPortrait  is %dx%d", 
            m_bgPortrait.width(),
            m_bgPortrait.height());

    this->setMinimumSize (m_Trans.expectedSize());

    /*
     * Enabling two finger gestures.
     */
    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);

    createWidgets ();
    redrawImage ();
}


void
WallpaperEditorWidget::createWidgets ()
{
    MLayout               *layout;
    MLinearLayoutPolicy   *policy;
    MWidget               *spacer;

    SYS_DEBUG ("");
    this->setContentsMargins (0, 0, 0, 0);
    this->setWindowFrameMargins (0.0, 0.0, 0.0, 0.0);

    /*
     *
     */
    layout = new MLayout;
    policy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    
    layout->setContentsMargins (0, 0, 0, 0);

    /*
     * The information header.
     */
    SYS_DEBUG ("Creating WallpaperInfoHeader");
    m_InfoHeader = new WallpaperInfoHeader;
    m_InfoHeader->setObjectName ("WallpaperInfoHeader");
    m_InfoHeader->setSizePolicy (
            QSizePolicy::Preferred,
            QSizePolicy::Minimum);
    policy->addItem (m_InfoHeader);
    
    spacer = new MWidget;
    spacer->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);
    policy->addItem (spacer);

    /*
     *
     */
    setLayout (layout);
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

    // FIXME: What to do then?
    if (!window)
        return;

    m_DoneAction = new MAction (
            "icon-m-framework-done",
            //% "Done"
            qtTrId("qtn_comm_command_done"),
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
    MWindow           *win;

    SYS_DEBUG ("");
    ltrans = m_Trans.orientation() == M::Landscape ?
        &m_Trans : &m_LandscapeTrans;
    ptrans = m_Trans.orientation() == M::Portrait ?
        &m_Trans : &m_PortraitTrans;

    /*
     * Here we save the settings.
     */
    m_WallpaperBusinessLogic->setBackground (ltrans, ptrans);
    
    /*
     * Turning back from fullscreen. This could be done in the destructor, but
     * that ends up with a segfault in the Qt...
     */
    win = MApplication::activeWindow ();
    if (win)
        win->showNormal();

    /*
     * We are done with the editing, let's page back to the view where we have
     * the list.
     */
    SYS_DEBUG ("Calling changeWidget()");
    changeWidget (0);
}

bool 
WallpaperEditorWidget::back ()
{
    MWindow *win;

    /*
     * Turning back from fullscreen. This could be done in the destructor, but
     * that ends up with a segfault in the Qt...
     */
    SYS_DEBUG ("");

    win = MApplication::activeWindow ();
    if (win)
        win->showNormal();

    return DcpWidget::back();
}

bool 
WallpaperEditorWidget::pagePans () const
{
    return false;
}

/******************************************************************************
 * Private low level functions.
 */

/*!
 * \returns The offset correction value for the new state.
 * \param show If the titlebar should be shown or not.
 *
 */
QPointF
WallpaperEditorWidget::toggleTitlebars (
        bool       show)
{
    MApplicationPage  *currentPage;
    /*
     * This shows 9, 69 and 9, 9 now, but the correct values are 10, 70 and 10,
     * 10 I believe. FIXME: Sooner or later we have to deal with this. 
     */
    QPointF r;
    r = mapToScene (0, 0);
    SYS_DEBUG ("r = %g, %g", r.x(), r.y());

    /*
     * If the requested state is the current state we don't need to hide/show
     * anything, we just return the correction value.
     */
    if (show == !m_NoTitlebar)
        goto finalize;
    m_NoTitlebar = !show;
    
    /*
     * We do the showing/hiding here.
     */
    if (MApplication::activeApplicationWindow())
        currentPage = MApplication::activeApplicationWindow()->currentPage();
    else 
        currentPage = 0;

    if (currentPage) {
        if (show) {
            currentPage->setComponentsDisplayMode (
                    MApplicationPage::AllComponents,
                    MApplicationPageModel::Show); 
            m_InfoHeader->show ();
        } else {
            currentPage->setComponentsDisplayMode (
                    MApplicationPage::AllComponents, 
                    MApplicationPageModel::Hide);
            m_InfoHeader->hide ();
        }
    }

finalize:
    /*
     * To return the correction value.
     */
    if (!show)
        return QPointF (0, TitleBarHeight);

    return QPointF (0, 0);
}

/*!
 * Returns the X offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageX () const
{
    int retval = 0;

    retval -= ExtraMargin;

    retval += m_UserOffset.x();
    retval += m_Trans.x();

    return retval;
}

int
WallpaperEditorWidget::imageDX () const
{
    bool portrait = (geometry().height() > geometry().width());

    return portrait ? 
        m_Trans * m_bgPortrait.width() : m_Trans * m_bgLandscape.width();
}

int
WallpaperEditorWidget::imageDY () const
{
    bool portrait = (geometry().height() > geometry().width());

    return portrait ? 
        m_Trans * m_bgPortrait.height() : m_Trans * m_bgLandscape.height();
}

/*!
 * Returns the Y offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageY () const
{
    int                 retval = 0;

    retval -= ExtraMargin;

    if (!m_NoTitlebar)
        retval -= TitleBarHeight;

    retval += m_UserOffset.y();
    retval += m_Trans.y();

    return retval;
}

void 
WallpaperEditorWidget::queueRedrawImage ()
{
    if (m_HasPendingRedraw) {
        //SYS_DEBUG ("Dropping...");
        return;
    }

    m_HasPendingRedraw = true;
    QTimer::singleShot(50, this, SLOT(redrawImage ()));
}

void 
WallpaperEditorWidget::redrawImage ()
{
    /*
     * We need to update the current page and not just this widget because of
     * those damn extra margins coming from somewhere.
     */
    //SYS_DEBUG ("Drawing...");
    m_HasPendingRedraw = false;
    if (MApplication::activeApplicationWindow())
        MApplication::activeApplicationWindow()->currentPage()->update();
}

void 
WallpaperEditorWidget::orientationChanged (
        M::Orientation orientation)
{
    if (m_Orientation == orientation) {
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
    
    m_Orientation = orientation;
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
    queueRedrawImage ();
}

void 
WallpaperEditorWidget::mouseMoveEvent (
        QGraphicsSceneMouseEvent *event)
{
    if (m_PinchOngoing)
        return;

    /*
     * If the tap happened outside the image we might still start moving the
     * image when the motion enters the image.
     */
    if (!m_MotionOngoing) {
        QPointF  position;

        position = event->pos();
        if (position.x() < imageX() ||
            position.y() < imageY() ||
            position.x() > imageX() + imageDX() ||
            position.y() > imageY() + imageDY()) {
            
            SYS_DEBUG ("Rejected... %g, %g not in %d, %d - %d, %d",
                position.x(), position.y(),
                imageX(), imageY(), 
                imageX() + imageDX(), 
                imageY() + imageDY());

            return;
        }

        m_MotionOngoing = true;
        toggleTitlebars (false);
        m_LastClick = event->pos();
        m_LastClick += toggleTitlebars (false);
        return;
    }

    m_UserOffset = event->pos() - m_LastClick;
    queueRedrawImage ();
}

void
WallpaperEditorWidget::mousePressEvent (
        QGraphicsSceneMouseEvent *event)
{
    QPointF  position;

    SYS_DEBUG ("Tapped");
    /*
     * If an ongoing pich gesture is processed we have nothing
     * to do here.
     */
    if (m_PinchOngoing)
        return;

    toggleTitlebars (false);
    /*
     * If the user tapped outside the image we reject moving the image. This way
     * the image can not be moved outside the visible area and the user feels
     * that the image can be grabbed but not the black background.
     */
    position = event->pos();
    if (position.x() < imageX() ||
            position.y() < imageY() ||
            position.x() > imageX() + imageDX() ||
            position.y() > imageY() + imageDY()) {
        SYS_DEBUG ("Rejected... %g, %g not in %d, %d - %d, %d",
                position.x(), position.y(),
                imageX(), imageY(), 
                imageX() + imageDX(), 
                imageY() + imageDY());
        return;
    }

    m_MotionOngoing = true;
    //toggleTitlebars (false);
    m_LastClick = event->pos();
    m_LastClick += toggleTitlebars (false);
}

void
WallpaperEditorWidget::mouseReleaseEvent (
        QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);

    SYS_DEBUG ("");
    if (!m_MotionOngoing) {
        toggleTitlebars (true);
        return;
    }

    SYS_DEBUG ("Finalizing something...");
    m_MotionOngoing = false;    
    m_Trans += m_UserOffset;
    m_UserOffset = QPointF();
    toggleTitlebars (true);
}

/*******************************************************************************
 * Stuff for the two finger gestures.
 */
void 
WallpaperEditorWidget::pinchGestureStarted (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    QPointF   centerPoint;

    Q_UNUSED (event);
    
    SYS_DEBUG ("Gesture started");
    if (m_PinchOngoing) {
        SYS_WARNING ("But gesture is not finished yet!");
        return;
    }

    /*
     *
     */
    centerPoint = gesture->centerPoint();
    if (centerPoint.x() < imageX() ||
            centerPoint.y() < imageY() ||
            centerPoint.x() > imageX() + imageDX() ||
            centerPoint.y() > imageY() + imageDY()) {
        SYS_DEBUG ("Rejected... %g, %g not in %d, %d - %d, %d",
                centerPoint.x(), centerPoint.y(),
                imageX(), imageY(), 
                imageX() + imageDX(), 
                imageY() + imageDY());
        return;
    }

    m_OriginalScaleFactor = m_Trans.scale();
    m_LastClick = centerPoint;
    m_PinchOngoing = true;
   
    if (!m_NoTitlebar) {
        m_LastClick += toggleTitlebars (true);
    }
   
    m_ImageFixpoint = QPointF (
            (centerPoint.x() - m_Trans.x()) / m_Trans.scale(),
            (centerPoint.y() - m_Trans.y()) / m_Trans.scale());

    event->accept(gesture);
}

void 
WallpaperEditorWidget::pinchGestureUpdate (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    qreal newScale;

    Q_UNUSED (event);
    
    if (!m_PinchOngoing)
        return;

    //SYS_DEBUG ("Gesture update");
    newScale = gesture->scaleFactor() * m_OriginalScaleFactor;
    if (newScale < ScaleLowerLimit)
        newScale = ScaleLowerLimit;
    m_Trans.setScale (newScale);

    m_UserOffset = QPointF (
            gesture->centerPoint().x() - m_ImageFixpoint.x() * m_Trans.scale() - m_Trans.x(),
            gesture->centerPoint().y() - m_ImageFixpoint.y() * m_Trans.scale() - m_Trans.y());
        
    event->accept(gesture);

    /*
     * No frame drop here: the pinch gesture is much better this way...
     */
    redrawImage ();
}

void 
WallpaperEditorWidget::pinchGestureEnded (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    Q_UNUSED (event);
    SYS_DEBUG ("Gesture finished");

    if (!m_PinchOngoing)
        return;

    m_PinchOngoing = false;
    m_Trans += m_UserOffset;
    m_UserOffset = QPointF();
    toggleTitlebars (true);

    event->accept(gesture);
}

void 
WallpaperEditorWidget::pinchGestureEvent (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    bool redrawNeeded = false;

    Q_UNUSED (event);
    
    if (gesture->state() == Qt::GestureStarted) {
        pinchGestureStarted (event, gesture);
    } else if (gesture->state() == Qt::GestureFinished) {
        pinchGestureEnded (event, gesture);
    } else {
        pinchGestureUpdate (event, gesture);
    }
}


