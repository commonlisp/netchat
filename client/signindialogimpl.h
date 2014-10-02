
#ifndef SIGNINDIALOGIMPL
#define SIGNINDIALOGIMPL

#include "signindialog.h"
#include <vector>

#include <qstring.h>
#include <qstringlist.h>
#include <qlineedit.h>

using namespace std;

class SigninDialogImpl : public SigninDialog
{
	public:
		SigninDialogImpl( QWidget * parent = 0, const char * name = 0, bool modal = FALSE, WFlags f = 0 );
		QStringList getSigninInfo();
		
};

#endif