/*
 * mh4
 * Heroes of Might and Magic IV Ressources Explorer and Modifier
 *
 * Author: AKUHAK and Olivier Soares
 * olivier@etranges-libellules.fr
 *
 */


#include <malloc.h>
#include <qwidget.h>
#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <qlayout.h> 
#include <qlistview.h>
#include "mh4widget.h"


const int g_headerColumnsCnt = 11;
const char* g_headerColumnsTxt[] = {"Name" , "Offset" , "Size", "Pointer name" ,
"Nr" , "Type" , "Date" , "Path", "Unpacked size" ,
"Comprassion" , "Debug size" };


// ------------------------------------------------------------------------------------------
// mh4widget::mh4widget
// ------------------------------------------------------------------------------------------
// Description:
//    Constructor.
// ------------------------------------------------------------------------------------------
// In:
//    parent: parent widget
//    name  : widget name
//
// Out:
//
// ------------------------------------------------------------------------------------------
mh4widget::mh4widget (QWidget *parent,const char *name): QWidget(parent){
  m_pMainGrid = new QGridLayout( this );
  m_pListView = new QTableView( this );

  QStandardItemModel *model = new QStandardItemModel;
  model->setColumnCount(g_headerColumnsCnt);
  QStringList headers;
  for (const char *header: g_headerColumnsTxt){
    headers << tr(header);
  }
  model->parent();
  model->setHorizontalHeaderLabels(headers);

  m_pListView->setModel(model);
  m_pListView->setSelectionMode( QTableView::ExtendedSelection );

  m_itemModel = model;
  m_selectionModel = dynamic_cast<QItemSelectionModel*>(m_pListView->model());


  m_pMainGrid->addWidget( m_pListView,0,0 );

  // No item
  m_NbItem = 0;
  m_pListViewItem = NULL;
}


// ------------------------------------------------------------------------------------------
// mh4widget::~mh4widget
// ------------------------------------------------------------------------------------------
// Description:
//    Destructor.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
mh4widget::~mh4widget (void)
{
  deleteListViewItem();
  delete m_pListView;
  delete m_pMainGrid;
}

std::vector<QModelIndex> mh4widget::getItemIdxs(void)
{
  std::vector<QModelIndex> idxs;
  for( ui32 numFile=0;numFile < m_NbItem;numFile++ )
  {
    for (int clmnIdx = 0; clmnIdx < g_headerColumnsCnt; clmnIdx++)
    {
      idxs.push_back(m_itemModel->index(numFile, clmnIdx));
    }
  }
  return idxs;
}


void mh4widget::createListViewItem (unsigned int nbItem)
{
  if( m_pListViewItem )
  {
    deleteListViewItem();
  }

  m_NbItem = nbItem;
  if( m_NbItem > 0 )
  {
    m_pListViewItem = new QStandardItem*[nbItem];
  }
}


// ------------------------------------------------------------------------------------------
// mh4widget::deleteListViewItem
// ------------------------------------------------------------------------------------------
// Description:
//    Delete the items of the list view.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4widget::deleteListViewItem (void)
{
  if( m_NbItem > 0 )
  {
    delete[] m_pListViewItem;
    m_itemModel->clear();

    // No more item
    m_NbItem = 0;
    m_pListViewItem = NULL;
  }
}
