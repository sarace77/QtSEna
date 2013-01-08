#include "numbersgrid.h"

#define _NUM_OF_ITEMS 90
#define _COL_SIZE 6

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

NumbersGrid::NumbersGrid(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QWidget {background-color:#fff;}");
    _gridLayout = new QGridLayout(this);
    _signalMapper = new QSignalMapper(this);
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++) {
        QCheckBox *cbItem = new QCheckBox();
        cbItem->setText(QString("%1").arg(i+1));
        connect(cbItem, SIGNAL(stateChanged(int)), _signalMapper, SLOT(map()));
        _signalMapper->setMapping(cbItem, cbItem->text().toInt());
        _grid.append(cbItem);
    }

    connect(_signalMapper, SIGNAL(mapped(int)), this, SIGNAL(clicked(int)));

    for (quint8 i = 0; i < 9; i++) {
        for (quint8 j = 0; j < 10; j++) {
            _gridLayout->addWidget(_grid.at(10*i+j), i, j, 1, 1);
        }
    }
    setLayout(_gridLayout);

#ifdef _DEBUG_FLAG_ENABLED
    connect(this, SIGNAL(clicked(int)), this, SLOT(numberStateChanged(int)));
#endif //_DEBUG_FLAG_ENABLED

    setAllValueChecked();
}

NumbersGrid::~NumbersGrid() {
#ifdef _DEBUG_FLAG_ENABLED
    qDebug() << values2stringlist(getValuesList());
#endif //_DEBUG_FLAG_ENABLED
    delete _gridLayout;
    delete _signalMapper;
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++) {
        delete _grid.at(i);
    }
}

quint8 NumbersGrid::getNumOfItems() {
    quint8 count = 0;
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++) {
        if (_grid.at(i)->isChecked())
            count++;
    }
    return count;
}

quint32 NumbersGrid::getSystemSize() {
    quint32 result = 1;
    for (quint8 i = 0; i < _COL_SIZE; i++) {
        result *= getNumOfItems() - i;
        result /= (i + 1);
    }
    return result;
}

ValuesList NumbersGrid::getValuesList() {
    ValuesList result;
    result.clear();
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++) {
        if (_grid.at(i)->isChecked())
            result << (i + 1);
    }
    return result;
}

#ifdef _DEBUG_FLAG_ENABLED
void NumbersGrid::numberStateChanged(int value) {
    qDebug() << "Columns: " << getSystemSize() << value << ": " << _grid.at(value - 1)->isChecked();
}
#endif //_DEBUG_FLAG_ENABLED


void NumbersGrid::setAllValueChecked() {
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++)
        setValueChecked(i);
}


void NumbersGrid::setAllValueUnchecked() {
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++)
        setValueUnchecked(i);
}


void NumbersGrid::setValueChecked(quint8 value) {
    _grid.at(value)->setChecked(true);
}

void NumbersGrid::setValueUnchecked(quint8 value) {
    _grid.at(value)->setChecked(false);
}

QStringList NumbersGrid::values2stringlist(ValuesList values) {
    QStringList result;
    result.clear();
    for (int i = 0; i < values.size(); i++) {
        result << QString("%1").arg(values.at(i));
    }
    return result;
}
