/*
 * mh4
 * Heroes of Might and Magic IV Ressources Explorer and Modifier
 *
 * Author: AKUHAK and Olivier Soares
 * olivier@etranges-libellules.fr
 *
 */


#include <cstdint>
#include <cstring>
#include <qdatetime.h>
#include <qapplication.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qtableview.h>
#include <qfiledialog.h>
#include <qprogressdialog.h>
#include <qpushbutton.h>
#include <qstandarditemmodel.h>
#include "mh4window.h"
#include "mh4widget.h"
#include "mh4.h"


// ------------------------------------------------------------------------------------------
// mh4Window::mh4Window
// ------------------------------------------------------------------------------------------
// Description:
//    Constructor.
// ------------------------------------------------------------------------------------------
// In:
//    parent: parent widget
//    f     : flag
//
// Out:
//
// ------------------------------------------------------------------------------------------


mh4Window::mh4Window(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags), m_curNumFile(0), m_pView(std::make_unique<mh4widget>(this)),m_pH4RFile(std::make_unique<H4RFile>())  {
    setWindowTitle(MH4_DESC);

    m_pView->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    setCentralWidget(m_pView.get());

    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    // Options menu
    QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));
    // Selection menu
    QMenu *selectionMenu = menuBar()->addMenu(tr("&Selection"));
    // About menu
    QMenu *aboutMenu = menuBar()->addMenu(tr("&About"));

    // *********
    // File menu
    // *********

    // Open menu
    fileMenu->addAction(tr("Open a h4r file"), this, SLOT(createSelection()));
    // Close menu
    fileMenu->addAction(tr("Close the current file"), this, SLOT(deleteSelection()));

    fileMenu->addSeparator();

    // Exit menu
    fileMenu->addAction(tr("Exit"), this, SLOT(closeAllWindows()));


    // ************
    // Options menu
    // ************

    // Extract menu
    optionsMenu->addAction(tr("Extract selected files"), this, SLOT(extractSelection()));
    // Dump menu
    optionsMenu->addAction(tr("Dump the current file"), this, SLOT(dumpSelection()));

    optionsMenu->addSeparator();

    // Build menu
    optionsMenu->addAction(tr("Build a h4l file"), this, SLOT(build()));


    // **************
    // Selection menu
    // **************

    // Select all menu
    selectionMenu->addAction(tr("Select all"), this, SLOT(selectAll()));
    // Select nothing menu
    selectionMenu->addAction(tr("Select nothing"), this, SLOT(selectNothing()));
    // Inverse selection menu
    selectionMenu->addAction(tr("Inverse selection"), this, SLOT(inverseSelection()));

 //   selectionMenu->addSeparator();
 //   for (uint16_t i = 0; i < NB_DATATYPE; i++) {
 //       const char *fileDataName = g_FileDataName[i];
 //       std::string header = std::string("Select ") + fileDataName;
 //       selectionMenu->addAction(tr(header.data()), this, SLOT(selectDataType(i)));
 //   };

    selectionMenu->addSeparator();

    // Select Pointer menu
 //   selectionMenu->addAction(tr("Select pointers"), this, SLOT(selectDataType(18)));


    // **********
    // About menu
    // **********

    // About menu
    aboutMenu->addAction(tr("About"), this, SLOT(about()));
}


// ------------------------------------------------------------------------------------------
// mh4Window::~mh4Window
// ------------------------------------------------------------------------------------------
// Description:
//    Destructor.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
mh4Window::~mh4Window() {
    m_pView.reset();
    m_pH4RFile.reset();
}


QModelIndex mh4Window::_getCellIdx(int row, int column){
      return m_pView->m_itemModel->index(row, column);
}

    void mh4Window::_setRowData(HeaderColumnIdx idx, QString data){
        m_pView->m_itemModel->setData(_getCellIdx(m_curNumFile,idx),data);
    }


