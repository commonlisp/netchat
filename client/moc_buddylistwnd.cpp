/****************************************************************************
** buddyListWnd meta object code from reading C++ file 'buddylistwnd.h'
**
** Created: Tue May 24 21:35:57 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "buddylistwnd.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *buddyListWnd::className() const
{
    return "buddyListWnd";
}

QMetaObject *buddyListWnd::metaObj = 0;
static QMetaObjectCleanUp cleanUp_buddyListWnd( "buddyListWnd", &buddyListWnd::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString buddyListWnd::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "buddyListWnd", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString buddyListWnd::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "buddyListWnd", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* buddyListWnd::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"msgUserNow", 0, 0 };
    static const QUMethod slot_1 = {"xferFileNow", 0, 0 };
    static const QUMethod slot_2 = {"menuSigninAction_activated", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "msgUserNow()", &slot_0, QMetaData::Public },
	{ "xferFileNow()", &slot_1, QMetaData::Public },
	{ "menuSigninAction_activated()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"buddyListWnd", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_buddyListWnd.setMetaObject( metaObj );
    return metaObj;
}

void* buddyListWnd::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "buddyListWnd" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool buddyListWnd::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: msgUserNow(); break;
    case 1: xferFileNow(); break;
    case 2: menuSigninAction_activated(); break;
    case 3: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool buddyListWnd::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool buddyListWnd::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool buddyListWnd::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
