// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL ();
    virtual void paintGL ();
    virtual void resizeGL (int, int);
    virtual void keyPressEvent (QKeyEvent*);
    void mouseMoveEvent (QMouseEvent*);
    virtual void carregaShaders ();

    virtual void creaBuffers ();

    void viewTransform ();
    void projectTransform (float, float, float, float);
    virtual void modelTransform ();

    float mousePos[2];

    glm::vec3 VRP, anglesCamera;

    float FOV, ra, zN, zF;

    float radi;

    float k;

    GLuint vertexLoc, colorLoc, transLoc, viewLoc, projLoc;

    Model legoMan;
    
    GLuint vaoLego;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
