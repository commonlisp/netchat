/****************************************************************************
** Form interface generated from reading ui file 'buddylistwnd.ui'
**
** Created: Tue May 24 21:35:25 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef BUDDYLISTWND_H
#define BUDDYLISTWND_H

#include <qvariant.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QPushButton;
class QListBox;
class QListBoxItem;

class buddyListWnd : public QMainWindow
{
    Q_OBJECT

public:
    buddyListWnd( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~buddyListWnd();

    QPushButton* msgBt;
    QPushButton* xferFileBt;
    QListBox* buddyListBox;
    QMenuBar *MenuBarEditor;
    QPopupMenu *File;
    QAction* msgUser;
    QAction* xferFileToUser;
    QAction* menuSigninAction;
    QAction* fileExitAction;

public slots:
    virtual void msgUserNow();
    virtual void xferFileNow();
    virtual void menuSigninAction_activated();

protected:
    QString myScreenName;


protected slots:
    virtual void languageChange();

};

#endif // BUDDYLISTWND_H
