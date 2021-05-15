// MyGLWidget.h
#include "BL2GLWidget.h"

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

	// Afegim al exercici 2

    virtual void viewTransform ();

    virtual void initializeCamera ();

    virtual void initializeGL ();

    glm::vec3 vecOBS, vecVRP, vecUP;

    GLuint viewLoc;   

  private:
    int printOglError(const char file[], int line, const char func[]);
};