// ------------------------------------------------------------------------------------------
// mh4Window::createSelection
// ------------------------------------------------------------------------------------------
// Description:
//    Create a selection of files to extract.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::createSelection(void) {
    QString fileNameString = QFileDialog::getOpenFileName(this, tr("Select a h4r file to open"), "/home",
                                                          tr("Heroes IV Ressources (*.h4r)"));
    if (fileNameString.isEmpty()) {
        return;
    }

    const ui32 nbFile = m_pH4RFile->scan(fileNameString.toLatin1().data());
    if (nbFile < 1) {
        QMessageBox::warning(this, "mh4", "No file to extract !");
        return;
    }

    QString info;
    QString  info3;
    QString info4;
    QDateTime info2;
    QDate qdate;
    QTime qtime;
    ui32 iiiint = 0;
    ui32 iint = 0;
    auto mas1 = std::make_unique<ui32[]>(nbFile);
    auto mas2 = std::make_unique<ui32[]>(nbFile);

    for (ui32 i = 0; i < nbFile; i++) {
        info.setNum(m_pH4RFile->m_pOffset[i]);
        mas1[i] = info.toInt();
        mas2[i] = i;
    }
    for (ui32 numFile1 = 0; numFile1 < nbFile; numFile1++) {
        for (ui32 numFile2 = numFile1 + 1; numFile2 < nbFile; numFile2++) {
            if (mas1[numFile1] > mas1[numFile2]) {
                iiiint = mas1[numFile1];
                mas1[numFile1] = mas1[numFile2];
                mas1[numFile2] = iiiint;
                iiiint = mas2[numFile1];
                mas2[numFile1] = mas2[numFile2];
                mas2[numFile2] = iiiint;
            }
        }
    }
    for (ui32 numFile4 = 0; numFile4 < nbFile - 1; numFile4++) {
        iint = mas2[numFile4];
        iiiint = mas2[numFile4 + 1];
        info.setNum(m_pH4RFile->m_pOffset[iiiint]);
        info3.setNum(m_pH4RFile->m_pOffset[iint]);
        info4.setNum(m_pH4RFile->m_pSize[iint]);
        mas1[iint] = info.toInt() - info3.toInt() - info4.toInt();
        if (numFile4 == nbFile - 2) {
            mas1[iiiint] = 0;
        }
    }

    m_pView->m_itemModel->setRowCount(nbFile);
    for (int numFile = 0; numFile < nbFile; numFile++) {
        m_curNumFile = numFile;
        // File name
	_setRowData(NAME, QString(m_pH4RFile->m_ppName[numFile]));
        // File offset
        info.setNum(m_pH4RFile->m_pOffset[numFile]);
        _setRowData(OFFSET, info.rightJustified(11, ' '));
        // File size (zipped)
        info.setNum(m_pH4RFile->m_pSize[numFile]);
	_setRowData(SIZE, info.rightJustified(8, ' '));

//        // File point
//        info = QString(m_pH4RFile->m_ppPointer[numFile]);
//        item = new QStandardItem(info);
//        row->setChild(numFile, 3, item);

        // File number
        info.setNum(numFile);
        _setRowData(NR, info.rightJustified(5, '0'));
        // File type
        info = QString(g_FileDataName[m_pH4RFile->m_pDataType[numFile]]);
        _setRowData(TYPE, info);
        // File date
        info2.setTime_t(m_pH4RFile->m_pTime[numFile]);
        qdate = info2.date();
        iiiint = qdate.year();
        info3 = info.setNum(iiiint) + '.';
        iiiint = qdate.month();
        if (iiiint < 10) {
            info3 += '0';
        }
        info3 += info.setNum(iiiint) + '.';
        iiiint = qdate.day();
        if (iiiint < 10) {
            info3 += '0';
        }
        info3 += info.setNum(iiiint) + ' ';
        qtime = info2.time();
        iiiint = qtime.hour();
        if (iiiint < 10) {
            info3 += '0';
        }
        info3 += info.setNum(iiiint) + ':';
        iiiint = qtime.minute();
        if (iiiint < 10) {
            info3 += '0';
        }
        info3 += info.setNum(iiiint) + ':';
        iiiint = qtime.second();
        if (iiiint < 10) {
            info3 += '0';
        }
        info3 += info.setNum(iiiint);
        _setRowData(DATE, info3);

        // File path
        info = QString(m_pH4RFile->m_ppPath[numFile]);
        _setRowData(PATH, info);

        // File size (unzipped)
        info.setNum(m_pH4RFile->m_pUnpSize[numFile]);
        _setRowData(UNPK_SIZE, info.rightJustified(8, ' '));

        // Gzip value
        info3 = info.setNum(m_pH4RFile->m_pCompr[numFile]);
        info.setNum(m_pH4RFile->m_pCompr[numFile]);
        if (info == "3")
            info3 = "Yes";
        else
            info3 = "No";
        _setRowData(COMPRASSION, info3);

        info.setNum(mas1[numFile]);
        if (mas1[numFile] > 0)
            info = info.rightJustified(8, ' ');
        else
            info = '\0';

        _setRowData(DBG_SIZE, info);
    }
}


