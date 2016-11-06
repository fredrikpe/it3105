/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../project3/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      39,   11,   11,   11, 0x08,
      64,   11,   11,   11, 0x08,
     100,   92,   11,   11, 0x08,
     141,   11,   11,   11, 0x08,
     168,   11,   11,   11, 0x08,
     204,   11,   11,   11, 0x08,
     230,   11,   11,   11, 0x08,
     256,   11,   11,   11, 0x08,
     294,  287,   11,   11, 0x08,
     327,   11,   11,   11, 0x08,
     335,   11,   11,   11, 0x08,
     377,   11,   11,   11, 0x08,
     412,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_oneStepButton_clicked()\0"
    "on_solveButton_clicked()\0"
    "on_openFileButton_clicked()\0cityMap\0"
    "normalize(vector<pair<double,double> >&)\0"
    "on_nEpochsButton_clicked()\0"
    "on_nEpochsSpinBox_editingFinished()\0"
    "on_staticButton_clicked()\0"
    "on_linearButton_clicked()\0"
    "on_exponentialButton_clicked()\0old,nu\0"
    "changeType(DecayType&,DecayType)\0"
    "reset()\0on_numIterationsspinBox_editingFinished()\0"
    "on_staticInfluenceButton_clicked()\0"
    "on_exponentialInfluenceButton_clicked()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
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

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
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
    }
    return _id;
}
QT_END_MOC_NAMESPACE
