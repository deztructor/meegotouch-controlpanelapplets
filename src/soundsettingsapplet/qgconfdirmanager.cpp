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

#include <QDebug>
#include <gconf/gconf-client.h>
#include "qgconfdirmanager.h"

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

GConfListenNode::GConfListenNode(const QString &dirElement, GConfListenNode *parent, int refcount):
	m_dirElement(dirElement),
	m_refcount(refcount),
	m_listening(false),
	m_parent(parent)
{
}

QGConfDirManager QGConfDirManager::privInstance;

QGConfDirManager &
QGConfDirManager::instance()
{
	return privInstance;
}

QGConfDirManager::QGConfDirManager():
	m_root(new GConfListenNode("", NULL, 1))
{
}

QGConfDirManager::~QGConfDirManager ()
{
    delete m_root;
}


void
QGConfDirManager::addDir(QStringList &lsDir)
{
	addDir(m_root, lsDir, 0, false);
}

void
QGConfDirManager::rmDir(QStringList &lsDir)
{
	rmDir(m_root, lsDir, 0);
}


void
QGConfDirManager::addDir(const QString &dir)
{
	QStringList ls = dir.split("/", QString::SkipEmptyParts);

	addDir(m_root, ls, 0, false);
}

void
QGConfDirManager::rmDir(const QString &dir)
{
	QStringList ls = dir.split("/", QString::SkipEmptyParts);

	rmDir(m_root, ls, 0);
}

void
gconf_add_dir(const QString &dir)
{
	GError *error = NULL;

	gconf_client_add_dir(gconf_client_get_default(), dir.toUtf8().constData(), GCONF_CLIENT_PRELOAD_NONE, &error);

	if (error) {
		qWarning() << "gconf_add_dir: Failed to add" << dir << ":" << QString(error->message);
		g_error_free(error);
		error = NULL;
	}
}

void
gconf_remove_dir(const QString &dir)
{
	GError *error = NULL;

	gconf_client_remove_dir(gconf_client_get_default(), dir.toUtf8().constData(), &error);

	if (error) {
		qWarning() << "gconf_remove_dir: Failed to remove" << dir << ":" << QString(error->message);
		g_error_free(error);
		error = NULL;
	}
}

void
QGConfDirManager::addDir(GConfListenNode *parent, QStringList &ls, int idx, bool someoneIsListening)
{
	if (idx < ls.size()) {
		GConfListenNode *continue_node = NULL;

		for (int Nix = 0 ; Nix < parent->m_children.size() ; Nix++) {
			if (parent->m_children[Nix]->m_dirElement == ls[idx]) {
				continue_node = parent->m_children[Nix];
				break;
			}
		}

		if (NULL == continue_node) {
			continue_node = new GConfListenNode(ls[idx], parent, 0);
			parent->m_children << continue_node;
		}

		if (continue_node != NULL)
			addDir(continue_node, ls, idx + 1, someoneIsListening || parent->m_listening);
	}
	else {
		if (!someoneIsListening) {
			gconf_add_dir(fullPath(parent));
			parent->m_listening = true;
		}
		parent->m_refcount++;
		for (int Nix = 0 ; Nix < parent->m_children.size() ; Nix++)
			dfsUnsetListening(parent->m_children[Nix]);
	}
}

void
QGConfDirManager::prune(GConfListenNode *parent)
{
	GConfListenNode *continue_node = NULL;

	if (0 == parent->m_refcount) {
		if (0 == parent->m_children.size()) {
			if (parent->m_parent) {
				parent->m_parent->m_children.removeAll(parent);
				continue_node = parent->m_parent;
			}
			delete parent;
		}
	}

	if (continue_node)
		prune(continue_node);
}

void
QGConfDirManager::rmDir(GConfListenNode *parent, QStringList &ls, int idx)
{
	if (idx < ls.size()) {
		for (int Nix = 0 ; Nix < parent->m_children.size() ; Nix++)
			if (parent->m_children[Nix]->m_dirElement == ls[idx])
				rmDir(parent->m_children[Nix], ls, idx + 1);
	}
	else
	if (parent->m_refcount > 0) {
		parent->m_refcount--;
		if (parent->m_listening) {
			gconf_remove_dir(fullPath(parent));
			parent->m_listening = false;
			for (int Nix = 0 ; Nix < parent->m_children.size() ; Nix++) {
				gconf_add_dir(fullPath(parent->m_children[Nix]));
				parent->m_children[Nix]->m_listening = true;
			}
		}
		prune(parent);
	}
}

void
QGConfDirManager::dfsUnsetListening(GConfListenNode *node)
{
	for (int Nix = 0 ; Nix < node->m_children.size() ; Nix++)
		dfsUnsetListening(node->m_children[Nix]);
	if (node->m_listening) {
		gconf_remove_dir(fullPath(node));
		node->m_listening = false;
	}
}
#ifdef _Q_GCONF_DIR_MANAGER_DEBUG_
void
QGConfDirManager::dumpTree()
{
	for (int Nix = 0 ; Nix < m_root->m_children.size() ; Nix++)
		dfsDumpNode(m_root->m_children[Nix]);
	qDebug();
}

void
QGConfDirManager::dfsDumpNode(GConfListenNode *node)
{
	for (int Nix =  0 ; Nix < node->m_children.size() ; Nix++)
		dfsDumpNode(node->m_children[Nix]);
	qDebug() << "[" << node->m_refcount << "] [" << (node->m_listening ? " true" : "false") << "]" << fullPath(node).toUtf8().constData();
}
#endif /* _Q_GCONF_DIR_MANAGER_DEBUG_ */
QString
QGConfDirManager::fullPath(GConfListenNode *node)
{
	QStringList ls;

	for (; node != NULL ; node = node->m_parent)
		ls.prepend(node->m_dirElement);

	if (ls.size() > 0)
		ls.removeAt(0);

	return "/" + ls.join(QString("/"));
}
