/****************************************************************************
** Meta object code from reading C++ file 'missionrecap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "missionrecap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'missionrecap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MissionRecap_t {
    QByteArrayData data[5];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MissionRecap_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MissionRecap_t qt_meta_stringdata_MissionRecap = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 20),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 23),
QT_MOC_LITERAL(4, 59, 24)
    },
    "MissionRecap\0replayMissionClicked\0\0"
    "on_pushButton_5_clicked\0"
    "on_VideoWidget_destroyed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MissionRecap[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MissionRecap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MissionRecap *_t = static_cast<MissionRecap *>(_o);
        switch (_id) {
        case 0: _t->replayMissionClicked(); break;
        case 1: _t->on_pushButton_5_clicked(); break;
        case 2: _t->on_VideoWidget_destroyed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MissionRecap::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MissionRecap.data,
      qt_meta_data_MissionRecap,  qt_static_metacall, 0, 0}
};


const QMetaObject *MissionRecap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MissionRecap::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MissionRecap.stringdata))
        return static_cast<void*>(const_cast< MissionRecap*>(this));
    return QWidget::qt_metacast(_clname);
}

int MissionRecap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE