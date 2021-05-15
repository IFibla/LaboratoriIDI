// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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

void MyGLWidget::initializeGL () {

    initializeOpenGLFunctions();  

    glEnable (GL_DEPTH_TEST);

    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    carregaShaders();
    creaBuffers();
}

void MyGLWidget::paintGL () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    modelTransform ();

    glBindVertexArray (vaoLego);

    glDrawArrays(GL_TRIANGLES, 0, legoMan.vertices().size() * 3);

    glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int width, int height) {
    glViewport(0, 0, width, height);
    float _FOV = 2.0 * atan ( tan ( FOV / 2.0 ) / ra );
    float _ra = (float) width / (float) height;
    projectTransform(_FOV, _ra, zN, zF);
}   

void MyGLWidget::keyPressEvent (QKeyEvent* event) {
    makeCurrent();
    switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran

        break;
    }
    case Qt::Key_D: { // escalar a més petit

        break;
    }
    default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::mouseMoveEvent (QMouseEvent* event) {
    makeCurrent();
    float currPos[2];
    currPos[0] = event->x();
    currPos[1] = event->y();

    float absValue[2];
    absValue[0] = abs(currPos[0] - mousePos[0]);
    absValue[1] = abs(currPos[1] - mousePos[1]);

    if (absValue[0] > absValue[1]) {
        if (currPos[0] - mousePos[0] > 0) {
            anglesCamera.y += absValue[0] * ((float) M_PI / (float) 180.0f);
        }
        else {
            anglesCamera.y -= absValue[0] * ((float) M_PI / (float) 180.0f);
        }
    }
    else {
        if (currPos[1] - mousePos[1] > 0) { 
            anglesCamera.x += absValue[1] * ((float) M_PI / (float) 180.0f);
        }
        else {
            anglesCamera.x -= absValue[1] * ((float) M_PI / (float) 180.0f);
        }
    }

    viewTransform ();
    update();
    mousePos[0] = currPos[0];
    mousePos[1] = currPos[1];
}

void MyGLWidget::carregaShaders () {
    QOpenGLShader fs (QOpenGLShader::Fragment, this);
    QOpenGLShader vs (QOpenGLShader::Vertex, this);

    fs.compileSourceFile("shaders/basicShader.frag");
    vs.compileSourceFile("shaders/basicShader.vert");

    program = new QOpenGLShaderProgram(this);

    program->addShader(&fs);
    program->addShader(&vs);

    program->link();

    program->bind();

    vertexLoc = glGetAttribLocation (program->programId(), "vertex");
    colorLoc = glGetAttribLocation (program->programId(), "color");
    transLoc = glGetUniformLocation(program->programId(), "TG");
    viewLoc = glGetUniformLocation(program->programId(), "view");
    projLoc = glGetUniformLocation(program->programId(), "proj");
}

void MyGLWidget::viewTransform () {
    glm::vec4 viewMatrix (1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, radi * k));
    viewMatrix = glm::rotate(viewMatrix, angles.z, glm::vec3(0, 0, 1));
    viewMatrix = glm::rotate(viewMatrix, angles.x, glm::vec3(1, 0, 0));
    viewMatrix = glm::rotate(viewMatrix, angles.y, glm::vec3(0, 1, 0));
    viewMatrix = glm::translate(viewMatrix, -VRP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
}

void MyGLWidget::projectTransform (float _FOV, float _ra, float _zN, float _zF) {
    glm::vec4 projMatrix = glm::perspective(_FOV, _ra, _zN, _zF);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projMatrix[0][0]);
}

void MyGLWidget::modelTransform () {
    glm::vec4 modelMatrix (1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &modelMatrix[0][0]);
}

void MyGLWidget::creaBuffers () {
    legoMan.load( "./models/legoman.obj" );
    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &vaoLego);
    glBindVertexArray(vaoLego);

    GLuint vboLego[2];
    glGenBuffers(2, vboLego);
    glBindBuffer(GL_ARRAY_BUFFER, vboLego[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * legoMan.faces().size() * 9, 
        legoMan.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, vboLego[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * legoMan.faces().size() * 9, 
        legoMan.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}