
#include "gridimagewidget.h"
#include <QGraphicsSceneMouseEvent>
#include <MImageWidget>
#include <MProgressIndicator>

//#define DEBUG
#define WARNING
#include "../debug.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(GridImageWidget)

//static const QString progressType = MProgressIndicator::barType;
static const QString progressType = MProgressIndicator::spinnerType;
static const QPen selectionPen (
        QBrush(), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);


GridImageLayout::GridImageLayout (
        QGraphicsLayoutItem *parent) : 
    QGraphicsLayout (parent),
    m_Image (0),
    m_ProgressBar (0),
    m_TopRightImageWidget (0)
{
}
 
GridImageLayout::~GridImageLayout()
{
#if 0
      QLayoutItem *item;
      while ((item = takeAt(0)))
          delete item;
#endif
}


int 
GridImageLayout::count() const
{
    int retval = 0;

    if (m_Image)
        retval++;
    if (m_ProgressBar)
        retval++;
    if (m_TopRightImageWidget)
        retval++;

    //SYS_DEBUG ("Returning %d", retval);
    return retval;
}


QGraphicsLayoutItem *
GridImageLayout::itemAt (int idx) const
{
    QGraphicsLayoutItem *retval = 0;

    if (idx == 0)
        retval = m_Image;
    else if (idx == 1)
        retval = m_ProgressBar;
    else if (idx == 2)
        retval = m_TopRightImageWidget;

    //SYS_DEBUG ("Returning %p at %d", retval, idx);
    return retval;
}

QGraphicsLayoutItem *
GridImageLayout::takeAt (int idx)
{
    SYS_DEBUG ("");
    return idx >= 0 && idx < list.size() ? list.takeAt(idx) : 0;
}

void 
GridImageLayout::removeAt (
        int index)
{
    SYS_DEBUG ("");
    list.removeAt (index);
    invalidate ();
}


void 
GridImageLayout::addItem (
        QGraphicsLayoutItem *layoutItem,
        GridImageLayoutRole  role)
{
    switch (role) {
        case Image:
            SYS_DEBUG ("Adding image");
            m_Image = layoutItem;
            break;

        case ProgressIndicator:
            SYS_DEBUG ("Adding progress indicator");
            m_ProgressBar = layoutItem;
            break;

        case CheckMark:
            SYS_DEBUG ("Adding checkmark");
            m_TopRightImageWidget = layoutItem;
    }

    addChildLayoutItem (layoutItem);
    invalidate ();
}

void 
GridImageLayout::setGeometry(
        const QRectF &r)
{
    qreal left, top, right, bottom;

    #if 1
    SYS_DEBUG ("---------------------------------------");
    SYS_WARNING ("*** at %g, %g (%gx%g)", r.x(), r.y(), r.width(), r.height());
    #endif

    QGraphicsLayout::setGeometry(r);

    getContentsMargins (&left, &top, &right, &bottom);

    if (m_Image)
        m_Image->setGeometry (r);

    if (m_TopRightImageWidget) {
        QSizeF iconSize = m_TopRightImageWidget->preferredSize ();
        QRectF iconGeometry (
                r.width() - iconSize.width() - left,
                top,
                iconSize.width(),
                iconSize.height());
        
        m_TopRightImageWidget->setGeometry (iconGeometry);
    }

    if (m_ProgressBar) {
        QSizeF progressSize;
        QRectF progressGeometry;
        
        if (progressType == MProgressIndicator::barType) {
            progressSize = m_ProgressBar->preferredSize ();
            progressGeometry = QRectF (
                    left, 
                    r.y() + r.height () - progressSize.height () - bottom, 
                    r.width () - left - right, 
                    progressSize.height ());
        } else {
            qreal HRem, VRem;
            progressSize = m_ProgressBar->minimumSize ();
            qreal progrWidth = progressSize.width();
            qreal progrHeight = progressSize.height();

            HRem = (r.width() - left - right - progrWidth) / 2;
            VRem = (r.height() - top - bottom - progrHeight) / 2;

            progressGeometry = QRectF (
                    left + HRem, 
                    top + VRem, 
                    progrWidth, 
                    progrHeight);
        }

        SYS_DEBUG ("*** progressSize = %gx%g", 
                progressSize.width(), 
                progressSize.height());

        SYS_DEBUG ("*** progressGeometry = %g, %g (%gx%g)", 
                progressGeometry.x(), 
                progressGeometry.y(), 
                progressGeometry.width(), 
                progressGeometry.height());
        m_ProgressBar->setGeometry (progressGeometry);
     }
}


