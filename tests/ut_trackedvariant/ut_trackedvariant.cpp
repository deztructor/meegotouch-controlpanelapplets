/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MApplication>
#include <QString>
#include <QStringList>
#include "ut_trackedvariant.h"
#include "qtrackedvariant.h"

/******************************************************************************
 * Ut_TrackedVariant implementation.
 */
void
Ut_TrackedVariantTests::init()
{
      cnt = 0;
}
void
Ut_TrackedVariantTests::cleanup()
{
}

void
Ut_TrackedVariantTests::trackedvariantConstructor ()
{
      QTrackedVariant tv("testName");
      QCOMPARE ( tv.key() , QString ("testName") );
}

void
Ut_TrackedVariantTests::trackedvariantSetValue ()
{
      QVariant v(123);
      QTrackedVariant tv("");
      tv.set(v);
      QCOMPARE ( tv.value().toInt() , 123 );
}

void
Ut_TrackedVariantTests::trackedvariantRealSetValue()
{
      QTrackedVariant tv( "email.alert.tone" );
      tv.m_val = QVariant("test");
      tv.fetchFromBackend();
      QCOMPARE ( tv.m_val.toString() , QString("test") );
}


class testClass : public QTrackedVariant
{
public:
      testClass(QString name):QTrackedVariant(name){}
      void  emitTestSignal(){  emit_changed(); }
};


void Ut_TrackedVariantTests::trackerSignalEmited ()
{
      testClass tv("");
	QObject::connect(&tv, SIGNAL(changed()), this, SLOT(catchsignal()));
	tv.emitTestSignal();
	QVERIFY(cnt != 0);
}

MApplication *app;

QTEST_APPLESS_MAIN(Ut_TrackedVariantTests)
