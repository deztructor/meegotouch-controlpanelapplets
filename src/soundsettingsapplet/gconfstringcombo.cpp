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

#include <QDebug>
#include "gconfstringcombo.h"
#include "alerttoneappletmaps.h"

#define TO_STRING(string) ((string).toUtf8().constData())

//#define DEBUG
#define WARNING
#include "../debug.h"

GConfStringCombo::GConfStringCombo (
        const QString &gconfKey, 
        const QStringList &possibleValues, 
        QGraphicsItem *parent) :
	MComboBox(parent),
	m_val(gconfKey),
	m_possibleValues(possibleValues)
{
    SYS_DEBUG ("*** gconfKey = %s", SYS_STR(gconfKey));

	QAbstractItemModel *model = itemModel();
	model->insertColumn(ColumnGConfString);

    connect (this, SIGNAL(currentIndexChanged(int)), 
            this, SLOT(currentIndexChanged(int)));
	connect (&m_val, SIGNAL(changed()), 
            this, SLOT(changedInGConf()));

	retranslateUi();
}

void
GConfStringCombo::retranslateUi()
{
	QAbstractItemModel *model = itemModel();
	int                 Nix;
    int                 idx = -1;
    SYS_DEBUG ("");

    /*
     * Removing every row from the list.
     */
	model->removeRows(0, model->rowCount());

    /*
     * Setting the title of the combobox.
     */
	setProperty("title", AlertToneAppletMaps::mapToUiString(m_val.key()));

    /*
     * Fetching the current value.
     */
	QString gconfVal = m_val.value().toString();

    /*
     * Adding a row for every possible value with the UI string and the possible
     * value code name.
     */
	for (Nix = 0; Nix < m_possibleValues.size(); Nix++) {
		model->insertRow(model->rowCount());
		model->setData (
                model->index(Nix, ColumnUiString), 
                AlertToneAppletMaps::mapToUiString (m_possibleValues[Nix]));
		model->setData(
                model->index(Nix, ColumnGConfString), 
                QVariant(m_possibleValues[Nix]));

        /*
         * This will show us the current index.
         */
        if (gconfVal == m_possibleValues[Nix])
            idx = Nix;
	}

    if (idx != -1) {
        /*
         * We found the index, let's just select it.
         */
        setCurrentIndex (idx);
    } else if (!gconfVal.isEmpty()) {
        /*
         * We did not find the index, but we can add the value since the current
         * value is not null.
         * FIXME: This way we let the unsupported value go into the combobox,
         * but of course, it is the current value...
         */
        idx = model->rowCount();

		model->insertRow (idx);
		model->setData (
                model->index(idx, ColumnUiString),
                AlertToneAppletMaps::mapToUiString (gconfVal));

		model->setData (
                model->index(idx, ColumnGConfString), 
                QVariant(gconfVal));
        
        setCurrentIndex (idx);
    }
}

void
GConfStringCombo::changedInGConf()
{
	QAbstractItemModel *model = itemModel();
    
    /*
     * If the item is already shown as current item we don't need to do
     * anything.
     */
    if (currentIndex() >= 0 &&
            model->data (model->index(currentIndex(),
                    ColumnGConfString)) == m_val.value())
        return;

    /*
     * Let's see where is that item and let's select it.
     */
	for (int Nix = 0 ; Nix < count() ; Nix++) {
		if (model->data(model->index(Nix, ColumnGConfString)) == m_val.value()) {
            setCurrentIndex(Nix);
            break;
        }
    }
}

/*!
 * This slot will be activated when the user selects an item from the combo. The
 * method will set the value in GConf using the QGConfValue object.
 */
void
GConfStringCombo::currentIndexChanged(
        int index)
{
    SYS_DEBUG ("");
	if (index >= 0) {
		QAbstractItemModel *model = itemModel();
		m_val.set(model->data(model->index(index, ColumnGConfString)));
	}
}
