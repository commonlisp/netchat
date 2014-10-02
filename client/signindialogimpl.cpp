
#include "signindialogimpl.h"
#include <qapplication.h>

SigninDialogImpl::SigninDialogImpl( QWidget * parent, const char * name, bool modal, WFlags f)
  : SigninDialog(parent,name,TRUE,f) 
{
	if (!modal) this->setModal(false);
}

QStringList SigninDialogImpl::getSigninInfo()
{	
	QStringList info;
	
	info += loginInput->text();
	info += serverInput->text();
	
	return info; 
}