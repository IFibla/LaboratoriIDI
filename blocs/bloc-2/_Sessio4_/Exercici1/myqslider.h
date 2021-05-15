#ifndef MYQSLIDER_H
#define MYQSLIDER_H

#include <QSlider>
#include "MyGLWidget.h"

class MyQSlider : public QSlider, MyGLWidget
{
    Q_OBJECT
public:

public slots:
    void canviaFOV(int);

signals:

};

#endif // MYQSLIDER_H
