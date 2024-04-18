/*
 * mh4
 * Heroes of Might and Magic IV Ressources Explorer and Modifier
 *
 * Author: AKUHAK and Olivier Soares
 * olivier@etranges-libellules.fr
 *
 */


#include <array>
#include <malloc.h>
#include <qwidget.h>
#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <qlayout.h> 
#include <qlistview.h>
#include "mh4widget.h"


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
mh4widget::mh4widget(QWidget* parent, Qt::WindowFlags flags):
  QWidget(parent, flags),
  m_NbItem(0),
  m_pMainGrid(std::make_unique<QGridLayout>( this )),
  m_pListView(std::make_unique<QTableView>( this )),
  m_selectionModel(dynamic_cast<QItemSelectionModel*>(m_pListView->model()))
{

  m_itemModel = std::make_unique<QStandardItemModel>();

  QStringList headers;
  const std::array<std::string, g_headerColumnCnt> g_headerColumnsTxt = {"Name" , "Offset" , "Size", "Pointer name" ,
  "Nr" , "Type" , "Date" , "Path", "Unpacked size" ,
  "Comprassion" , "Debug size" };
  for (const std::string &header: g_headerColumnsTxt){
    headers << tr(header.data());
  }
  m_itemModel->parent();
  m_itemModel->setHorizontalHeaderLabels(headers);
  m_itemModel->setColumnCount(g_headerColumnCnt);

  m_pListView->setModel(m_itemModel.get());
  m_pListView->setSelectionMode( QTableView::ExtendedSelection );

  m_pMainGrid->addWidget( m_pListView.get(),0,0 );
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
mh4widget::~mh4widget ()
{
  if (m_NbItem > 0) {
    m_itemModel->clear();
    // No more item
    m_NbItem = 0;
  }
  m_pListView.reset();
  m_pMainGrid.reset();
}

std::vector<QModelIndex> mh4widget::getItemIdxs()
{
  std::vector<QModelIndex> idxs;
  for( int numFile=0;numFile < m_NbItem;numFile++ )
  {
    for (int clmnIdx = 0; clmnIdx < g_headerColumnCnt; clmnIdx++)
    {
      idxs.push_back(m_itemModel->index(numFile, clmnIdx));
    }
  }
  return idxs;
}
