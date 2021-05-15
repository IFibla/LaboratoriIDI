#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

    virtual void paintGL ();

    virtual void iniCamera0 ();

    virtual void iniCamera1 ();

    virtual void initializeGL ();

    virtual void keyPressEvent(QKeyEvent* event);

    virtual void iniEscena();

    virtual void homerTransform (int x, int z); 

    float angleH;

    unsigned int cameraValue;


  private:
    int printOglError(const char file[], int line, const char func[]);
};
