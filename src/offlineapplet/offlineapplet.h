#ifndef OFFLINEAPPLET_H
#define OFFLINEAPPLET_H

#include <DcpAppletIf>
#include <QObject>
class DcpWidget;


class OfflineApplet : public QObject, public DcpAppletIf 
{
	Q_OBJECT
	Q_INTERFACES(DcpAppletIf)

public:
    virtual void init();
    virtual DcpWidget* constructWidget(int widgetId);
    virtual QString title() const;
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int);
};
#endif // OFFLINEAPPLET_H
