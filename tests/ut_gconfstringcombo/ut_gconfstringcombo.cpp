/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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

#include <MApplication>
#include <QString>
#include <QStringList>
#include  "ut_gconfstringcombo.h"
#include  "gconfstringcombo.h"

#define DEBUG
#define WARNING
#include "../debug.h"

/*********************************************************************************
 * GConf read-write stub. 
 */
#include <gconf/gconf-client.h>
#include <gconf/gconf-value.h>

static QHash<QString, QString> MyGConfDatabase;

void 
Ut_GConfStringComboTests::_setGConfReturnString (
        const QString   &key,
        const QString   &newValue, 
        QGConfValue     *toNotify)
{
    SYS_DEBUG ("*** key        = %s", SYS_STR(key));
    SYS_DEBUG ("*** newValue   = %s", SYS_STR(newValue));
    MyGConfDatabase[key] = newValue;
    if (toNotify)
        QGConfValue::notifyValue (0, 0, 0, toNotify);
}

GConfValue *
gconf_client_get (
        GConfClient* client,
        const gchar* key,
        GError** err)
{
    QString myKey = key;
    GConfValue *retval;

    SYS_DEBUG ("*** key = %s", SYS_STR(myKey));
    if (myKey == "/meegotouch/target/name") {
        // FIXME: This maybe is not a good idea, we could read the GConf
        // database from a separate process.
        const char *r = "Default";

        retval = gconf_value_new (GCONF_VALUE_STRING);
        gconf_value_set_string (retval, r);
        goto finalize;
    } else if (myKey.startsWith("/meegotouch/i18n/")) {
        // FIXME: This maybe is not a good idea, we could read the GConf
        // database from a separate process.
        const char *r = "en_US";

        retval = gconf_value_new (GCONF_VALUE_STRING);
        gconf_value_set_string (retval, r);
        goto finalize;
    } else {
        const char *retString;

        retString = MyGConfDatabase[myKey].toLatin1().constData();
        SYS_DEBUG ("returning %s", retString);
        retval = gconf_value_new (GCONF_VALUE_STRING);
        gconf_value_set_string (retval, retString);
        goto finalize;
    }

    retval = gconf_value_new (GCONF_VALUE_BOOL);

finalize:
    return retval;
}

static QString lastChangedKey;
static QString lastStringValue;

gboolean
gconf_client_set_string  (
        GConfClient* client, 
        const gchar* key,
        const gchar* val, 
        GError** err)
{
    lastChangedKey = key;
    lastStringValue = val;

    SYS_DEBUG ("*** key = %s", SYS_STR(lastChangedKey));
    SYS_DEBUG ("*** val = %s", SYS_STR(lastStringValue));

    return true;
}

/*******************************************************************************
 * Stubbing the localization.
 */
static const QString translatedSuffix ("-translated");
QString
qtTrId (
        const char  *id, 
        int          n)
{
    QString retVal (id);

    /*
     * This will show that the string is actually trabslated.
     */
    retVal += translatedSuffix;

    SYS_DEBUG ("*** id          = %s", id);
    SYS_DEBUG ("*** translation = %s", SYS_STR(retVal));
    return retVal;
}

/******************************************************************************
 * Ut_GConfStringCombo implementation.
 */
int   argc = 1;
char *argv[] = {
      (char *) "./ut_gconfstringcombo",
      NULL };

void
Ut_GConfStringComboTests::initTestCase()
{
      m_App = new MApplication(argc, argv);
}

void
Ut_GConfStringComboTests::cleanupTestCase()
{
    if (m_App)
        delete m_App;
}

void
Ut_GConfStringComboTests::gconfstringcomboConstructor_data ()
{
      QTest::addColumn<QString>("gconfkey");
      QTest::addColumn<QString>("val1");
      QTest::addColumn<QString>("val2");
      QTest::addColumn<QString>("val3");
      QTest::addColumn<QString>("val4");

      QTest::newRow("") << "/meegotouch/input_feedback/volume/priority2/volume/pulse" << "off" << "low" << "medium" << "high";
      QTest::newRow("") << "/meegotouch/input_feedback/volume/priority2/volume/vibra" << "off" << "low" << "medium" << "high";
}

