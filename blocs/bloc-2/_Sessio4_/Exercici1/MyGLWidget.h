// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:

    virtual void initializeGL ( );

    virtual void creaBuffers();

    virtual void paintGL ( );

    virtual void carregaShaders ();

    virtual void modelTransform ( int );

    virtual void viewTransform ();

    virtual void projectTransform ( float, float );

    virtual void initializeCamera ();

    virtual void calculaPunts ();

    virtual void resizeGL ( int, int );

    virtual void keyPressEvent (QKeyEvent *event);

    virtual void mouseMoveEvent (QMouseEvent *event);

    GLuint VAO_Casa, VAO_Homer, VAO_Patricio;

    GLuint vertexLoc, colorLoc, transLoc, viewLoc, projLoc;

    glm::vec3 angles, VRP;

    float FOV, ra, zNear, zFar, radi;

    Model homerProves, patricio;

    glm::vec3 puntMinHomer, puntMinPatricio, puntMaxHomer, puntMaxPatricio;

    float escala, zoom;

    GLuint width, height;

    float x, y;

  public slots:
    void sendValue (int);
  signals:

  private:
    int printOglError(const char file[], int line, const char func[]);
};
