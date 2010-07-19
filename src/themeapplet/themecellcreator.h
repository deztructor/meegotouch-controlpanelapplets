/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef THEMECELLCREATOR_H__
#define THEMECELLCREATOR_H__

#include <QList>
#include <MAdvancedListItem>
#include <MAbstractCellCreator>

class ThemeCellCreator : public MAbstractCellCreator<MAdvancedListItem>
{
    public:
        virtual MWidget *createCell (
                const QModelIndex &index, 
                MWidgetRecycler   &recycler) const;

        virtual void updateCell (
                const QModelIndex &index, 
                MWidget           *cell) const;

        virtual void updateContentItemMode (
              const QModelIndex &index, 
              MAdvancedListItem *contentItem) const;

};

#endif
