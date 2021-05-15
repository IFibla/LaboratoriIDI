#ifndef MYFORM_H
#define MYFORM_H

#include "ui_MyForm.h"

class MyForm : public QWidget {
    Q_OBJECT

public:
    MyForm(QWidget *parent = 0);

private:
    Ui::MyForm ui;
};

#endif // MYFORM_H
