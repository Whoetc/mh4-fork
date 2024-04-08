/*
 * mh4
 * Heroes of Might and Magic IV Ressources Explorer and Modifier
 *
 * Author: AKUHAK and Olivier Soares
 * olivier@etranges-libellules.fr
 *
 */


#pragma once
#ifndef __MH4WIDGET_H__
#define __MH4WIDGET_H__


#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <qlayout.h> 
#include "mh4.h"

class QGridLayout;
class QTableView;
class QStandardItemModel;

// File type name and suffix
const extern int g_headerColumnsCnt;
const extern char* g_headerColumnsTxt[];

enum HeaderColumn
{
  NAME = 0,
  OFFSET,
  SIZE,
  POINTER,
  NR,
  TYPE,
  DATE,
  PATH,
  UNPK_SIZE,
  COMPRASSION,
  DBG_SIZE
};

class mh4widget: public QWidget
{
  Q_OBJECT

  public:
    mh4widget (QWidget *parent,const char *name = NULL);
    ~mh4widget (void);
    void createListViewItem (ui32 nbItem);
    void deleteListViewItem (void);
    std::vector<QModelIndex> getItemIdxs(void);

    QGridLayout *m_pMainGrid;
    QTableView *m_pListView;
    QStandardItem **m_pListViewItem;
    QItemSelectionModel *m_selectionModel;
    QStandardItemModel *m_itemModel;
    ui32 m_NbItem;
};



#endif  // __MH4WIDGET_H__
