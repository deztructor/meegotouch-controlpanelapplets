/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef USBAPPLET_H
#define USBAPPLET_H

#include <DcpAppletIf>
#include <QObject>

class MAction;
class DcpWidget;
class UsbBrief;
class UsbSettingsLogic;

class UsbApplet : public QObject, public DcpAppletIf 
{
	Q_OBJECT
	Q_INTERFACES (DcpAppletIf)

public:
    void                init ();
	DcpWidget          *constructWidget (int widgetId);
    QString             title () const;
    DcpBrief*           constructBrief (int partId);
    QVector<MAction*>   viewMenuItems ();

private:
    UsbBrief           *m_brief;
    UsbSettingsLogic   *m_logic;
};

#endif

