#include "myqlcd.h"

MyQLCD::MyQLCD(QWidget *parent) : QLCDNumber(parent)
{
    setDigitCount(2);
    setStyleSheet("color : red;");
}

void MyQLCD::valorZero() {
    escriuNumero(0);
}

void MyQLCD::escriuNumero (int value) {
    if (value == 0) setStyleSheet("color : red;");
    else setStyleSheet("color : black;");
    display(value);
}
