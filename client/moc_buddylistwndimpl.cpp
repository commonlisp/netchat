/****************************************************************************
** BuddyListWndImpl meta object code from reading C++ file 'buddylistwndimpl.h'
**
** Created: Tue May 24 21:35:54 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "buddylistwndimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BuddyListWndImpl::className() const
{
    return "BuddyListWndImpl";
}

QMetaObject *BuddyListWndImpl::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BuddyListWndImpl( "BuddyListWndImpl", &BuddyListWndImpl::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BuddyListWndImpl::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BuddyListWndImpl", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BuddyListWndImpl::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BuddyListWndImpl", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BuddyListWndImpl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = buddyListWnd::staticMetaObject();
    static const QUMethod slot_0 = {"xferFileNow", 0, 0 };
    static const QUMethod slot_1 = {"msgUserNow", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "xferFileNow()", &slot_0, QMetaData::Public },
	{ "msgUserNow()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"BuddyListWndImpl", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BuddyListWndImpl.setMetaObject( metaObj );
    return metaObj;
}

void* BuddyListWndImpl::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BuddyListWndImpl" ) )
	return this;
    return buddyListWnd::qt_cast( clname );
}

bool BuddyListWndImpl::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: xferFileNow(); break;
    case 1: msgUserNow(); break;
    default:
	return buddyListWnd::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BuddyListWndImpl::qt_emit( int _id, QUObject* _o )
{
    return buddyListWnd::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BuddyListWndImpl::qt_property( int id, int f, QVariant* v)
{
    return buddyListWnd::qt_property( id, f, v);
}

bool BuddyListWndImpl::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
