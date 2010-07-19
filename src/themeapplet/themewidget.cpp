/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themewidget.h"
#include "themedescriptor.h"
#include "themelistmodel.h"
#include "themecellcreator.h"
#include "themedialog.h"

#include <QGraphicsLinearLayout>
#include <MSortFilterProxyModel>

#include <MLayout>
#include <MTextEdit>
#include <MList>
#include <MListFilter>
#include <MApplication>
#include <MApplicationWindow>

#define DEBUG
#include "../debug.h"

static const char *oviCommand = "webwidgetrunner /usr/share/webwidgets/applications/d34177b1c241ea44cb132005b63ee6527c9f6040-wrt-widget.desktop -widgetparameter themes &";

#include <QTimer>

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
}

ThemeWidget::~ThemeWidget ()
{
}

void
ThemeWidget::createWidgets ()
{
    QGraphicsLinearLayout *mainLayout;

    m_List = new MList(this);
    m_List->setSelectionMode (MList::SingleSelection);


    ThemeCellCreator *cellCreator = new ThemeCellCreator();
    m_List->setCellCreator(cellCreator);
    // This function will create the m_LiveFilterEditor widget.
    readLocalThemes ();

    /*
     * An item to activate the OVI link.
     */
    m_OviItem = new MContentItem(MContentItem::IconAndSingleTextLabel);
    m_OviItem->setImageID ("icon-m-common-ovi");
    m_OviItem->setObjectName("OviItem");

    connect (m_OviItem, SIGNAL(clicked()),
            this, SLOT(oviActivated()));

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    
    mainLayout->addItem (m_LiveFilterEditor);
    mainLayout->addItem (m_OviItem);
    mainLayout->addItem (m_List);

    connect (m_LiveFilterEditor, SIGNAL(textChanged()),
            this, SLOT(textChanged ()));
    this->setLayout (mainLayout);

    retranslateUi ();

    mainLayout->insertItem(0, m_LiveFilterEditor);
    m_LiveFilterEditor->setVisible(true);
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
    /*
     * Creating the model and connecting it to the businesslogic so we can show
     * the spinner while the theme change is in progress.
     */
    m_ThemeListModel = new ThemeListModel (this);
    SYS_DEBUG ("*** m_ThemeListModel = %p", m_ThemeListModel);

    if (m_ThemeBusinessLogic) {
        connect (m_ThemeBusinessLogic, SIGNAL(themeChangeStarted(QString)),
                m_ThemeListModel, SLOT(themeChangeStarted(QString)));
        connect (m_ThemeBusinessLogic, SIGNAL(themeChanged(QString)),
                m_ThemeListModel, SLOT(themeChanged(QString)));
    }

    m_ThemeDescList = m_ThemeBusinessLogic->availableThemes ();
    m_ThemeListModel->setThemeList(m_ThemeDescList);

#if 0
    m_Proxy = new MSortFilterProxyModel();
    m_Proxy->setSortRole (ThemeListModel::SearchRole);
    m_Proxy->setFilterRole (ThemeListModel::SearchRole);
    m_Proxy->setSourceModel (m_ThemeListModel);
    m_List->setItemModel (m_Proxy);
#endif
    /*
     * Enabling the live filter feature for the list. From this moment on the
     * list will use a QSortFilterProxyModel object as model. 
     */
    m_List->filtering()->setEnabled (true);
    m_List->filtering()->setFilterRole (ThemeListModel::SearchRole);
    m_LiveFilterEditor = m_List->filtering()->editor();

    #if 1
    m_Proxy = m_List->filtering()->proxy();
    m_Proxy->setFilterRole (ThemeListModel::SearchRole);
    m_Proxy->setSourceModel (m_ThemeListModel);
    #endif
    

    //m_List->setItemModel (m_ThemeListModel);
    //m_List->setItemModel (proxy);


    connect(m_List, SIGNAL(itemClicked(QModelIndex)),
            this, SLOT(themeActivated(QModelIndex)));

    selectCurrentTheme ();
}

/*!
 * This slot selects the current theme from the theme list. We needed to
 * implement this feature as a slot, so we can select the current theme when the
 * user pressed the cancel button in the dialog. We need to go back to the
 * original theme then.
 */
void 
ThemeWidget::selectCurrentTheme ()
{
    QString        currentThemeCodeName; 
    QModelIndex    currentIndex;
    
    /*
     * Selecting the current theme from the list.
     */
    SYS_DEBUG ("Selecting current theme -----");
    currentThemeCodeName = m_ThemeBusinessLogic->currentThemeCodeName();
    currentIndex = m_ThemeListModel->indexOfCodeName(currentThemeCodeName);
    currentIndex = m_Proxy->mapFromSource (currentIndex);
    SYS_DEBUG ("*** currentIndex = %d", currentIndex.row());
    m_List->selectItem (currentIndex);
    m_List->scrollTo (currentIndex, MList::EnsureVisibleHint);
    SYS_DEBUG ("End. -------------------------");
}

void 
ThemeWidget::themeActivated (
        const QModelIndex &index)
{
    ThemeDialog      *dialog;
    QVariant          data;
    QString           codeName;
    ThemeDescriptor  *descr = 0;
   
    data = m_ThemeListModel->data(index, ThemeListModel::CodeNameRole);
    codeName = data.toString();
    /*
     * If the user selects the current theme we don't do anything.
     */
    if (codeName == m_ThemeBusinessLogic->currentThemeCodeName())
        return;

    /*
     * FIXME:  This is certainly too complicated here, the ThemeBusinessLogic
     * should do stuff like this.
     */
    foreach (ThemeDescriptor *d, m_ThemeDescList) {
        if (d->codeName() == codeName) {
            descr = d;
            break;
        }
    }

    if (descr == 0) {
        SYS_CRITICAL("codename not found: %s", SYS_STR(codeName));
        return;
    }

    dialog = new ThemeDialog (m_ThemeBusinessLogic, descr);
    connect (dialog, SIGNAL(themeChangeCancelled()),
            this, SLOT(selectCurrentTheme()));
    dialog->showDialog ();
}

void
ThemeWidget::oviActivated ()
{
    SYS_DEBUG ("Executing %s", oviCommand);
    system (oviCommand);
}

void 
ThemeWidget::textChanged ()
{
    SYS_DEBUG ("Text: %s", SYS_STR(m_LiveFilterEditor->text()));
    //m_Proxy->setFilterFixedString (m_LiveFilterEditor->text());
}
