/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <QString>
#include <QStringList>
#include "ut_qgconfdirmanager.h"
#include "qgconfdirmanager.h"
#include <QtTracker/Tracker>
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
      cdm.addDir("/meegotouch/input_feedback/2/volume");
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
      cdm.addDir("/meegotouch/input_feedback/2/volume");
      cdm.rmDir("/meegotouch/input_feedback/2/volume");

      void *   ptr  =  & cdm;
      GConfListenNode * cln =  * ((GConfListenNode **) ptr);
      QCOMPARE( cln->m_children.size() , 0 );
}
void
Ut_QConfDirManager::qconfdirmanagerPrune()
{
      QGConfDirManager  cdm;
      cdm.addDir("/meegotouch/input_feedback/2/volume");
      GConfListenNode * cln =   cdm.m_root;

      cln = cln->m_children.at(0);
}

void 
Ut_QConfDirManager::qconfdirmanagerUnsetListening()
{
      QGConfDirManager  cdm;
      cdm.addDir("/meegotouch/input_feedback/2/volume");
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
