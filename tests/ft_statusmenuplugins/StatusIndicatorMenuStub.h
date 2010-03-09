#ifndef STATUSINDICATORMENU_STUB_H
#define STATUSINDICATORMENU_STUB_H

#include <QObject>
#include <DuiWidget>
#include <duistatusindicatormenuplugininterface.h>

class StatusIndicatorMenuStub : public DuiWidget, public DuiStatusIndicatorMenuInterface
{
    Q_OBJECT

    public:
        virtual void showStatusIndicatorMenu ();
        virtual void hideStatusIndicatorMenu ();

};

#endif
