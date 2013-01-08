#ifndef NUMBERSGRID_H
#define NUMBERSGRID_H

#include <QCheckBox>
#include <QGridLayout>
#include <QSignalMapper>
#include <QWidget>

//#define _DEBUG_FLAG_ENABLED

typedef QList<quint8 > ValuesList;

class NumbersGrid : public QWidget
{
    Q_OBJECT

private:
    QGridLayout *_gridLayout;
    QSignalMapper *_signalMapper;
    QVector<QCheckBox *> _grid;

#ifdef _DEBUG_FLAG_ENABLED
private slots:
    void numberStateChanged(int value);
#endif //_DEBUG_FLAG_ENABLED

public:
    explicit NumbersGrid(QWidget *parent = 0);
    ~NumbersGrid();
    quint8 getNumOfItems();
    quint32 getSystemSize();
    ValuesList getValuesList();

    static QStringList values2stringlist(ValuesList values);

public slots:
    void setAllValueChecked();
    void setAllValueUnchecked();
    void setValueChecked(quint8 value);
    void setValueUnchecked(quint8 value);

signals:
    void clicked(int value);
};

#endif // NUMBERSGRID_H
