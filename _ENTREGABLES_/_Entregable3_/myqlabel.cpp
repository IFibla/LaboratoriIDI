#include "myqlabel.h"

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent)
{
    value = 0;
    setText("0");
}

void MyQLabel::increment() {
    if (value) {
        value = 0;
        setText("0");
        setStyleSheet("QLabel { background-color : green; color : black; }");
        emit nextIncr();
    }
    else {
        ++value;
        setText("1");
        setStyleSheet("QLabel { background-color : yellow; color : black; }");
    }
}

void MyQLabel::reset() {
    value = 0;
    setText("0");
    setStyleSheet("QLabel { background-color : green; color : black; }");
    emit sendReset();
}
