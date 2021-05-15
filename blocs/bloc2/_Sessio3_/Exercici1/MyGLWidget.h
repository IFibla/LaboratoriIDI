// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

struct Camera {
  float FOV;
  float ra;
  float zNear;
  float zFar;
  glm::vec3 OBS;
  glm::vec3 VRP;
  glm::vec3 UP;
  float radi;
  float angleX;
  float angleY;
  float angleZ;
};

class myModel {
  public:
  glm::vec3 puntMin;
  glm::vec3 puntMax;
  Model model;
  float escalar;
  float rotacio;
};

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL ( );
    virtual void paintGL ( );
    virtual void resizeGL ( int width, int height );
    void creaAllBuffers ( int, myModel* );
    virtual void carregaShaders ();

    void initializeMainCamera (myModel*, Camera*);
    void viewTransform (Camera*);
    void projectTransform (Camera*);
    void modelAllTransform ( int, myModel* );

    void initializeModel (int, myModel*);
    void calculPunts ( myModel* );

    void calculaCapsaContenidora (glm::vec3, glm::vec3, Camera*);

    virtual void keyPressEvent (QKeyEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *);

    myModel Models [3];

    GLuint vertexLoc, colorLoc, transLoc, viewLoc, projLoc;

    GLuint vaoHomer;
    GLuint vaoPatricio;
    GLuint vaoTerra;

    Camera mainCam;

    float escalar;

    int xClick, yClick;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
