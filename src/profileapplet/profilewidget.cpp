/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "profilewidget.h"
#include "profilecontainer.h"
#include "dcpprofile.h"

#include <QGraphicsLinearLayout>
#include <MContainer>
#include <QDebug>

#undef DEBUG
#include "../debug.h"

ProfileWidget::ProfileWidget (
        ProfileDataInterface *api,
        QGraphicsWidget      *parent) :
    DcpWidget (parent),
    m_ProfileIf (api)
{
    SYS_DEBUG ("");
    initWidget ();
}

ProfileWidget::~ProfileWidget ()
{
}


void 
ProfileWidget::initWidget ()
{
    // catch profile If actions
   connect (m_ProfileIf, SIGNAL(vibrationValue(int, bool)), 
            this, SLOT(setVibration(int, bool)));

    // get init values
    initProfiles ();
}

void 
ProfileWidget::initProfiles ()
{
    QList<ProfileDataInterface::ProfileData> l = m_ProfileIf->getProfilesData();
    MContainer *mContainer = new MContainer(qtTrId("qtn_prof_vibration"));
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout();
    QGraphicsLinearLayout *vibraLayout = new QGraphicsLinearLayout(Qt::Vertical);
    
    SYS_DEBUG ("We have %d profiles.", l.count());

    // create profile containers
    for (int i = 0; i < l.count(); ++i) {
        ProfileDataInterface::ProfileData d = l.at(i);
        ProfileContainer* cont = new ProfileContainer(
            d.profileId,
            d.visualData.second,
            d.vibrationEnabled);
        connect (cont, SIGNAL(toggled(bool)), 
                this, SLOT(vibrationChanged(bool)));
        m_Containers.insert(d.profileId, cont);
        vibraLayout->addItem(cont);
    }

    /*
     * mainLayout
     */
    mContainer->centralWidget()->setLayout(vibraLayout);
    mainLayout->addItem(mContainer);
    this->setLayout (mainLayout);
}

void 
ProfileWidget::vibrationChanged (
        bool enabled)
{
    //NOTE: MButton->isChecked() method returns the state before the 
    // press at this point
    ProfileContainer *profile =
        static_cast<ProfileContainer*> (this->sender ());

    SYS_DEBUG ("valuechanged for %s: %d",
               SYS_STR (profile->text ()), index);

    m_ProfileIf->setVibration (profile->id (), enabled);
}

void 
ProfileWidget::setVibration (
        int profileId, 
        bool enabled)
{
    SYS_DEBUG ("set vibration for profile: %d, enabled: %s",
               profileId, SYS_BOOL (enabled));

    ProfileContainer *cont = m_Containers.value (profileId);

    if (cont)
        cont->setChecked (enabled);

}

void
ProfileWidget::retranslateUi ()
{
    SYS_DEBUG ("");

    // Re-load the profiles-data [with the new translations]
    QList<ProfileDataInterface::ProfileData> l = m_ProfileIf->getProfilesData();

    for (int i = 0; i < l.count(); ++i) {
        ProfileDataInterface::ProfileData d = l.at (i);

        // Update the containers title field
        m_Containers.value (d.profileId)->setText (d.visualData.second);
    }
}

