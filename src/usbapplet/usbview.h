/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef USBVIEW_H
#define USBVIEW_H

#include <DcpWidget>
#include <qmusbmode.h>

class MLabel;
class MButton;
class MButtonGroup;

class UsbView : public DcpWidget
{
    Q_OBJECT

public:
    UsbView (Maemo::QmUSBMode *logic);

signals:
    void settingsChanged ();

private slots:
    void selectionChanged (int id);

protected:
    void retranslateUi ();

private:
    void initWidget (void);

    Maemo::QmUSBMode    *m_logic;
    MButton             *m_buttons[3];
    MButtonGroup        *m_btn_group;
    MLabel              *m_info_label;
};

#endif

