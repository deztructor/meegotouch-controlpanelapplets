/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMECONTAINER_H
#define THEMECONTAINER_H

#include <DuiWidget>
#include <QObject>

class QString;
class DuiWidget;
class DuiLabel;
class DuiImageWidget;
class QGraphicsSceneMouseEvent;
class ThemeDescriptor;

/*!
 * A simple layout type widget that represents one theme.
 */
class ThemeContainer: public DuiWidget 
{
    Q_OBJECT

public:
    ThemeContainer (
            ThemeDescriptor *descr,
            DuiWidget       *parent = 0);
    ~ThemeContainer ();

    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);

signals:
    /*!
     * \param themeName The name of the represented theme.
     *
     * Emitted when the user clicks on the widget.
     */
    void activated (ThemeDescriptor *themeDesc);

private:
    void createWidgets ();

    DuiLabel         *m_NameLabel;
    DuiImageWidget   *m_Icon;
    ThemeDescriptor  *m_ThemeDescriptor;
};
#endif
