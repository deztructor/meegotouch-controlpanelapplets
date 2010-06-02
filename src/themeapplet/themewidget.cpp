/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themewidget.h"
#include "themedescriptor.h"
#include "themelistmodel.h"
#include "themecellcreator.h"
#include "themedialog.h"

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MList>
#include <MApplication>
#include <MApplicationWindow>

#include "../debug.h"

ThemeWidget::ThemeWidget (
        ThemeBusinessLogic *themeBusinessLogic, 
        QGraphicsWidget    *parent) :
    DcpWidget (parent),
    m_ThemeBusinessLogic (themeBusinessLogic),
    m_ThemeListModel(0),
    m_List(0),
    m_OviItem(0)
{
    createWidgets ();
    retranslateUi ();
    readLocalThemes ();
}

ThemeWidget::~ThemeWidget ()
{
}

void
ThemeWidget::createWidgets ()
{
    QGraphicsLinearLayout *mainLayout;

    m_List = new MList();
    m_List->setSelectionMode(MList::SingleSelection);
    ThemeCellCreator *cellCreator = new ThemeCellCreator();
    m_List->setCellCreator(cellCreator);

    m_OviItem = new MContentItem(MContentItem::IconAndSingleTextLabel);
    const QPixmap *oviIcon = MTheme::pixmap("icon-m-common-ovi-store");
    m_OviItem->setPixmap(*oviIcon);
    m_OviItem->setObjectName("OviItem");

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->addItem (m_OviItem);
    mainLayout->addItem (m_List);

    this->setLayout (mainLayout);
}

void
ThemeWidget::retranslateUi ()
{
    //% "Get more from Ovi Store"
    m_OviItem->setTitle(qtTrId("qtn_teme_store"));
}

/**
 * If you consider to call this function multiple times
 * (for example because you want to update the theme list),
 * pay attention to freeing ThemeDescriptor objects and
 * m_ThemeListModel.
 *
 * Apparently multiple calls of setItemModel() with the same
 * model doesn't update MList.
 *
 * You also have to deal with disconnection and reconnection of
 * itemClicked() signal because MList::selectItem() emits that!
 */
void
ThemeWidget::readLocalThemes ()
{
    m_ThemeListModel = new ThemeListModel(this);
    m_ThemeDescList = m_ThemeBusinessLogic->availableThemes ();
    m_ThemeListModel->setThemeList(m_ThemeDescList);
    m_List->setItemModel(m_ThemeListModel);
    QString currentTheme = m_ThemeBusinessLogic->currentThemeCodeName();
    QModelIndex currentIndex = m_ThemeListModel->indexOfCodeName(currentTheme);
    m_List->selectItem(currentIndex);
    connect(m_List, SIGNAL(itemClicked(QModelIndex)),
            this, SLOT(themeActivated(QModelIndex)));
}


void 
ThemeWidget::themeActivated (const QModelIndex &index)
{
    qDebug() << "YYY";
    ThemeDialog *dialog;
    SYS_DEBUG ("Theme '%s' activated", SYS_STR(themeDescr->name()));

    QStringList row = m_ThemeListModel->data(index).value<QStringList>();
    QString codeName = row[ThemeColumnCodeName];
    ThemeDescriptor *descr = 0;
    foreach (ThemeDescriptor *d, m_ThemeDescList) {
        if (d->codeName() == codeName) {
            descr = d;
            break;
        }
    }

    if (descr == 0) {
        SYS_CRITICAL("codename not found: %s", codename.data());
        return;
    }

    dialog = new ThemeDialog (m_ThemeBusinessLogic, descr);
    dialog->showDialog ();
}

