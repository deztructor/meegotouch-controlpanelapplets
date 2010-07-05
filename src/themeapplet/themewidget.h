/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QPointer>
#include <QModelIndex>
#include <MContentItem>
#include <dcpwidget.h>

#include "themebusinesslogic.h"
#include "themedescriptor.h"
#include "themelistmodel.h"

class MList;

class ThemeWidget : public DcpWidget
{
    Q_OBJECT

public:
    ThemeWidget (
            ThemeBusinessLogic *themeBusinessLogic, 
            QGraphicsWidget    *parent = 0);
    ~ThemeWidget ();

    void retranslateUi ();

private slots:
    void themeActivated(const QModelIndex &index);

private:
    void createWidgets ();
    void readLocalThemes ();

    QPointer<ThemeBusinessLogic>  m_ThemeBusinessLogic;
    ThemeListModel               *m_ThemeListModel;
    QList<ThemeDescriptor *>      m_ThemeDescList;
    MList                        *m_List;
    MContentItem                 *m_OviItem;
    #ifdef UNIT_TEST
    friend class Ut_ThemeWidget;
    #endif
};

#endif
