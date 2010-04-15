/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <dcpwidget.h>

class MButton;
class MSlider;
class MLabel;
class MContainer;
class DisplayBusinessLogic;

class DisplayWidget : public DcpWidget
{
    Q_OBJECT

public:
    DisplayWidget(QGraphicsWidget *parent = 0);
    virtual ~DisplayWidget();

protected:
    void initWidget();

private slots:
    void modify_screenlight_handle (int newValue);
    void update_onoff_label (bool value);

private:
    DisplayBusinessLogic   *m_logic;
    QList<int>              m_brightness_vals;
    QList<int>              m_screenlight_vals;
    MSlider              *m_brightnessSlider;
    MSlider              *m_screenlightSlider;

    void retranslateUi ();

    MContainer           *m_brightnessContainer;
    MContainer           *m_screenlightContainer;
    MContainer           *m_blankInhibitContainer;
    MLabel               *m_OnOffLabel;
};

#endif // DISPLAYWIDGET_H

