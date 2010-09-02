/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QPointer>
#include <QModelIndex>
#include <dcpwidget.h>

#include "themebusinesslogic.h"
#include "themelistmodel.h"
#include "themecellcreator.h"

class MList;
class MTextEdit;
class MSortFilterProxyModel;
class MBasicListItem;

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
    void selectCurrentTheme ();
    void themeActivated(const QModelIndex &index);
    void oviActivated ();
    void textChanged ();
    void hideEmptyTextEdit ();
    
private:
    void createWidgets ();
    void readLocalThemes ();

    QPointer<ThemeBusinessLogic>  m_ThemeBusinessLogic;
    ThemeListModel               *m_ThemeListModel;
    MSortFilterProxyModel        *m_Proxy;
    ThemeCellCreator             *m_CellCreator;

    MTextEdit                    *m_LiveFilterEditor;
    MList                        *m_List;
    MBasicListItem               *m_OviItem;
    #ifdef UNIT_TEST
    friend class Ut_ThemeWidget;
    #endif
};

#endif