void
Ut_GConfStringComboTests::gconfstringcomboConstructor ()
{
      QFETCH(QString, gconfkey);
      QFETCH(QString, val1);
      QFETCH(QString, val2);
      QFETCH(QString, val3);
      QFETCH(QString, val4);

      QStringList  list;
      list.push_back(val1);list.push_back(val2);list.push_back(val3);list.push_back(val4);
      GConfStringCombo  gcsc (gconfkey, list);

		  /* Use gcsc.m_val instead */
			/*
      QGConfValue * val;
      val = (QGConfValue *) getPtr (0,(void *)& gcsc ) ;
			*/

      QCOMPARE(gcsc.m_possibleValues.size(),4);
      QCOMPARE(gcsc.m_possibleValues.at(0),val1);
      QCOMPARE(gcsc.m_possibleValues.at(1),val2);
      QCOMPARE(gcsc.m_possibleValues.at(2),val3);
      QCOMPARE(gcsc.m_possibleValues.at(3),val4);
}

void
Ut_GConfStringComboTests::gconfstringcomboChangedInGConf_data()
{
    QTest::addColumn<QString>("gconfkey");
    QTest::addColumn<QString>("val1");
    QTest::addColumn<QString>("val2");
    QTest::addColumn<QString>("val3");
    QTest::addColumn<QString>("val4");
    QTest::addColumn<QString>("currentval");
    QTest::addColumn<QString>("uistring");

    QTest::newRow("") << 
        "/meegotouch/input_feedback/volume/priority2/volume/pulse" << 
        "off" << "low" << "medium" << "high" <<
        "off" <<
        "qtn_comm_settings_off" + translatedSuffix;
    
    QTest::newRow("") << 
        "/meegotouch/input_feedback/volume/priority2/volume/pulse" << 
        "off" << "low" << "medium" << "high" <<
        "low" <<
        "qtn_sond_level_1" + translatedSuffix;
    
    QTest::newRow("") << 
        "/meegotouch/input_feedback/volume/priority2/volume/pulse" << 
        "off" << "low" << "medium" << "high" <<
        "medium" <<
        "qtn_sond_level_2" + translatedSuffix;
    
    QTest::newRow("") << 
        "/meegotouch/input_feedback/volume/priority2/volume/pulse" << 
        "off" << "low" << "medium" << "high" <<
        "high" <<
        "qtn_sond_level_3" + translatedSuffix;
}

/*!
 * This test will create a combo box with four possible values and will emulate
 * a GConf database change. Then the test will check if the combobox follows the
 * emulated GConf change and show the UI string it supposed to show.
 */
void
Ut_GConfStringComboTests::gconfstringcomboChangedInGConf()
{
    QStringList  list;

    QFETCH(QString, gconfkey);
    QFETCH(QString, val1);
    QFETCH(QString, val2);
    QFETCH(QString, val3);
    QFETCH(QString, val4);
    QFETCH(QString, currentval);
    QFETCH(QString, uistring);

    /*
     * Possible values list.
     */
    list.push_back(val1);
    list.push_back(val2);
    list.push_back(val3);
    list.push_back(val4);

    /*
     * Creating the object with the given GConf key and possible value list.
     */
    GConfStringCombo gcsc (gconfkey, list);

    /*
     * Emulating a GConf database change and checking if we get the UI string
     * back.
     */
    _setGConfReturnString (gconfkey, currentval, &gcsc.m_val);
    QCOMPARE(gcsc.currentText(), uistring);
}

void
Ut_GConfStringComboTests::gconfstringcomboCurrentIndexChanged_data()
{
    QTest::addColumn<QString>("gconfkey");
    QTest::addColumn<QString>("val1");
    QTest::addColumn<QString>("val2");
    QTest::addColumn<QString>("val3");
    QTest::addColumn<QString>("val4");

    QTest::newRow("") << 
        "/meegotouch/input_feedback/volume/priority2/volume/pulse" << 
        "off" << "low" << "medium" << "high"; 
}

