/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themewidget.h"
#include "themedescriptor.h"
#include "themelistmodel.h"
#include "themecellcreator.h"
#include "themedialog.h"

#include <MApplication>
#include <MApplicationWindow>
#include <QGraphicsLinearLayout>
#include <MTextEdit>
#include <MList>
#include <MListFilter>
#include <MSortFilterProxyModel>

#define DEBUG
#define WARNING
#include "../debug.h"

static const char *oviCommand = "webwidgetrunner /usr/share/webwidgets/applications/d34177b1c241ea44cb132005b63ee6527c9f6040-wrt-widget.desktop -widgetparameter themes &";

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
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout (Qt::Vertical);

    m_List = new MList();
    m_List->setObjectName ("ThemeList");

    m_CellCreator = new ThemeCellCreator;
    m_List->setCellCreator (m_CellCreator);
    m_List->setSelectionMode (MList::SingleSelection);


    // This function will create the m_LiveFilterEditor widget.
    readLocalThemes ();
    //m_List->setShowGroups(true);

    /*
     * An item to activate the OVI link.
     */
    m_OviItem = new MContentItem(MContentItem::IconAndSingleTextLabel);
    m_OviItem->setItemMode (MContentItem::Single);
    m_OviItem->setImageID ("icon-m-common-ovi");
    m_OviItem->setObjectName("OviItem");

    connect (m_OviItem, SIGNAL(clicked()),
            this, SLOT(oviActivated()));

    m_LiveFilterEditor->setParentLayoutItem (mainLayout);
    mainLayout->addItem (m_OviItem);
    mainLayout->addItem (m_List);

    connect (m_LiveFilterEditor, SIGNAL(textChanged()),
            this, SLOT(textChanged ()));
    connect (m_List, SIGNAL(panningStarted()),
            this, SLOT(hideEmptyTextEdit()));

    setLayout(mainLayout);

    retranslateUi ();
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
    m_ThemeListModel = new ThemeListModel (m_ThemeBusinessLogic);

    m_ThemeListModel->setObjectName ("ThemeListModel");
    SYS_DEBUG ("*** m_ThemeListModel = %p", m_ThemeListModel);
    m_List->setItemModel (m_ThemeListModel);

    if (m_ThemeBusinessLogic) {
        connect (m_ThemeBusinessLogic, SIGNAL(themeChangeStarted(QString)),
                m_ThemeListModel, SLOT(themeChangeStarted(QString)));
        connect (m_ThemeBusinessLogic, SIGNAL(themeChanged(QString)),
                m_ThemeListModel, SLOT(themeChanged(QString)));
    }

    /*
     * Enabling the live filter feature for the list. From this moment on the
     * list will use a QSortFilterProxyModel object as model. 
     */
    m_List->filtering()->setEnabled (true);
    m_List->filtering()->setFilterRole (ThemeListModel::SearchRole);

    m_Proxy = m_List->filtering()->proxy();
    m_Proxy->setSortRole (ThemeListModel::SearchRole);
    m_Proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    // Seems that the sort() method simply will not sort when the
    // ThemeListModel::SearchRole is used.
    m_Proxy->sort(Qt::DisplayRole);
    m_Proxy->setFilterKeyColumn(0);

    m_LiveFilterEditor = m_List->filtering()->editor();

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
    currentThemeCodeName = m_ThemeBusinessLogic->currentThemeCodeName();
    currentIndex = m_ThemeListModel->indexOfCodeName(currentThemeCodeName);
    currentIndex = m_Proxy->mapFromSource (currentIndex);
    m_List->selectItem (currentIndex);
    m_List->scrollTo (currentIndex, MList::EnsureVisibleHint);
}

void 
ThemeWidget::themeActivated (
        const QModelIndex &index)
{
    ThemeDialog      *dialog;
    QString           codeName;
    ThemeDescriptor  *descr = 0;
      
    SYS_WARNING ("*** index at %d, %d", index.row(), index.column());
    codeName = m_Proxy->data(index, ThemeListModel::CodeNameRole).toString();

    /*
     * If the user selects the current theme we don't do anything.
     */
    if (codeName == m_ThemeBusinessLogic->currentThemeCodeName())
        return;

    #if 0
    /*
     * For debugging purposes it is possible to leave out the dialog and change
     * the theme here.
     */
    m_ThemeBusinessLogic->changeTheme (codeName);
    return;
    #endif

    descr = m_ThemeBusinessLogic->themeByCodename (codeName);
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
    if (!m_List->filtering()->editor()->isOnDisplay()) {
        m_List->filtering()->editor()->show();
        m_List->filtering()->editor()->setFocus();
    }

    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(layout ());
    if (mainLayout)
    {
      if (m_LiveFilterEditor->text ().isEmpty () == true)
      {
          mainLayout->removeItem (m_LiveFilterEditor);
          m_LiveFilterEditor->setPos (QPointF (0.,-200.));
      }
      else
      {
          mainLayout->insertItem (0, m_LiveFilterEditor);
      }  
      mainLayout->invalidate ();
    }

    m_CellCreator->highlightByText (m_LiveFilterEditor->text());
    // Seems that the sort() method simply will not sort when the
    // ThemeListModel::SearchRole is used.
    m_Proxy->sort(Qt::DisplayRole);
    selectCurrentTheme ();
    m_ThemeListModel->refresh();

    update ();
}

void 
ThemeWidget::hideEmptyTextEdit ()
{
    if (m_List->filtering()->editor()->text().isEmpty())
        m_List->filtering()->editor()->hide();
}

