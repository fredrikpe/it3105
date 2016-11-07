/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../project3/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[426];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 24), // "on_oneStepButton_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 22), // "on_solveButton_clicked"
QT_MOC_LITERAL(4, 60, 25), // "on_openFileButton_clicked"
QT_MOC_LITERAL(5, 86, 9), // "normalize"
QT_MOC_LITERAL(6, 96, 29), // "vector<pair<double,double> >&"
QT_MOC_LITERAL(7, 126, 7), // "cityMap"
QT_MOC_LITERAL(8, 134, 24), // "on_nEpochsButton_clicked"
QT_MOC_LITERAL(9, 159, 33), // "on_nEpochsSpinBox_editingFini..."
QT_MOC_LITERAL(10, 193, 23), // "on_staticButton_clicked"
QT_MOC_LITERAL(11, 217, 23), // "on_linearButton_clicked"
QT_MOC_LITERAL(12, 241, 28), // "on_exponentialButton_clicked"
QT_MOC_LITERAL(13, 270, 10), // "changeType"
QT_MOC_LITERAL(14, 281, 10), // "DecayType&"
QT_MOC_LITERAL(15, 292, 3), // "old"
QT_MOC_LITERAL(16, 296, 9), // "DecayType"
QT_MOC_LITERAL(17, 306, 2), // "nu"
QT_MOC_LITERAL(18, 309, 5), // "reset"
QT_MOC_LITERAL(19, 315, 39), // "on_numIterationsspinBox_editi..."
QT_MOC_LITERAL(20, 355, 32), // "on_staticInfluenceButton_clicked"
QT_MOC_LITERAL(21, 388, 37) // "on_exponentialInfluenceButton..."

    },
    "MainWindow\0on_oneStepButton_clicked\0"
    "\0on_solveButton_clicked\0"
    "on_openFileButton_clicked\0normalize\0"
    "vector<pair<double,double> >&\0cityMap\0"
    "on_nEpochsButton_clicked\0"
    "on_nEpochsSpinBox_editingFinished\0"
    "on_staticButton_clicked\0on_linearButton_clicked\0"
    "on_exponentialButton_clicked\0changeType\0"
    "DecayType&\0old\0DecayType\0nu\0reset\0"
    "on_numIterationsspinBox_editingFinished\0"
    "on_staticInfluenceButton_clicked\0"
    "on_exponentialInfluenceButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    1,   87,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    2,   95,    2, 0x08 /* Private */,
      18,    0,  100,    2, 0x08 /* Private */,
      19,    0,  101,    2, 0x08 /* Private */,
      20,    0,  102,    2, 0x08 /* Private */,
      21,    0,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 16,   15,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_oneStepButton_clicked(); break;
        case 1: _t->on_solveButton_clicked(); break;
        case 2: _t->on_openFileButton_clicked(); break;
        case 3: _t->normalize((*reinterpret_cast< vector<pair<double,double> >(*)>(_a[1]))); break;
        case 4: _t->on_nEpochsButton_clicked(); break;
        case 5: _t->on_nEpochsSpinBox_editingFinished(); break;
        case 6: _t->on_staticButton_clicked(); break;
        case 7: _t->on_linearButton_clicked(); break;
        case 8: _t->on_exponentialButton_clicked(); break;
        case 9: _t->changeType((*reinterpret_cast< DecayType(*)>(_a[1])),(*reinterpret_cast< DecayType(*)>(_a[2]))); break;
        case 10: _t->reset(); break;
        case 11: _t->on_numIterationsspinBox_editingFinished(); break;
        case 12: _t->on_staticInfluenceButton_clicked(); break;
        case 13: _t->on_exponentialInfluenceButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
