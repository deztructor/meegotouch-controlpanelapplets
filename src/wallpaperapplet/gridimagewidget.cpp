
#include "gridimagewidget.h"
#include <QGraphicsSceneMouseEvent>

GridImageWidget::GridImageWidget (
        QGraphicsItem *parent)
    : MImageWidget(parent)
{
}


void GridImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mousePressEvent(event);
    event->accept();
}

void GridImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mouseReleaseEvent(event);
    emit clicked();
}

void GridImageWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    MImageWidget::contextMenuEvent(event);
    emit longPressed();
}

