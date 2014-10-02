/****************************************************************************
** Form interface generated from reading ui file 'getmsgdialog.ui'
**
** Created: Tue May 24 21:35:25 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GETMSGDIALOG_H
#define GETMSGDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTextEdit;
class QPushButton;

class GetMsgDialog : public QDialog
{
    Q_OBJECT

public:
    GetMsgDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~GetMsgDialog();

    QTextEdit* incomingTextEdit;
    QTextEdit* msgText;
    QPushButton* sendbt;
    QPushButton* buttonCancel;

public slots:
    virtual void sendText();

protected:
    QVBoxLayout* layout4;
    QHBoxLayout* layout3;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // GETMSGDIALOG_H
