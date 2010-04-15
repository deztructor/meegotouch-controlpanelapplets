/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMECONTAINER_H
#define THEMECONTAINER_H

#include <MWidget>
#include <QObject>
#include <QPointer>

class QString;
class MWidget;
class MLabel;
class MImageWidget;
class QGraphicsSceneMouseEvent;
class ThemeDescriptor;
class ThemeBusinessLogic;

/*!
 * A simple layout type widget that represents one theme.
 */
class ThemeContainer: public MWidget 
{
    Q_OBJECT

public:
    ThemeContainer (
            ThemeDescriptor    *descr,
            ThemeBusinessLogic *logic = 0,
            MWidget          *parent = 0);
    ~ThemeContainer ();

    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);

public slots:
    void themeChanged (QString themeCodeName);

signals:
    /*!
     * \param themeName The name of the represented theme.
     *
     * Emitted when the user clicks on the widget.
     */
    void activated (ThemeDescriptor *themeDesc);

private:
    void createWidgets ();
    bool current ();

    MLabel            *m_NameLabel;
    MImageWidget      *m_Icon;
    ThemeDescriptor     *m_ThemeDescriptor;
    QPointer<ThemeBusinessLogic> m_ThemeBusinessLogic;
};
#endif
