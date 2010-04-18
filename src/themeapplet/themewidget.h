/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QPointer>
#include <MContentItem>
#include <dcpwidget.h>

#include "themebusinesslogic.h"
#include "themedescriptor.h"

class ThemeListContainer;
class MContainer;

class ThemeWidget : public DcpWidget
{
    Q_OBJECT

public:
    ThemeWidget (
            ThemeBusinessLogic *themeBusinessLogic, 
            QGraphicsWidget    *parent = 0);
    ~ThemeWidget ();

    void retranslateUi ();

    typedef enum {
        ThemeLocal,
        ThemeOvi
    } ThemeCategoryId;

private slots:
    void themeActivated(ThemeDescriptor *themeDescr);

private:
    void createWidgets ();
    MContainer *createContainer (ThemeWidget::ThemeCategoryId category);
    void readLocalThemes ();

    QPointer<ThemeBusinessLogic>  m_ThemeBusinessLogic;
    ThemeListContainer           *m_LocalContainer;
    ThemeListContainer           *m_OviContainer;
};

#endif
