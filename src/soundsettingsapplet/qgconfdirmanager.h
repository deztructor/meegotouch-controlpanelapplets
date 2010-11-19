/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _Q_GCONF_DIR_MANAGER_H_
#define _Q_GCONF_DIR_MANAGER_H_

#include <QList>
#include <QString>
#include <QStringList>

//#define _Q_GCONF_DIR_MANAGER_DEBUG_

class GConfListenNode;

class QGConfDirManager
{
public:
	QGConfDirManager();
    ~QGConfDirManager ();
	void addDir(const QString &dir);
	void rmDir(const QString &dir);

	void addDir(QStringList &lsDir);
	void rmDir(QStringList &lsDir);
#ifdef _Q_GCONF_DIR_MANAGER_DEBUG_
	void dumpTree();
#endif /* _Q_GCONF_DIR_MANAGER_DEBUG_ */

	static QGConfDirManager &instance();

private:
	void addDir(GConfListenNode *parent, QStringList &ls, int idx, bool someoneIsListening);
	void rmDir(GConfListenNode *parent, QStringList &ls, int idx);
	void dfsUnsetListening(GConfListenNode *node);
	QString fullPath(GConfListenNode *node);
	void prune(GConfListenNode *parent);
#ifdef _Q_GCONF_DIR_MANAGER_DEBUG_
	void dfsDumpNode(GConfListenNode *node);
#endif /* _Q_GCONF_DIR_MANAGER_DEBUG_ */

	GConfListenNode *m_root;

	static QGConfDirManager privInstance;
#ifdef UNIT_TEST
    friend class Ut_QConfDirManager;
#endif
};

#endif /* !_Q_GCONF_DIR_MANAGER_H_ */
