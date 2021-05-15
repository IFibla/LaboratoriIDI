#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QObject>
#include <QLabel>

class MyQLabel : public QLabel
{
    Q_OBJECT

    QString origialWord;
public:
    MyQLabel(QWidget* parent = 0);

public slots:
    void Truncar(int);
    void SetOriginal(QString);

signals:
    void enviaText(const QString&);
};

#endif // MYQLABEL_H
