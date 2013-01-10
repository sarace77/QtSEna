#include "numbersgrid.h"

#ifdef _DEBUG_FLAG_ENABLED
#include <QDebug>
#endif //_DEBUG_FLAG_ENABLED

NumbersGrid::NumbersGrid(QWidget *parent) :
    QWidget(parent)
{
    _gridLayout = new QGridLayout(this);
    _signalMapper = new QSignalMapper(this);
    setStyleSheet("QWidget {background-color:#fff;}");
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++) {
        QCheckBox *cbItem = new QCheckBox();
        cbItem->setText(QString("%1").arg(i+1));
        connect(cbItem, SIGNAL(stateChanged(int)), _signalMapper, SLOT(map()));
        _signalMapper->setMapping(cbItem, cbItem->text().toInt());
        _grid.append(cbItem);
        _gridLayout->addWidget(_grid.at(i), i/10, i - (i/10)*10, 1, 1);
    }
    setAllValueChecked();
    connect(_signalMapper, SIGNAL(mapped(int)), this, SIGNAL(clicked(int)));
#ifdef _DEBUG_FLAG_ENABLED
    connect(this, SIGNAL(clicked(int)), this, SLOT(numberStateChanged(int)));
#endif //_DEBUG_FLAG_ENABLED
}

NumbersGrid::~NumbersGrid() {
#ifdef _DEBUG_FLAG_ENABLED
    qDebug() << "[NUMBERSGRID] - ~NumbersGrid() - Valori:" << values2stringlist(getValuesList());
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
    for (quint8 i = 0; i < _NUM_OF_ITEMS; i++) {
        if (_grid.at(i)->isChecked())
            result << (i + 1);
    }
    return result;
}

#ifdef _DEBUG_FLAG_ENABLED
void NumbersGrid::numberStateChanged(int value) {
    qDebug() << "[NUMBERSGRID] - numberStateChanged() - Colonne Sistema: " << getSystemSize() << value << ": " << _grid.at(value - 1)->isChecked();
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
    for (int i = 0; i < values.size(); i++) {
        result << QString("%1").arg(values.at(i));
    }
    return result;
}
