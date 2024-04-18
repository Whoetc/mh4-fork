/*
 * mh4
 * Heroes of Might and Magic IV Ressources Explorer and Modifier
 *
 * Author: AKUHAK and Olivier Soares
 * olivier@etranges-libellules.fr
 *
 */


#pragma once 
#include <qabstractitemmodel.h>
#ifndef __MH4WINDOW_H__
#define __MH4WINDOW_H__


#include <qmainwindow.h>
#include <qnamespace.h>
#include "mh4.h"
#include "mh4widget.h"

// Window min size
#define MIN_WIDTH  320
#define MIN_HEIGHT 240


class mh4widget;
class H4RFile;


class mh4Window: public QMainWindow
{
  Q_OBJECT

  public:
  explicit mh4Window(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
  ~mh4Window ();

  protected:
    int m_curNumFile;
    std::unique_ptr<mh4widget> m_pView;
    std::unique_ptr<H4RFile> m_pH4RFile;

    QModelIndex _getCellIdx(int row, int column);
    void _setRowData(HeaderColumnIdx idx, QString data);

  protected slots:
    // Options menu functions
    void createSelection (void);
    void deleteSelection (void);
    void closeAllWindows (void);
    void extractSelection (void);
    void dumpSelection (void);
    void build (void);

    // Selection menu functions
    void selectAll (void);
    void selectNothing (void);
    void inverseSelection (void);
    void selectDataType (uint32_t dataType);
    void selectPointer (void);

    // About menu functions
    void about (void);
};


#endif // __MH4WINDOW_H__
