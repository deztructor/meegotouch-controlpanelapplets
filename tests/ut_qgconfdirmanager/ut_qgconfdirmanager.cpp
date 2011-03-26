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
#include "ut_qgconfdirmanager.h"

#include <QString>
#include <QStringList>
#include "qgconfdirmanager.h"
#include <QDebug>

/******************************************************************************
 * Ut_QConfDirManager implementation.
 */

int   argc = 1;
char *argv[] = {
      (char *) "./ut_qconfdirmanager",
      NULL };

void
Ut_QConfDirManager::initTestCase()
{
      m_App = new MApplication(argc, argv);
}

void
Ut_QConfDirManager::cleanupTestCase()
{
}

class GConfListenNode
{
public:
	GConfListenNode(const QString &dirElement, GConfListenNode *parent, int refcount);
	QString m_dirElement;
	int m_refcount;
	bool m_listening;
	QList<GConfListenNode *> m_children;
	GConfListenNode *m_parent;
};

void Ut_QConfDirManager::qconfdirmanagerGConfListenNode ()
{
      GConfListenNode  cln  ((QString)"test",(GConfListenNode *) 0, 1);
      QCOMPARE( cln.m_refcount ,1 );
      QCOMPARE( cln.m_listening ,false );
      QCOMPARE( ( int)( (void *)cln.m_parent) , 0);
      QCOMPARE( cln.m_dirElement ,QString("test") );
}

void
Ut_QConfDirManager::qconfdirmanagerAddDir ()
{
      QGConfDirManager  cdm;
      cdm.addDir("/meegotouch/input_feedback/volume/priority2/volume");
      GConfListenNode * cln =   cdm.m_root;

      cln = cln->m_children.at(0);
      QStringList  list;
      list<< "meegotouch" << "input_feedback" <<  "volume";
      for (int  i= 0 ;i < 2; i ++)
      {
            QCOMPARE( cln->m_dirElement ,list.at(i) );
            cln = cln->m_children.at(0);
      }
}

void
Ut_QConfDirManager::qconfdirmanagerRmDir ()
{
      QGConfDirManager  cdm;
      cdm.addDir("/meegotouch/input_feedback/volume/priority2/volume");
      cdm.rmDir("/meegotouch/input_feedback/volume/priority2/volume");

      void *   ptr  =  & cdm;
      GConfListenNode * cln =  * ((GConfListenNode **) ptr);
      QCOMPARE( cln->m_children.size() , 0 );
}
void
Ut_QConfDirManager::qconfdirmanagerPrune()
{
      QGConfDirManager  cdm;
      cdm.addDir("/meegotouch/input_feedback/volume/priority2/volume");
      GConfListenNode * cln =   cdm.m_root;

      cln = cln->m_children.at(0);
}

void 
Ut_QConfDirManager::qconfdirmanagerUnsetListening()
{
      QGConfDirManager  cdm;
      cdm.addDir("/meegotouch/input_feedback/volume/priority2/volume");
      GConfListenNode * cln =   cdm.m_root;
      
      cln = cln->m_children.at(0);
      GConfListenNode * work = cln;
      
      for (int  i= 0 ;i < 2; i ++)
      {
            work->m_listening = true;      
            work = work->m_children.at(0);
      }
	
      cdm.dfsUnsetListening(cln);
      	
      for (int  i= 0 ;i < 2; i ++)
      {
          	QVERIFY( cln->m_listening == false );   
            cln = cln->m_children.at(0);
      }
}

QTEST_APPLESS_MAIN(Ut_QConfDirManager)
