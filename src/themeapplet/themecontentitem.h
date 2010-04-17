/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMECONTENTITEM_H
#define THEMECONTENTITEM_H

#include <MContentItem>
#include <QPointer>
#include <QObject>

class ThemeDescriptor;
class ThemeBusinessLogic;

class ThemeContentItem : public MContentItem
{
        Q_OBJECT

public:
    ThemeContentItem (
            ThemeDescriptor    *descr,
            ThemeBusinessLogic *logic = 0,
            MWidget            *parent = 0);
    ~ThemeContentItem ();

public slots:
    void themeChanged (QString themeCodeName);
    void slotClicked ();
    
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

    ThemeDescriptor              *m_ThemeDescriptor;
    QPointer<ThemeBusinessLogic>  m_ThemeBusinessLogic;
};

#endif
