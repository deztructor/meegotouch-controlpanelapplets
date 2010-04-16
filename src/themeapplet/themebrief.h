/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef THEMEBRIEF_H
#define THEMEBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

#include "themebusinesslogic.h"


class ThemeBrief: public DcpBrief
{
    Q_OBJECT

public:
    ThemeBrief (ThemeBusinessLogic *businessLogic);

    virtual int widgetTypeID() const;
    virtual QString valueText() const;
    virtual QString icon () const;
    virtual QString image () const;

private:
    QPointer<ThemeBusinessLogic> m_ThemeBusinessLogic;
};

#endif
