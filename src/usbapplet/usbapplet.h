/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef USBAPPLET_H
#define USBAPPLET_H

#include <QObject>
#include <QPointer>
#include <DcpAppletIf>
#include <qmusbmode.h>

#include "usbview.h"

class MAction;
class DcpWidget;
class UsbBrief;

class UsbApplet : public QObject, public DcpAppletIf 
{
	Q_OBJECT
	Q_INTERFACES (DcpAppletIf)

public:
    virtual void        init ();
	virtual DcpWidget  *constructWidget (int widgetId);
    virtual QString     title () const;
    virtual DcpBrief   *constructBrief (int partId);
    QVector<MAction *>  viewMenuItems ();

private:
    QPointer<UsbView>   m_MainWidget;
    UsbBrief           *m_brief;
    Maemo::QmUSBMode   *m_logic;
    #ifdef UNIT_TEST
    friend class Ut_UsbApplet;
    #endif
};

#endif