QSizeF 
GridImageLayout::sizeHint (
        Qt::SizeHint which, 
        const QSizeF &constraint) const
{
    Q_UNUSED (which);
    Q_UNUSED (constraint);
    QSizeF retval (160.0, 160.0);

    if (m_Image)
        return m_Image->preferredSize ();

    return retval;
}



/******************************************************************************
 *
 */
GridImageWidget::GridImageWidget () :
    m_Layout (0),
    m_TopRightImageWidget (0),
    m_ProgressIndicator (0),
    m_Current (false)
{
    createLayout ();
}

void 
GridImageWidget::setCurrent (
        bool current)
{
    m_Current = current;
}

void 
GridImageWidget::paint (
        QPainter                       *painter, 
        const QStyleOptionGraphicsItem *option, 
        QWidget                        *widget)
{
    QRectF geom = geometry ();

    //MListItem::paint (painter, option, widget);
    painter->drawPixmap (0, 0, (int) geom.width(), (int) geom.height(), m_Pixmap);

    if (m_Current) {
        QPen pen(selectionPen);
        pen.setColor (QColor("Blue"));
        painter->setPen(pen);

        const int mod    = pen.width() % 2;
        const int adjust = pen.width() / 2;
        QRect rect (0, 0, 
                (int) geom.width(), (int) geom.height());
        const QRect selection = rect.adjusted(adjust,adjust,-(adjust+mod),-(adjust+mod));
        painter->drawRect(selection);
    }
}


QString 
GridImageWidget::topRightImage() const
{
    if (!m_TopRightImageWidget)
        return QString("");

    return m_TopRightImageWidget->image();
}


void 
GridImageWidget::setPixmap(
        const QPixmap &pixmap)
{
    if (!m_Layout)
        createLayout ();

    m_Pixmap = pixmap;
}


MProgressIndicator *
GridImageWidget::progressIndicator (
        bool create)
{
    if (!m_Layout)
        createLayout ();

    if (!m_ProgressIndicator && create) {
        SYS_DEBUG ("Creating a progress indicator.");
        m_ProgressIndicator = new MProgressIndicator (this, progressType);
        m_ProgressIndicator->setObjectName ("WallpaperLoadingSpinner");
        m_ProgressIndicator->setStyleName ("CommonProgressBarInverted");
        m_ProgressIndicator->setUnknownDuration (true);
        m_Layout->addItem (
                m_ProgressIndicator, 
                GridImageLayout::ProgressIndicator);
        // This seems to solve the issue in NB#208329. FIXME: We should at least
        // use some stored/calculated values instead of these litarals.
        m_Layout->setGeometry (QRectF(0.0, 0.0, 160.0, 160.0));
    } else {
        SYS_DEBUG ("Already have a progress indicator.");
    }

    return m_ProgressIndicator;
}


void 
GridImageWidget::createLayout()
{
    if (m_Layout) {
        SYS_WARNING ("Layout already created.");
        return;
    }

    m_Layout = new GridImageLayout(this);
    m_Layout->setContentsMargins(0.0, 0.0, 0.0, 0.0);
    setLayout (m_Layout);
}

void GridImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MListItem::mousePressEvent(event);
    event->accept();
}

void GridImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MListItem::mouseReleaseEvent(event);
    emit clicked();
}

void GridImageWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    MListItem::contextMenuEvent(event);
    emit longPressed();
}