// ------------------------------------------------------------------------------------------
// mh4Window::deleteSelection
// ------------------------------------------------------------------------------------------
// Description:
//    Delete the selection of files to extract.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::deleteSelection(void) {
    m_pH4RFile.reset();
}


// ------------------------------------------------------------------------------------------
// mh4Window::extractSelection
// ------------------------------------------------------------------------------------------
// Description:
//    Extract the selection.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::extractSelection(void) {
    // No h4r file
    if (!m_pH4RFile) {
        QMessageBox::warning(this, "Nothing to extract", "Open an h4r file first !");
        return;
    }
    // We check the file to extract
    ui32 nbFileToExtract = 0;
    for (ui32 i = 0; i < m_pView->m_NbItem; i++) {
        m_pH4RFile->m_pToExtract[i] = false;
        for (QModelIndex &idx: m_pView->getItemIdxs()) {
            if (idx.row() == i && m_pView->m_selectionModel->isSelected(idx)) {
                m_pH4RFile->m_pToExtract[i] = true;
                break;
            }
        }

        if (m_pH4RFile->m_pToExtract[i]) {
            nbFileToExtract++;
        }
        // else
        // {
        // if( m_pH4RFile->m_pSize[i] == 0 )
        // {
        // nbFileToExtract++;
        // }
        // }
    }

    // Nothing is selected
    if (nbFileToExtract == 0) {
        QMessageBox::warning(this, "Nothing is selected", "Select the files you want to extract !");
        return;
    }

    FILE *srcFile = fopen(m_pH4RFile->m_szH4rFileName, "rb");
    if (!srcFile) {
        QMessageBox::warning(this, "Extraction error", "Impossible to open the h4r file !");
        return;
    }

    QString dirString = QFileDialog::getExistingDirectory(this, "Where do you want to put the extracted files",
                                                          QString::null, QFileDialog::ShowDirsOnly);
    if (dirString.isEmpty()) {
        return;
    }

    char *dirName = dirString.toLatin1().data();

    char extractedFileName[MAX_FILENAME_LENGTH];

    // We memorize in a file the list of all extracted files
    char lstFileName[MAX_FILENAME_LENGTH], tmp[MAX_FILENAME_LENGTH];
    strcpy(tmp, m_pH4RFile->m_szH4rFileName);
    getFileName(tmp);
    strcpy(lstFileName, dirName);
    strcat(lstFileName, tmp);
    removeFileNameSuffix(lstFileName);
    strcat(lstFileName, ".h4l");

    FILE *lstFile = fopen(lstFileName, "wt");
    strcpy(tmp, m_pH4RFile->m_szH4rFileName);
    getFileName(tmp);
    fprintf(lstFile, "[H4R File List]\n%s\n%d\n", tmp, nbFileToExtract);

    QProgressDialog *pProgressBar = new QProgressDialog("Extract", "Cancel", 0, nbFileToExtract, this);
    pProgressBar->setLabelText("Please Wait");

    ui32 nbExtractedFile = 0;
    for (ui32 numFile = 0; numFile < m_pH4RFile->m_NbFile; numFile++) {
        if (m_pH4RFile->extract(srcFile, numFile, dirName, extractedFileName)) {
            if (m_pH4RFile->m_pSize[numFile] == 0) {
                fprintf(lstFile, "%s%c", m_pH4RFile->m_ppName[numFile], '|');
                fprintf(lstFile, "%s\n", m_pH4RFile->m_ppPointer[numFile]);
            } else {
                getFileName(extractedFileName);
                fprintf(lstFile, "%s\n", extractedFileName);
            }
            nbExtractedFile++;
            pProgressBar->setValue(nbExtractedFile);
        }

        // if ( m_pH4RFile->m_pSize[numFile] == 0 )
        // {
        // fprintf( lstFile,"%s%c",m_pH4RFile->m_ppName[numFile], '|');
        // fprntf( lstFile,"%s\n",m_pH4RFile->m_ppPath[numFile] );
        // nbExtractedFile++;
        // pProgressBar->setProgress( nbExtractedFile );
        // }

        if (pProgressBar->wasCanceled()) {
            QMessageBox::warning(this, "File not build", "The build has been stopped by the user !");
            delete pProgressBar;
            return;
        }
    }
    delete pProgressBar;

    fprintf(lstFile, "[End]");
    fclose(srcFile);
    fclose(lstFile);

    QString stringNbExtractedFile;
    stringNbExtractedFile.setNum(nbExtractedFile);

    QString stringNbFile;
    stringNbFile.setNum(m_pH4RFile->m_NbFile);

    QMessageBox::information(this, "Files extracted",
                             stringNbExtractedFile + " / " + stringNbFile + " files were extracted");
}

