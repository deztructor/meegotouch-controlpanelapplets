/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MApplication>
#include <MApplicationWindow>
#include <QString>
#include <QStringList>
#include "ut_soundsettingsapplet.h"
#include "soundsettingsapplet.h"
#include "alerttonebrowser.h"
#include "alerttoneappletwidget.h"
#include <typeinfo>
/******************************************************************************
 * Ut_SoundSettingsApplet implementation.
 */
int   argc = 1;
char *argv[] = {
      (char *) "./ut_soundsettingsapplet",
      NULL };

void
Ut_SoundSettingsAppletTests::initTestCase()
{
      m_App = new MApplication(argc, argv);
}

void
Ut_SoundSettingsAppletTests::cleanupTestCase()
{
}

void
Ut_SoundSettingsAppletTests::soundsettingsappletConstructor ()
{
      MApplicationWindow aw;
      SoundSettingsApplet  ssa;
}

void
Ut_SoundSettingsAppletTests::soundsettingsappletconstructWidget ()
{
      DcpWidget * newWidget;
      SoundSettingsApplet  ssa;
      ssa.init();

      newWidget = ssa.constructWidget (3)  ;
      QVERIFY ( typeid (AlertToneAppletWidget) == typeid( * newWidget ) );

      newWidget = ssa.constructWidget (65536);
      QVERIFY ( typeid (AlertToneBrowser) == typeid( * newWidget) );

      newWidget = ssa.constructWidget (3);
      QVERIFY ( 0 ==  newWidget );

      QCOMPARE( ssa.m_stack.size() , 2 );
}

void
Ut_SoundSettingsAppletTests::soundsettingsappletinit()
{
      MApplicationWindow aw;
      SoundSettingsApplet  ssa;
      ssa.init();

      QVERIFY(ssa.m_alertTones.size() != 0 );
}

void
Ut_SoundSettingsAppletTests::soundsettingsapplettitle()
{

      SoundSettingsApplet  ssa;
      ssa.init();
      QCOMPARE( ssa.title(), QString ("qtn_sond_sounds")  );
}

void
Ut_SoundSettingsAppletTests::soundsettingsappletviewMenuItems ()
{
      MApplicationWindow aw;
      SoundSettingsApplet  ssa;

      ssa.init();
      ssa.constructWidget (3)  ;
       //ssa.viewMenuItems().size();
}

void Ut_SoundSettingsAppletTests::soundsettingsappletToplevelDestroyed()
{
      DcpWidget * newWidget;
      SoundSettingsApplet  ssa;
      ssa.init();

      newWidget = ssa.constructWidget (3);
      newWidget = ssa.constructWidget (65536);

      QCOMPARE( ssa.m_stack.size() , 2 );
      delete  ssa.m_stack.at(0);
      QCOMPARE( ssa.m_stack.size() , 2 );
      delete  ssa.m_stack.at(1);
      QCOMPARE( ssa.m_stack.size() , 1 );
}

QTEST_APPLESS_MAIN(Ut_SoundSettingsAppletTests)
