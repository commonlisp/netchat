/****************************************************************************
** Form interface generated from reading ui file 'signindialog.ui'
**
** Created: Tue May 24 21:35:25 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QLineEdit;

class SigninDialog : public QDialog
{
    Q_OBJECT

public:
    SigninDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SigninDialog();

    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QLineEdit* loginInput;
    QLineEdit* serverInput;

protected:
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // SIGNINDIALOG_H
