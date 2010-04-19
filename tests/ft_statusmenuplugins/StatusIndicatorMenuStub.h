#ifndef STATUSINDICATORMENU_STUB_H
#define STATUSINDICATORMENU_STUB_H

#include <QObject>
#include <MWidget>
#include <mstatusindicatormenuplugininterface.h>

class StatusIndicatorMenuStub : public MWidget, public MStatusIndicatorMenuInterface
{
    Q_OBJECT

    public:
        virtual void showStatusIndicatorMenu ();
        virtual void hideStatusIndicatorMenu ();

};

#endif
