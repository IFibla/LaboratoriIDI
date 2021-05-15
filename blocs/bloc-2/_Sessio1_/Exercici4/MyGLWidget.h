// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:

    virtual void projectTransform();
    
    virtual void carregaShaders();

    virtual void paintGL();

    GLuint projLoc;

    virtual void viewTransform ();

    virtual void initializeCamera ();

    virtual void initializeGL ();

    glm::vec3 vecOBS, vecVRP, vecUP;

    GLuint viewLoc;   


    Model homer;

    virtual void creaBuffers ();

    virtual void modelTransform ();

    GLuint VAO_Homer;

    GLuint colorLoc, vertexLoc, transLoc;

    GLfloat escala;  

    // Exercici 4

    float angle;

    virtual void keyPressEvent(QKeyEvent* event);

  private:
    int printOglError(const char file[], int line, const char func[]);
};
