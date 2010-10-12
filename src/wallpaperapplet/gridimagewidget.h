#ifndef GRIDIMAGEWIDGET_H
#define GRIDIMAGEWIDGET_H

#include <MListItem>
#include <QString>

class MImageWidget;
class MProgressIndicator;

#include <QGraphicsLayout>
#include <QList>


class GridImageLayout : public QGraphicsLayout
{
    public:
        GridImageLayout (QGraphicsLayoutItem *parent = 0);
        ~GridImageLayout();

        typedef enum {
            Image,
            ProgressIndicator,
            CheckMark
        } GridImageLayoutRole;

        void addItem (
                QGraphicsLayoutItem *item, 
                GridImageLayoutRole role);
     
        QSizeF sizeHint (
             Qt::SizeHint which, 
             const QSizeF &constraint = QSizeF()) const;

        int count() const;

        QGraphicsLayoutItem *itemAt(int) const;
        QGraphicsLayoutItem *takeAt(int);
        void removeAt (int index);

        virtual void setGeometry(const QRectF &rect);

    private:
        QList<QGraphicsLayoutItem*> list;
        QGraphicsLayoutItem   *m_Image;
        QGraphicsLayoutItem   *m_ProgressBar;
        QGraphicsLayoutItem   *m_TopRightImageWidget;
};

class GridImageWidget : public MListItem
{
    Q_OBJECT

    public:
        GridImageWidget();
        
        QString image() const;
        QString topRightImage() const;

    public Q_SLOTS:
        void setPixmap(const QPixmap &pixmap);
        void setImage(const QString &id);
        void setImage(const QString &id, const QSizeF &size);
        void setTopRightImage (const QString &id);

        MProgressIndicator *progressIndicator(bool create);

    Q_SIGNALS:
        //void clicked();
        void longPressed();

   protected:
        virtual void createLayout();

   private:
        GridImageLayout     *m_Layout;
        MImageWidget        *m_ImageWidget;
        MImageWidget        *m_TopRightImageWidget;
        MProgressIndicator  *m_ProgressIndicator;
#if 1
    protected:
         virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
         virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
         virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
#endif
};
#endif