void mh4Window::closeAllWindows(void) { QApplication::closeAllWindows(); }


// ------------------------------------------------------------------------------------------
// mh4Window::dumpSelection
// ------------------------------------------------------------------------------------------
// Description:
//    Dump the selection.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::dumpSelection(void) {
    // No h4r file
    if (!m_pH4RFile) {
        QMessageBox::warning(this, "Nothing to extract", "Open an h4r file first !");
        return;
    }

    QString dirString = QFileDialog::getExistingDirectory(this, "Where do you want to put the dump file", QString::null,
                                                          QFileDialog::ShowDirsOnly);
    if (dirString.isEmpty()) {
        return;
    }

    char *dirName = dirString.toLatin1().data();

    // Dump
    if (m_pH4RFile->dump(dirName)) {
        QMessageBox::information(this, "File dumped", "The file was dumped");
    } else {
        QMessageBox::warning(this, "File not dumped", "Error during the dump of the file !");
    }
}


// ------------------------------------------------------------------------------------------
// mh4Window::build
// ------------------------------------------------------------------------------------------
// Description:
//    Build a list file.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::build(void) {
    // Choose the file to build
    QString fileNameString = QFileDialog::getOpenFileName(this, "Select a h4l file to build", "Heroes IV List (*.h4l)");

    if (fileNameString.isEmpty()) {
        return;
    }

    char *fileNameToBuild = fileNameString.toLatin1().data();
    char dirName[MAX_FILENAME_LENGTH];
    strcpy(dirName, fileNameToBuild);
    getFilePath(dirName);

    H4RFile h4rFile;

    // Scan the lst file
    ui32 tableSize;
    FILE *lstFile = h4rFile.scanList(fileNameToBuild, dirName, &tableSize);
    if (!lstFile) {
        QMessageBox::warning(this, "File not build", "Error in the lst file !");
        return;
    }

    QProgressDialog *pProgressBar = new QProgressDialog("Build h4r header", "Cancel", 0, h4rFile.m_NbFile, this);
    pProgressBar->setLabelText("Please Wait");

    // Scan and zip each file
    ui32 numFile;
    for (numFile = 0; numFile < h4rFile.m_NbFile; numFile++) {
        if (!h4rFile.scanBuild(lstFile, numFile, dirName, &tableSize)) {
            QMessageBox::warning(this, "File not build", "Error during the build of the file header !");
            delete pProgressBar;
            return;
        }
        if (pProgressBar->wasCanceled()) {
            QMessageBox::warning(this, "File not build", "The build has been stopped by the user !");
            delete pProgressBar;
            return;
        }
        pProgressBar->setValue(numFile + 1);
    }
    delete pProgressBar;

    fclose(lstFile);

    // Build the header of the h4r file
    FILE *h4rBuildFile = h4rFile.headerBuild(tableSize);
    if (!h4rBuildFile) {
        QMessageBox::warning(this, "File not build", "Error during the write of the file header !");
        return;
    }

    pProgressBar = new QProgressDialog("Build h4r file", "Cancel", 0, h4rFile.m_NbFile, this);
    pProgressBar->setLabelText("Please Wait");

    // Copy the zipped files into the h4r file
    for (numFile = 0; numFile < h4rFile.m_NbFile; numFile++) {
        if (!h4rFile.dataBuild(h4rBuildFile, numFile, dirName)) {
            QMessageBox::warning(this, "File not build", "Error during the build of the file !");
            delete pProgressBar;
            return;
        }
        if (pProgressBar->wasCanceled()) {
            QMessageBox::warning(this, "File not build", "The build has been stopped by the user !");
            delete pProgressBar;
            return;
        }
        pProgressBar->setValue(numFile + 1);
    }
    delete pProgressBar;

    fclose(h4rBuildFile);

    // QMessageBox::information( this,"File build","The file was build !" );
    QMessageBox::information(this, "File build", "The file was build !");
}

