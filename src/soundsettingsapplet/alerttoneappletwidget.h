/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef _ALERT_TONE_APPLET_WIDGET_H_
#define _ALERT_TONE_APPLET_WIDGET_H_

#include <MWidgetController>
#include <QPointer>

#include "profiledatainterface.h"
#include "alerttonetoplevel.h"
#include "alerttone.h"

class MLabel;
class MLinearLayoutPolicy;
class MApplicationExtensionArea;

class AlertToneAppletWidget : public AlertToneToplevel
{
    Q_OBJECT

    public:
        AlertToneAppletWidget (
                QList<AlertTone *>  alertTones, 
                QGraphicsWidget    *parent = 0);
        ~AlertToneAppletWidget ();


    protected:
        virtual void polishEvent ();
        virtual void createContents();

    private slots:
        void vibrationChanged (bool enabled);

    private:
        MWidgetController *createAlertTonesList (QGraphicsWidget *parent);
        MWidgetController *createFeedbackList (QGraphicsWidget *parent);
        void createProfileSwitches (
                MLinearLayoutPolicy   *policy,
                QGraphicsWidget       *parent);

    private:
        QList<AlertTone *>               m_alertTones;
        QPointer<ProfileDataInterface>   m_ProfileIf;
        MWidgetController               *m_tones;
        MWidgetController               *m_feedback;
        MApplicationExtensionArea       *m_volumeExtension;

#ifdef UNIT_TEST
    friend class Ut_AlertToneAppletWidgetTests;
#endif
};

#endif /* !_ALERT_TONE_APPLET_WIDGET_H_ */
