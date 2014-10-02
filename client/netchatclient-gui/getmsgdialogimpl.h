/*
 *  getmsgdialogimpl.h
 *  netchatclient-gui
 *
 */
#ifndef GETMSGDIALOGIMPL
#define GETMSGDIALOGIMPL

#include "getmsgdialog.h"
#include <string>
#include <qstring.h>
#include <qtextedit.h>

using namespace std;

class GetMsgDialogImpl : public GetMsgDialog 
{
	public:
		GetMsgDialogImpl(string myScreenName_, QWidget* parent = 0, 
				 const char* name = 0, 
				 WFlags fl = WType_TopLevel );
		~GetMsgDialogImpl();
		
		void setMyCaption(string target);
		void sendText();
		void newIncomingText(string author, string text, QColor color = Qt::blue);
		
	private:
		string user;
		string myScreenName;
};

#endif

