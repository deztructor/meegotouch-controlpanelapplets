#ifndef THEMECELLCREATOR_H__
#define THEMECELLCREATOR_H__

#include <QList>
#include <MContentItem>
#include <MAbstractCellCreator>

class ThemeCellCreator : public MAbstractCellCreator<MContentItem>
{
    public:
        virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
};

#endif
