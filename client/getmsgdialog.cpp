/****************************************************************************
** Form implementation generated from reading ui file 'getmsgdialog.ui'
**
** Created: Tue May 24 21:35:48 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "getmsgdialog.h"

#include <qvariant.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a GetMsgDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
GetMsgDialog::GetMsgDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "GetMsgDialog" );
    setSizeGripEnabled( TRUE );

    QWidget* privateLayoutWidget = new QWidget( this, "layout4" );
    privateLayoutWidget->setGeometry( QRect( 20, 10, 488, 250 ) );
    layout4 = new QVBoxLayout( privateLayoutWidget, 11, 6, "layout4"); 

    incomingTextEdit = new QTextEdit( privateLayoutWidget, "incomingTextEdit" );
    incomingTextEdit->setEnabled( TRUE );
    layout4->addWidget( incomingTextEdit );

    msgText = new QTextEdit( privateLayoutWidget, "msgText" );
    layout4->addWidget( msgText );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
    Horizontal_Spacing2 = new QSpacerItem( 320, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( Horizontal_Spacing2 );

    sendbt = new QPushButton( privateLayoutWidget, "sendbt" );
    layout3->addWidget( sendbt );

    buttonCancel = new QPushButton( privateLayoutWidget, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    layout3->addWidget( buttonCancel );
    layout4->addLayout( layout3 );
    languageChange();
    resize( QSize(533, 298).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( sendbt, SIGNAL( clicked() ), this, SLOT( sendText() ) );
    connect( msgText, SIGNAL( returnPressed() ), this, SLOT( sendText() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
GetMsgDialog::~GetMsgDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GetMsgDialog::languageChange()
{
    setCaption( tr( "Message User" ) );
    sendbt->setText( tr( "Send" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
}

void GetMsgDialog::sendText()
{
    qWarning( "GetMsgDialog::sendText(): Not implemented yet" );
}

