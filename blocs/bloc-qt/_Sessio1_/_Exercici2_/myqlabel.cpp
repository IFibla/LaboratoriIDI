#include "myqlabel.h"

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent)
{

}

void MyQLabel::Truncar(int i) {
    QString aux = origialWord;
    aux.truncate(i);
    emit enviaText(aux);
}

void MyQLabel::SetOriginal(QString aux) {
    origialWord = aux;
    Truncar(aux.size());
}
