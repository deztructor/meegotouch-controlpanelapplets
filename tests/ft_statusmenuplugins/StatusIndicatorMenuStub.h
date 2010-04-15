#ifndef STATUSINDICATORMENU_STUB_H
#define STATUSINDICATORMENU_STUB_H

#include <QObject>
#include <MWidget>
#include <duistatusindicatormenuplugininterface.h>

class StatusIndicatorMenuStub : public MWidget, public DuiStatusIndicatorMenuInterface
{
    Q_OBJECT

    public:
        virtual void showStatusIndicatorMenu ();
        virtual void hideStatusIndicatorMenu ();

};

#endif
