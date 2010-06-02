#ifndef THEMECELLCREATOR_H__
#define THEMECELLCREATOR_H__

#include <QList>
#include <MContentItem>
#include <MAbstractCellCreator>

class ThemeCellCreator : public MAbstractCellCreator<MContentItem>
{
    public:
        virtual MWidget *createCell(const QModelIndex &index, 
                                    MWidgetRecycler &recycler) const;
        virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
};

#endif
