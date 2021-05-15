// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

void MyGLWidget::projectTransform () 
{
    glm::mat4 Proj = glm::perspective(float(M_PI)/4.0f, 1.0f, 0.4f, 9.0f);
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
    TG = glm::translate(TG, glm::vec3(0, -0.5, 0));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL() {

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindVertexArray(VAO_Terra);

    floorTransform();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);



    glBindVertexArray(VAO_Homer);

    modelTransform();

    glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);

    glBindVertexArray(0);

}

void MyGLWidget::viewTransform() {
    glm::mat4 View = glm::lookAt(vecOBS, vecVRP, vecUP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::initializeCamera() {
    vecOBS = glm::vec3(0,0,1); 
    vecVRP = glm::vec3(0,0,0);
    vecUP  = glm::vec3(0,1,0);

    viewTransform();
    projectTransform();
}

void MyGLWidget::initializeGL() {
    initializeOpenGLFunctions();

    CapsaContenidora(glm::vec3(0,0,0), glm::vec3(0,10,0));
    
    glClearColor(0.5, 0.7, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);

    carregaShaders();
    creaBuffersHomer();
    creaBuffersTerra();
    escala = 1.0f;
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

void MyGLWidget::creaBuffersHomer () {
    homer.load("./models/HomerProves.obj");

    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    GLuint VBO_Homer[2];
    glGenBuffers(2, VBO_Homer);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3,
		    homer.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3,
		    homer.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::modelTransform () {
    glm::mat4 transform(1.0f);
    transform = rotate(transform, angle, glm::vec3(0,1,0));
    transform = glm::scale(transform, glm::vec3(escala));
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

void MyGLWidget::CapsaContenidora(glm::vec3 puntA, glm::vec3 puntB) {

    float distance = glm::distance(puntA, puntB);

    std::cout << distance << endl;

}