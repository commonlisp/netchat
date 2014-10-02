/*
 *  getmsgdialogimpl.cpp
 *  netchatclient-gui
 *
 */

#include "getmsgdialogimpl.h"
#include "netchat-client.h"

using namespace std;

GetMsgDialogImpl::GetMsgDialogImpl(string myScreenName_, QWidget* parent, 
				   const char* name, WFlags fl )
  : GetMsgDialog(parent, name, false, fl), myScreenName(myScreenName_)
{
	incomingTextEdit->setReadOnly(true);
}

GetMsgDialogImpl::~GetMsgDialogImpl()
{
}

void GetMsgDialogImpl::setMyCaption(string target)
{
	user = target;
	target = "Message " + target;
	QString cpt(target);
	setCaption(cpt);	
}

void GetMsgDialogImpl::sendText()
{
	QString txt = msgText->text();
	msgText->clear();
	newIncomingText(myScreenName, txt, Qt::red); 
	string msg = txt.ascii();
	sendIM(user, msg);
}

void GetMsgDialogImpl::newIncomingText(string author, string text, QColor color) 
{
	QColor oldColor = incomingTextEdit->color();
	incomingTextEdit->setColor(color);
	incomingTextEdit->append(author + ": " + text);
	incomingTextEdit->setColor(oldColor);
	incomingTextEdit->scrollToBottom();
}
