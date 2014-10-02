/*
 *  buddylistwndimpl.cpp
 *  netchat
 *
 */

#include "buddylistwndimpl.h"
#include <iostream>
#include "netchat-client.h"

using namespace std;


BuddyListWndImpl::BuddyListWndImpl(QWidget* parent, const char* name, WFlags fl )
  : buddyListWnd(parent, name, fl)
{
	init();
}

BuddyListWndImpl::~BuddyListWndImpl() 
{
}

void BuddyListWndImpl::init()
{

}

void BuddyListWndImpl::refreshBuddyList(vector<string> v)
{
	QStringList lst;
	QListBoxItem* p = NULL;
		
	for (vector<string>::const_iterator itr = v.begin(); itr != v.end(); itr++) {
		QString qstr(*itr);
		if ((p = buddyListBox->findItem(qstr, Qt::ExactMatch)) == 0)
			buddyListBox->insertItem(qstr);
			
		lst += qstr;
	}
	
	for (unsigned int i = 0; i < buddyListBox->count(); i++) {
		bool found = false;
		for (QStringList::Iterator it = lst.begin(); it != lst.end(); ++it) {
			if (*it == buddyListBox->item(i)->text()) {
				found = true;
				break;
			}
		}
		
		if (!found) buddyListBox->removeItem(i);
	}
}

void BuddyListWndImpl::xferFileNow() 
{
	if (buddyListBox->selectedItem() != NULL) {
	QString filename = QFileDialog::getOpenFileName(
							QString::null, "Any file (*.*)", this,
							"file open", "NetChat-client -- File Open" );
	if ( !filename.isEmpty() )
		sendFile(buddyListBox->selectedItem()->text(), filename);
	else
		QMessageBox mbx ("Error!", "The file cannot be opened.", QMessageBox::Warning, QMessageBox::Ok, 
						 QMessageBox::NoButton, QMessageBox::NoButton, this);
	}
}

void BuddyListWndImpl::msgUserNow()
{
	if (buddyListBox->selectedItem() != NULL) {
		updateMsgWnd(buddyListBox->selectedItem()->text());
	} else {
	}
}

void BuddyListWndImpl::newMsgFrom(string user, string msg)
{
	updateMsgWnd(user);
	if (myMap.find(user) != myMap.end()) { 
	  cerr << "Found dialog for " << user << endl;
	  myMap[user]->newIncomingText(user, msg);
	} else {
	  cerr << "Cannot find dialog for " << user << endl;
	}
}

void BuddyListWndImpl::updateMsgWnd(string user)
{
  if (myMap.find(user) == myMap.end()) {
    cout << "BuddyListWndImpl::updateMsgWnd() cannot find user " << user << endl;
    GetMsgDialogImpl* dlg;
    if (this != NULL) {
      dlg = new GetMsgDialogImpl(myScreenName, this);
    } else {
      cerr << "this is NULL!" << endl;
      dlg = new GetMsgDialogImpl(myScreenName);
    }
    cout << "Made a new dlg" << endl;
    myMap[user] = dlg;
    cout << "Hashed the new dlg";
    dlg->setMyCaption(user);
    
    //buddyListBox->setSelected(buddyListBox->findItem(user), true);
    if (isVisible())
      dlg->show();
    cout << "New Dialog good to go!" << endl;
  } else {
    myMap[user]->show();
    myMap[user]->setActiveWindow();
  }
}
