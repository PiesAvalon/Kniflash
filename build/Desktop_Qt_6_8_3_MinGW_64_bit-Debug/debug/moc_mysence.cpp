/****************************************************************************
** Meta object code from reading C++ file 'mysence.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mysence.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mysence.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7MySenceE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN7MySenceE = QtMocHelpers::stringData(
    "MySence",
    "propPicked",
    "",
    "Prop*",
    "p",
    "player_win_signal",
    "checkDistance",
    "checkCharacterDistance",
    "getAimedChar",
    "resetAimLine"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN7MySenceE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    0,   53,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   54,    2, 0x08,    4 /* Private */,
       7,    0,   55,    2, 0x08,    5 /* Private */,
       8,    0,   56,    2, 0x08,    6 /* Private */,
       9,    0,   57,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MySence::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_meta_stringdata_ZN7MySenceE.offsetsAndSizes,
    qt_meta_data_ZN7MySenceE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN7MySenceE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MySence, std::true_type>,
        // method 'propPicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Prop *, std::false_type>,
        // method 'player_win_signal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkDistance'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkCharacterDistance'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getAimedChar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetAimLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MySence::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MySence *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->propPicked((*reinterpret_cast< std::add_pointer_t<Prop*>>(_a[1]))); break;
        case 1: _t->player_win_signal(); break;
        case 2: _t->checkDistance(); break;
        case 3: _t->checkCharacterDistance(); break;
        case 4: _t->getAimedChar(); break;
        case 5: _t->resetAimLine(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Prop* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (MySence::*)(Prop * );
            if (_q_method_type _q_method = &MySence::propPicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (MySence::*)();
            if (_q_method_type _q_method = &MySence::player_win_signal; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *MySence::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MySence::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN7MySenceE.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int MySence::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MySence::propPicked(Prop * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MySence::player_win_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
