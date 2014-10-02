/*
 *  buddylistwndimpl.h
 *  netchat
 *
 */

#ifndef BUDDYLISTWNDIMPL_H
#define BUDDYLISTWNDIMPL_H 1

#include "buddylistwnd.h"
#include "signindialogimpl.h"
#include "getmsgdialogimpl.h"

#include <qlistbox.h>
#include <qapplication.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <vector>

using namespace std;

typedef QMap<QString, GetMsgDialogImpl*> DlgMap;

class BuddyListWndImpl : public buddyListWnd {
	Q_OBJECT
	
	public: 
		BuddyListWndImpl(QWidget* parent = 0, const char* name = 0, 
				 WFlags fl = WType_TopLevel );
		~BuddyListWndImpl();
		
		void refreshBuddyList(vector<string> names);
		void newMsgFrom(string user, string msg); 
	
	public slots:
		void xferFileNow();
		void msgUserNow();
		
	private:
		void init();
		void updateMsgWnd(string user);

		DlgMap myMap;
		
};
#endif
