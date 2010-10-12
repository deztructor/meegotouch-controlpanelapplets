
#include "gridimagewidget.h"
#include <QGraphicsSceneMouseEvent>
#include <MImageWidget>
#include <MProgressIndicator>

#define DEBUG
#include "../debug.h"

//static const QString progressType = MProgressIndicator::barType;
static const QString progressType = MProgressIndicator::spinnerType;

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
            m_Image = layoutItem;
            break;

        case ProgressIndicator:
            m_ProgressBar = layoutItem;
            break;

        case CheckMark:
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

    #if 0
    SYS_DEBUG ("---------------------------------------");
    SYS_DEBUG ("*** at %g, %g (%gx%g)", r.x(), r.y(), r.width(), r.height());
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
    QSizeF retval (100.0, 100.0);

    if (m_Image)
        return m_Image->preferredSize ();

    return retval;
}



/******************************************************************************
 *
 */
GridImageWidget::GridImageWidget () :
    m_Layout (0),
    m_ImageWidget (0),
    m_TopRightImageWidget (0),
    m_ProgressIndicator (0)
{
    createLayout ();
}

QString 
GridImageWidget::image() const
{
    if (!m_ImageWidget)
        return QString("");

    return m_ImageWidget->image();
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
    if (!m_ImageWidget)
        createLayout ();

    m_ImageWidget->setPixmap(pixmap);
}

void
GridImageWidget::setImage(
        const QString &id)
{
    if (!m_ImageWidget)
        createLayout ();

    m_ImageWidget->setImage (id);
}

void
GridImageWidget::setImage(
        const QString  &id,
        const QSizeF   &size)
{
    if (!m_ImageWidget)
        createLayout ();

    m_ImageWidget->setImage (id, QSize(
                (int) size.width(), 
                (int) size.height()));
}


void 
GridImageWidget::setTopRightImage (
        const QString &id)
{
    /*
     * If the ID is not empty and we have no stamp widget we create one.
     */
    if (!m_TopRightImageWidget && !id.isEmpty()) {
        m_TopRightImageWidget = new MImageWidget (this);
        m_TopRightImageWidget->setObjectName ("CommonIcon");
        m_Layout->addItem (
                m_TopRightImageWidget, 
                GridImageLayout::CheckMark);
    }

    /*
     * Set the image and show it if the ID is valid.
     */
    if (!id.isEmpty()) {
        m_TopRightImageWidget->setImage (id);
        m_TopRightImageWidget->show();
    }
    
    /*
     * The stamp can be disabled by the empty ID.
     */
    if (id.isEmpty() && m_TopRightImageWidget) {
        m_TopRightImageWidget->setImage (id);
        m_TopRightImageWidget->hide ();
    }
}

MProgressIndicator *
GridImageWidget::progressIndicator (
        bool create)
{
    if (!m_Layout)
        createLayout ();

    if (!m_ProgressIndicator && create) {
        m_ProgressIndicator = new MProgressIndicator (this, progressType);
        m_ProgressIndicator->setObjectName ("CommonProgressBar");
        m_ProgressIndicator->setUnknownDuration (true);
        m_Layout->addItem (
                m_ProgressIndicator, 
                GridImageLayout::ProgressIndicator);
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
    m_Layout->setContentsMargins(10.0, 10.0, 10.0, 10.0);

    m_ImageWidget = new MImageWidget (this);

    m_Layout->addItem (
            m_ImageWidget,
            GridImageLayout::Image);

    m_ImageWidget->show ();
    setLayout (m_Layout);
}

#if 1
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
#endif
