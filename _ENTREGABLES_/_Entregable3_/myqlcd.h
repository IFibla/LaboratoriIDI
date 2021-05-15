#ifndef MYQLCD_H
#define MYQLCD_H

#include <QLCDNumber>

class MyQLCD : public QLCDNumber
{
    Q_OBJECT
    int value;

public:
    MyQLCD(QWidget *parent);

public slots:
    void increment();
    void reset();

signals:
    void sendIncrU();
    void sendIncrD();
    void sendResetU();
    void sendReset();

};

#endif // MYQLCD_H
