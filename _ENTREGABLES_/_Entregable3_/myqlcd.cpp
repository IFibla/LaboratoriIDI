#include "myqlcd.h"

MyQLCD::MyQLCD(QWidget *parent) : QLCDNumber(parent)
{
    value = 0;
    setOctMode();
    setDigitCount(2);
    display(value);
}

void MyQLCD::increment() {
    if (value != 63) {
        ++value;
        display(value);
        emit sendIncrU();
        if (value % 8 == 0 && value > 7) {
            emit sendIncrD();
            emit sendResetU();
        }
    }
}

void MyQLCD::reset() {
    value = 0;
    display(value);
    emit sendReset();
}

