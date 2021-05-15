#ifndef MYQLCD_H
#define MYQLCD_H

#include <QLCDNumber>

class MyQLCD : public QLCDNumber
{
    Q_OBJECT
public:
    MyQLCD(QWidget *parent);

public slots:

    void valorZero();

    void escriuNumero(int);

signals:

};

#endif // MYQLCD_H