// ------------------------------------------------------------------------------------------
// mh4Window::selectAll
// ------------------------------------------------------------------------------------------
// Description:
//    Select all files.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::selectAll(void) {
    if (!m_pH4RFile) {
        return;
    }

    for (ui32 numFile = 0; numFile < m_pH4RFile->m_NbFile; numFile++) {
        m_pView->m_pListView->selectAll();
    }
}


// ------------------------------------------------------------------------------------------
// mh4Window::selectAll
// ------------------------------------------------------------------------------------------
// Description:
//    Select all files.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::selectPointer(void) {
    if (!m_pH4RFile) {
        return;
    }

    for (ui32 numFile = 0; numFile < m_pH4RFile->m_NbFile; numFile++) {
        m_pView->m_selectionModel->select(m_pView->m_itemModel->index(numFile, HeaderColumnIdx::POINTER),
                                          QItemSelectionModel::Select);
    }
}


// ------------------------------------------------------------------------------------------
// mh4Window::selectNothing
// ------------------------------------------------------------------------------------------
// Description:
//    Select no file.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::selectNothing(void) {
    if (!m_pH4RFile) {
        return;
    }
    m_pView->m_selectionModel->clearSelection();
}


// ------------------------------------------------------------------------------------------
// mh4Window::inverseSelection
// ------------------------------------------------------------------------------------------
// Description:
//    Inverse the selection.
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::inverseSelection(void) {
    if (!m_pH4RFile) {
        return;
    }
    m_pView->m_selectionModel->select(m_pView->m_selectionModel->selection(), QItemSelectionModel::Toggle);
}


// ------------------------------------------------------------------------------------------
// mh4Window::selectDataType
// ------------------------------------------------------------------------------------------
// Description:
//    Select a kind of file.
// ------------------------------------------------------------------------------------------
// In:
//    dataType: data type to select
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::selectDataType(uint32_t dataType) {
    if (!m_pH4RFile) {
        return;
    }

    for (uint32_t numFile = 0; numFile < m_pH4RFile->m_NbFile; numFile++) {
        //QString dataTypeString = m_pView->m_pListViewItem[numFile]->child(0, HeaderColumnIdx::TYPE)->text();
        //if (dataTypeString == g_FileDataName[dataType]) {
        //    m_pView->m_selectionModel->select(m_pView->m_itemModel->index(numFile, 0), QItemSelectionModel::Select);
        //}
    }
}


// ------------------------------------------------------------------------------------------
// mh4Window::about
// ------------------------------------------------------------------------------------------
// Description:
//    What about mh4 ...
// ------------------------------------------------------------------------------------------
// In:
//
// Out:
//
// ------------------------------------------------------------------------------------------
void mh4Window::about(void) {
    QMessageBox::information(this, "About",
                             "Heroes of Might and Magic IV Ressources Explorer and Modifier\nBy AKUHAK and Olivier Soares\nolivier@etranges-libellules.fr\n\nRemake  by AKuHAK\nakuhak@gmail.com");
}
