// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

void MyGLWidget::projectTransform () 
{
    glm::mat4 Proj;
    if (cameraPersepective) Proj = glm::perspective(FOV, ra, znear, zfar);
    //else Proj = glm::ortho(xmin, xmax, 0.0, 4.0, znear, zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::carregaShaders() {
    QOpenGLShader fs (QOpenGLShader::Fragment, this);
    QOpenGLShader vs (QOpenGLShader::Vertex, this);

    fs.compileSourceFile("shaders/basicShader.frag");
    vs.compileSourceFile("shaders/basicShader.vert");

    program = new QOpenGLShaderProgram(this);

    program->addShader(&fs);
    program->addShader(&vs);

    program->link();

    program->bind();

    transLoc = glGetUniformLocation(program->programId(), "TG");
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");

    colorLoc = glGetAttribLocation(program->programId(), "color");
    vertexLoc = glGetAttribLocation(program->programId(), "vertex");
}

void MyGLWidget::floorTransform() {
    glm::mat4 TG(1.0f);
    TG = glm::scale(TG, glm::vec3(5,1,5));
    TG = glm::translate(TG, glm::vec3(0, 0, 0));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL() {

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindVertexArray(VAO_Terra);

    floorTransform();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    glBindVertexArray(VAO_patricio);

    modelTransform();

    glDrawArrays(GL_TRIANGLES, 0, patricio.faces().size()*3);

    glBindVertexArray(0);

}

void MyGLWidget::viewTransform() {
    glm::mat4 View = glm::lookAt(vecOBS, vecVRP, vecUP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::initializeCamera() {

    xmin = xmax = patricio.vertices()[0];
    ymin = ymax = patricio.vertices()[0+1];
    zmin = zmax = patricio.vertices()[0+2];

    for(unsigned int i = 3; i < patricio.vertices().size(); i += 3){
        if (patricio.vertices()[i] > xmax) xmax = patricio.vertices()[i];
        if (patricio.vertices()[i] < xmin) xmin = patricio.vertices()[i];
        if (patricio.vertices()[i+1] > ymax) ymax = patricio.vertices()[i+1];
        if (patricio.vertices()[i+1] < ymin) ymin = patricio.vertices()[i+1];
        if (patricio.vertices()[i+2] > zmax) zmax = patricio.vertices()[i+2];
        if (patricio.vertices()[i+2] < zmin) zmin = patricio.vertices()[i+2];
    }

    printf("%f, %f, %f, %f, %f, %f \n", xmin, ymin, zmin, xmax, ymax, zmax);
    escala = 4.0/(ymax-ymin);
    modelTransform();
    CapsaContenidora(-(xmax+xmin)/2, 0, -(zmax+zmin)/2, (xmax+xmin)/2, 4.0, (zmax+zmin)/2);
    viewTransform();
    projectTransform();
}

void MyGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    cameraPersepective = true;
    glClearColor(0.5, 0.7, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);

    carregaShaders();
    creaBufferspatricio();
    creaBuffersTerra();
    escala = 0.5f;
    initializeCamera();
}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::creaBufferspatricio () {
    patricio.load("./models/HomerProves.obj");

    glGenVertexArrays(1, &VAO_patricio);
    glBindVertexArray(VAO_patricio);

    GLuint VBO_patricio[2];
    glGenBuffers(2, VBO_patricio);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_patricio[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*3*3,
		    patricio.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_patricio[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patricio.faces().size()*3*3,
		    patricio.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::modelTransform () {
    glm::mat4 transform(1.0f);
    transform = rotate(transform, angle, glm::vec3(0,1,0));
    transform = glm::scale(transform, glm::vec3(escala));
    transform = translate(transform, glm::vec3(-(xmax+xmin)/2, -ymin, -(zmin+xmax)/2));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      escala += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      escala -= 0.05;
      break;
    }
    case Qt::Key_R: {
        angle += M_PI/4;
        break;
    }
    case Qt::Key_O: {
        cameraPersepective = !cameraPersepective;
        initializeCamera();
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffersTerra() {
    glm::vec3 geomTerra [6] = {
        glm::vec3(-0.5, 0.0, -0.5),
        glm::vec3(0.5, 0.0, -0.5),
        glm::vec3(0.5, 0.0, 0.5),
        glm::vec3(-0.5, 0.0, -0.5),
        glm::vec3(0.5, 0.0, 0.5),
        glm::vec3(-0.5, 0.0, 0.5)
    };

    glm::vec3 negre (0.8, 0.8, 0.8);
    glm::vec3 colTerra[6] = {
        negre, negre, negre, negre, negre, negre 
    };

    glGenVertexArrays(1, &VAO_Terra);
    glBindVertexArray(VAO_Terra);

    GLuint VBO_Terra[2];
    glGenBuffers(2, VBO_Terra);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geomTerra), geomTerra, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colTerra), colTerra, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);
}

void MyGLWidget::CapsaContenidora(float _xmin, float _ymin, float _zmin, float _xmax, float _ymax, float _zmax) {

    printf("%f, %f, %f, %f, %f, %f \n", _xmin, _xmax, _ymin, _ymax, _zmin, _zmax);

    radiEsferaContenidora = sqrt((_xmax-_xmin)*(_xmax-_xmin)+(_ymax-_ymin)*(_ymax-_ymin)+(_zmax-_zmin)*(_zmax-_zmin))/2.0;
    float k = 5;

    vecVRP = glm::vec3((_xmax+_xmin)/2.0,(_ymax+_ymin)/2.0,(_zmax+_zmin)/2.0);
    vecOBS = vecVRP + glm::vec3(0, 0, radiEsferaContenidora*k);
    vecUP = glm::vec3(0,1,0);

    printf("VRP: %f, %f, %f\n", (_xmax+_xmin)/2.0,(_ymax+_ymin)/2.0,(_zmax+_zmin)/2.0);
    printf("OBS: %f, %f, %f\n", 0.0, 0.0, radiEsferaContenidora*k);

    FOV = 2.0*asin(radiEsferaContenidora/(radiEsferaContenidora*k));
    ra = 1.0f;
    znear = radiEsferaContenidora*k-radiEsferaContenidora;
    zfar = radiEsferaContenidora*k+radiEsferaContenidora;

}

void MyGLWidget::resizeGL ( int h, int w ) {
    glViewport(0, 0, w, h);
    ra = float(w) / float(h);
    FOV = 2.0 * atan(tan(FOV/2.0)/ra);
    projectTransform();
}
