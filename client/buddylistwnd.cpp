/****************************************************************************
** Form implementation generated from reading ui file 'buddylistwnd.ui'
**
** Created: Tue May 24 21:35:44 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "buddylistwnd.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include "buddylistwnd.ui.h"

/*
 *  Constructs a buddyListWnd as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
buddyListWnd::buddyListWnd( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "buddyListWnd" );
    setMinimumSize( QSize( 21, 85 ) );
    setMaximumSize( QSize( 250, 490 ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    msgBt = new QPushButton( centralWidget(), "msgBt" );
    msgBt->setGeometry( QRect( 20, 389, 71, 31 ) );

    xferFileBt = new QPushButton( centralWidget(), "xferFileBt" );
    xferFileBt->setGeometry( QRect( 100, 390, 110, 31 ) );

    buddyListBox = new QListBox( centralWidget(), "buddyListBox" );
    buddyListBox->setGeometry( QRect( 20, 19, 191, 361 ) );

    // actions
    msgUser = new QAction( this, "msgUser" );
    xferFileToUser = new QAction( this, "xferFileToUser" );
    menuSigninAction = new QAction( this, "menuSigninAction" );
    fileExitAction = new QAction( this, "fileExitAction" );


    // toolbars


    // menubar
    MenuBarEditor = new QMenuBar( this, "MenuBarEditor" );


    File = new QPopupMenu( this );
    menuSigninAction->addTo( File );
    fileExitAction->addTo( File );
    MenuBarEditor->insertItem( QString(""), File, 1 );

    languageChange();
    resize( QSize(250, 490).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( msgBt, SIGNAL( clicked() ), msgUser, SLOT( activate() ) );
    connect( xferFileBt, SIGNAL( clicked() ), xferFileToUser, SLOT( activate() ) );
    connect( buddyListBox, SIGNAL( doubleClicked(QListBoxItem*) ), msgUser, SLOT( activate() ) );
    connect( msgUser, SIGNAL( activated() ), this, SLOT( msgUserNow() ) );
    connect( xferFileToUser, SIGNAL( activated() ), this, SLOT( xferFileNow() ) );
    connect( menuSigninAction, SIGNAL( activated() ), this, SLOT( menuSigninAction_activated() ) );
    connect( fileExitAction, SIGNAL( activated() ), this, SLOT( close() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
buddyListWnd::~buddyListWnd()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void buddyListWnd::languageChange()
{
    setCaption( tr( "Buddy List" ) );
    msgBt->setText( tr( "Msg" ) );
    xferFileBt->setText( tr( "Transfer File" ) );
    xferFileToUser->setText( tr( "Action" ) );
    menuSigninAction->setText( tr( "Signin" ) );
    menuSigninAction->setMenuText( tr( "Signin" ) );
    fileExitAction->setText( tr( "Exit" ) );
    fileExitAction->setMenuText( tr( "Exit" ) );
    if (MenuBarEditor->findItem(1))
        MenuBarEditor->findItem(1)->setText( tr( "File" ) );
}

