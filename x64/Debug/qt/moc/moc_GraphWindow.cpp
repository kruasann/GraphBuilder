/****************************************************************************
** Meta object code from reading C++ file 'GraphWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../GraphWindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GraphWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSGraphWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSGraphWindowENDCLASS = QtMocHelpers::stringData(
    "GraphWindow",
    "onAddFunctionClicked",
    "",
    "onRemoveSelectedFunction",
    "onFunctionItemChanged",
    "QListWidgetItem*",
    "item",
    "onAxisRangeChanged",
    "saveGraph",
    "editFunction",
    "index",
    "changeFunctionColor",
    "onFunctionListContextMenu",
    "pos",
    "updateMouseCoordinates",
    "x",
    "y"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSGraphWindowENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[12];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[25];
    char stringdata4[22];
    char stringdata5[17];
    char stringdata6[5];
    char stringdata7[19];
    char stringdata8[10];
    char stringdata9[13];
    char stringdata10[6];
    char stringdata11[20];
    char stringdata12[26];
    char stringdata13[4];
    char stringdata14[23];
    char stringdata15[2];
    char stringdata16[2];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSGraphWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSGraphWindowENDCLASS_t qt_meta_stringdata_CLASSGraphWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "GraphWindow"
        QT_MOC_LITERAL(12, 20),  // "onAddFunctionClicked"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 24),  // "onRemoveSelectedFunction"
        QT_MOC_LITERAL(59, 21),  // "onFunctionItemChanged"
        QT_MOC_LITERAL(81, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(98, 4),  // "item"
        QT_MOC_LITERAL(103, 18),  // "onAxisRangeChanged"
        QT_MOC_LITERAL(122, 9),  // "saveGraph"
        QT_MOC_LITERAL(132, 12),  // "editFunction"
        QT_MOC_LITERAL(145, 5),  // "index"
        QT_MOC_LITERAL(151, 19),  // "changeFunctionColor"
        QT_MOC_LITERAL(171, 25),  // "onFunctionListContextMenu"
        QT_MOC_LITERAL(197, 3),  // "pos"
        QT_MOC_LITERAL(201, 22),  // "updateMouseCoordinates"
        QT_MOC_LITERAL(224, 1),  // "x"
        QT_MOC_LITERAL(226, 1)   // "y"
    },
    "GraphWindow",
    "onAddFunctionClicked",
    "",
    "onRemoveSelectedFunction",
    "onFunctionItemChanged",
    "QListWidgetItem*",
    "item",
    "onAxisRangeChanged",
    "saveGraph",
    "editFunction",
    "index",
    "changeFunctionColor",
    "onFunctionListContextMenu",
    "pos",
    "updateMouseCoordinates",
    "x",
    "y"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSGraphWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x08,    1 /* Private */,
       3,    0,   69,    2, 0x08,    2 /* Private */,
       4,    1,   70,    2, 0x08,    3 /* Private */,
       7,    0,   73,    2, 0x08,    5 /* Private */,
       8,    0,   74,    2, 0x08,    6 /* Private */,
       9,    1,   75,    2, 0x08,    7 /* Private */,
      11,    1,   78,    2, 0x08,    9 /* Private */,
      12,    1,   81,    2, 0x08,   11 /* Private */,
      14,    2,   84,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::QPoint,   13,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   15,   16,

       0        // eod
};

Q_CONSTINIT const QMetaObject GraphWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSGraphWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSGraphWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSGraphWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GraphWindow, std::true_type>,
        // method 'onAddFunctionClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRemoveSelectedFunction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFunctionItemChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'onAxisRangeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'editFunction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'changeFunctionColor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onFunctionListContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'updateMouseCoordinates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

void GraphWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GraphWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAddFunctionClicked(); break;
        case 1: _t->onRemoveSelectedFunction(); break;
        case 2: _t->onFunctionItemChanged((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 3: _t->onAxisRangeChanged(); break;
        case 4: _t->saveGraph(); break;
        case 5: _t->editFunction((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->changeFunctionColor((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onFunctionListContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 8: _t->updateMouseCoordinates((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *GraphWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSGraphWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int GraphWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
