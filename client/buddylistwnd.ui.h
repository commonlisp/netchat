/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include "signindialogimpl.h"
#include "netchat-client.h"

void buddyListWnd::msgUserNow()
{

}


void buddyListWnd::xferFileNow()
{

}



void buddyListWnd::menuSigninAction_activated()
{
    
    SigninDialogImpl dlg;
    if (dlg.exec()) {
	QStringList* qslp = new QStringList;
	*qslp = dlg.getSigninInfo();
	if (qslp->count() > 0) {
	    myScreenName = (*qslp)[0];
	}
	pthread_t shandleT;
	pthread_create(&shandleT, NULL, ( void* (*) (void*)) handleServerConnection, qslp);
	
	menuSigninAction->setDisabled(true);
    }
}
