#ifndef GRIDIMAGEWIDGET_H
#define GRIDIMAGEWIDGET_H

#include <MImageWidget>

class GridImageWidget : public MImageWidget
{
    Q_OBJECT
    M_CONTROLLER(MImageWidget)

    public:
        GridImageWidget(QGraphicsItem *parent = 0);

    Q_SIGNALS:
        void clicked();
        void longPressed();

    protected:
         virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
         virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
         virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};
#endif
