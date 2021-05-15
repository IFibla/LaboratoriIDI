#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>

class MyQLabel : public QLabel
{
    Q_OBJECT

    int value;

public:
    MyQLabel(QWidget *parent);

public slots:
    void increment();
    void reset();

signals:
    void nextIncr();
    void sendReset();
};

#endif // MYQLABEL_H
