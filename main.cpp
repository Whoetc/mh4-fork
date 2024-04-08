/*
 * mh4
 * Heroes of Might and Magic IV Ressources Explorer and Modifier
 *
 * Author: AKUHAK and Olivier Soares
 * olivier@etranges-libellules.fr
 *
 */


#include "mh4window.h"
#include <qfileinfo.h>
#include <qapplication.h>
//#include <qlistbox.h>
 

// ------------------------------------------------------------------------------------------
// main
// ------------------------------------------------------------------------------------------
// Description:
//    Main function.
// ------------------------------------------------------------------------------------------
// In:
//    argc: number of arguments
//    argv: arguments list
//
// Out:
//    Error code
// ------------------------------------------------------------------------------------------
int main (int argc,char **argv)
{
  QApplication app( argc,argv );

	mh4Window window;
	window.show();
	
  return app.exec();
}
