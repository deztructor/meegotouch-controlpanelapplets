/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef USBVIEW_H
#define USBVIEW_H

#include <DcpWidget>

#ifdef HAVE_QMSYSTEM
#  include <qmusbmode.h>
using namespace Maemo;
#endif

class MLabel;
class MButton;
class MButtonGroup;
class MNotification;

class UsbView : public DcpWidget
{
    Q_OBJECT

public:
    #ifdef HAVE_QMSYSTEM
    UsbView (Maemo::QmUSBMode *logic);
    #else
    UsbView (void *logic);
    #endif
    ~UsbView ();

signals:
    void settingsChanged ();

private slots:
    void selectionChanged (int id);

protected:
    void retranslateUi ();

private:
    void initWidget (void);
    #ifdef HAVE_QMSYSTEM
    Maemo::QmUSBMode    *m_logic;
    #endif
    MButton             *m_buttons[3];
    MButtonGroup        *m_btn_group;
    MLabel              *m_info_label;
    MNotification       *m_error;
    #ifdef UNIT_TEST
    friend class Ut_UsbApplet;
    #endif
};

#endif

