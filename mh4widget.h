/*
 * mh4
 * Heroes of Might and Magic IV Ressources Explorer and Modifier
 *
 * Author: AKUHAK and Olivier Soares
 * olivier@etranges-libellules.fr
 *
 */


#pragma once
#include <memory>
#ifndef __MH4WIDGET_H__
#define __MH4WIDGET_H__


#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <qlayout.h> 
#include <vector>
#include "mh4.h"

class QGridLayout;
class QTableView;
class QStandardItemModel;

// File type name and suffix
const int g_headerColumnCnt = 11;

enum HeaderColumnIdx
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
    explicit mh4widget(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~mh4widget ();
    std::vector<QModelIndex> getItemIdxs();

    std::unique_ptr<QGridLayout> m_pMainGrid;
    std::unique_ptr<QTableView> m_pListView;
    std::unique_ptr<QStandardItemModel> m_itemModel;
    QItemSelectionModel *m_selectionModel;
    ui32 m_NbItem;
};



#endif  // __MH4WIDGET_H__