/*!
 * This test will emulate the use case when the user selects a new item from the
 * combobox. The test will set the current index for the combobox and watch the
 * simulated gconf_client_set_string() function to see if the GConf database
 * is changed the right way.
 */
void
Ut_GConfStringComboTests::gconfstringcomboCurrentIndexChanged()
{
    QFETCH(QString, gconfkey);
    QFETCH(QString, val1);
    QFETCH(QString, val2);
    QFETCH(QString, val3);
    QFETCH(QString, val4);

    QStringList  list;
    list.push_back(val1);
    list.push_back(val2);
    list.push_back(val3);
    list.push_back(val4);
      
    GConfStringCombo  gcsc (gconfkey, list);

    for (int n = 0; n < 4; ++n) {
        SYS_WARNING ("Changing to index %d", n);
        gcsc.setCurrentIndex (n); 
        QCOMPARE (lastChangedKey, gconfkey); 
        QCOMPARE (lastStringValue, list[n]); 
    }
}

void
Ut_GConfStringComboTests::gconfstringcomboRetranslateUi_data()
{
      QTest::addColumn<QString>("gconfkey");
      QTest::addColumn<QString>("title");
      QTest::addColumn<QString>("val1");
      QTest::addColumn<QString>("val2");
      QTest::addColumn<QString>("val3");
      QTest::addColumn<QString>("val4");
      QTest::addColumn<QString>("uistring1");
      QTest::addColumn<QString>("uistring2");
      QTest::addColumn<QString>("uistring3");
      QTest::addColumn<QString>("uistring4");

      /*
       * FIXME: We shoudl add more rows...
       */
      QTest::newRow("") << 
          "/meegotouch/input_feedback/volume/priority2/volume/pulse" << 
          "qtn_sond_touch_screen" + translatedSuffix <<
          "off" << "low" << "medium" << "high" <<
          "qtn_comm_settings_off" + translatedSuffix <<
          "qtn_sond_level_1" + translatedSuffix <<
          "qtn_sond_level_2" + translatedSuffix <<
          "qtn_sond_level_3" + translatedSuffix;
}

/*!
 * This test will create a combo with a possible value list, then it will
 * emulate a current value set in the GConf database and call the
 * retranslateUi() method of the combobox to see if the proper UI string is
 * set as current text even if the current value is not contained in the
 * possible value list.
 * This test is enhanced to check the title of the combobox.
 */
void
Ut_GConfStringComboTests::gconfstringcomboRetranslateUi()
{
    QStringList  list;
    QStringList  uistrings;
    QStringList  possiblevalues;

    QFETCH(QString, gconfkey);
    QFETCH(QString, title);
    QFETCH(QString, val1);
    QFETCH(QString, val2);
    QFETCH(QString, val3);
    QFETCH(QString, val4);
    possiblevalues <<
        val1 <<
        val2 <<
        val3 <<
        val4;

    // Intentional, only one item, the combobox will be adding the remainder.
    list.push_back(val1);

      
    QFETCH(QString, uistring1);
    QFETCH(QString, uistring2);
    QFETCH(QString, uistring3);
    QFETCH(QString, uistring4);
    uistrings << 
        uistring1 << 
        uistring2 << 
        uistring3 << 
        uistring4;


    GConfStringCombo  gcsc (gconfkey, list);
    QAbstractItemModel *model = gcsc.itemModel();

    for (int i = 0; i < uistrings.size(); ++i) {
        gcsc.m_val.m_val = possiblevalues[i];
        gcsc.retranslateUi();
        QCOMPARE (gcsc.currentText(), uistrings[i]);
        QCOMPARE (gcsc.title(), title);
    } 
}

QTEST_APPLESS_MAIN(Ut_GConfStringComboTests)
