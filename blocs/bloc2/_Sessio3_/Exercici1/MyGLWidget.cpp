// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

#define HOMER 0
#define PATRICIO 1
#define TERRA 2

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

void MyGLWidget::initializeGL ( ) {

    xClick = 0;
    yClick = 0;
    escalar = 5.0f;
    initializeOpenGLFunctions();  
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.7, 1.0, 1.0);
    carregaShaders();
    initializeModel(HOMER, &Models[HOMER]);
    initializeModel(PATRICIO, &Models[PATRICIO]);
    initializeModel(TERRA, &Models[TERRA]);
    initializeMainCamera(&Models[HOMER], &mainCam);

}

void MyGLWidget::paintGL ( )  {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray (vaoHomer);
    modelAllTransform ( HOMER, &Models[HOMER] );
    glDrawArrays(GL_TRIANGLES, 0, Models[HOMER].model.faces().size() * 3);

    glBindVertexArray (vaoPatricio);
    modelAllTransform ( PATRICIO, &Models[PATRICIO] );
    glDrawArrays(GL_TRIANGLES, 0, Models[PATRICIO].model.faces().size() * 3);

    glBindVertexArray (vaoTerra);
    modelAllTransform ( TERRA, &Models[TERRA] );
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);

}

void MyGLWidget::resizeGL (int width, int height) {
    Camera myCam = mainCam;
    glViewport( 0, 0, width, height );
    myCam.ra = (float) width / (float) height;
    myCam.FOV = (float) (2.0f * atan(tan(mainCam.FOV / 2.0f) / mainCam.ra));
    projectTransform (&myCam);
}

void MyGLWidget::creaAllBuffers (int thing, myModel* _model) {
    if (thing == 0) {
        _model->model.load ( "./models/HomerProves.obj" );

        glGenVertexArrays(1, &vaoHomer);
        glBindVertexArray(vaoHomer);

        GLuint vboHomer[2];
        glGenBuffers(2, vboHomer);
        glBindBuffer(GL_ARRAY_BUFFER, vboHomer[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _model->model.faces().size() * 9, 
            _model->model.VBO_vertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vertexLoc);

        glBindBuffer(GL_ARRAY_BUFFER, vboHomer[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _model->model.faces().size() * 9, 
            _model->model.VBO_matdiff(), GL_STATIC_DRAW);

        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colorLoc);

        glBindVertexArray (0);
    }
    
    else if (thing == 1) {
        _model->model.load ( "./models/Patricio.obj" );

        glGenVertexArrays(1, &vaoPatricio);
        glBindVertexArray(vaoPatricio);

        GLuint vboPatricio[2];
        glGenBuffers(2, vboPatricio);
        glBindBuffer(GL_ARRAY_BUFFER, vboPatricio[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _model->model.faces().size() * 9, 
            _model->model.VBO_vertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vertexLoc);

        glBindBuffer(GL_ARRAY_BUFFER, vboPatricio[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _model->model.faces().size() * 9, 
            _model->model.VBO_matdiff(), GL_STATIC_DRAW);

        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colorLoc);

        glBindVertexArray (0);
    }

    else if (thing == 2) {
        glm::vec3 posicio [6] = {
            glm::vec3(-2, 0.0, -2),
            glm::vec3( 2, 0.0, -2),
            glm::vec3(-2, 0.0,  2),
            glm::vec3(-2, 0.0,  2),
            glm::vec3( 2, 0.0,  2),
            glm::vec3( 2, 0.0, -2),
        };

        glm::vec3 negre (0.8, 0.8, 0.8);
        glm::vec3 colTerra[6] = {
            negre, negre, negre, negre, negre, negre 
        };

        glGenVertexArrays(1, &vaoTerra);
        glBindVertexArray(vaoTerra);

        GLuint vboTerra[2];
        glGenBuffers(2, vboTerra);
        glBindBuffer(GL_ARRAY_BUFFER, vboTerra[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), 
            posicio, GL_STATIC_DRAW);

        glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vertexLoc);

        glBindBuffer(GL_ARRAY_BUFFER, vboTerra[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colTerra), 
            colTerra, GL_STATIC_DRAW);

        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colorLoc);

        glBindVertexArray (0);
    }
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

void MyGLWidget::initializeMainCamera (myModel* _model, Camera* _cam) {
    calculaCapsaContenidora(_model->puntMax, _model->puntMin, _cam);
    viewTransform(_cam);
    projectTransform(_cam);
}

void MyGLWidget::viewTransform (Camera* _cam) {
    /*
    glm::mat4 matrixLook = glm::lookAt(_cam->OBS, _cam->VRP, _cam->UP);
    */
    glm::mat4 matrixLook (1.0f);
    matrixLook = glm::translate(matrixLook, glm::vec3(0,0, -_cam->radi * escalar));
    matrixLook = glm::rotate(matrixLook, _cam->angleZ, glm::vec3(0,0,1));
    matrixLook = glm::rotate(matrixLook, _cam->angleX, glm::vec3(1,0,0));
    matrixLook = glm::rotate(matrixLook, _cam->angleY, glm::vec3(0,1,0));
    matrixLook = glm::translate(matrixLook, -_cam->VRP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &matrixLook[0][0]);
}

void MyGLWidget::projectTransform (Camera* _cam) {
    glm::mat4 matrixCam = glm::perspective(_cam->FOV, _cam->ra, _cam->zNear, _cam->zFar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &matrixCam[0][0]);
}

void MyGLWidget::calculaCapsaContenidora (glm::vec3 puntA, glm::vec3 puntB, Camera* _cam) {
    float x = puntA.x - puntB.x;
    float y = puntA.y - puntB.y;
    float z = puntA.z - puntB.z;

    _cam->radi = sqrt((x * x) + (y * y) + (z * z));

    _cam->VRP = glm::vec3((puntA.x + puntB.x)/2, (puntA.y + puntB.y)/2, (puntA.z + puntB.z)/2);
    _cam->OBS = _cam->VRP + glm::vec3(0, 0, _cam->radi * escalar);
    _cam->UP = glm::vec3(0, 1, 0);

    _cam->FOV = 2.0 * asin ( _cam->radi / (_cam->radi * escalar) );
    _cam->ra = 1.0;
    _cam->zNear = _cam->radi * (escalar - 2);
    _cam->zFar = _cam->radi * (escalar + 3);
}

void MyGLWidget::modelAllTransform (int thing, myModel* _model) {
    glm::mat4 transform (1.0f);
    if ( thing == HOMER ) {
        transform = glm::scale(transform, glm::vec3(_model->escalar));
        transform = glm::translate(transform, glm::vec3(-1, 0, 0));
    }
    else if ( thing == PATRICIO ) {
        transform = glm::scale(transform, glm::vec3(_model->escalar));
        transform = glm::translate(transform, glm::vec3(-(_model->puntMax.x + _model->puntMin.x)/2, -_model->puntMax.y, -(_model->puntMax.z + _model->puntMin.z)/2));
    }
    else if ( thing == TERRA ) {
        transform = glm::translate(transform, glm::vec3(0, -1, 0));
        transform = glm::scale(transform, glm::vec3(_model->escalar));
    }     
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *event) {
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_Z: { // escalar a més gran
            mainCam.FOV -= 0.05;
            break;
        }
        case Qt::Key_X: { // escalar a més petit
            mainCam.FOV += 0.05;
            break;
        }

        default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::calculPunts ( myModel* _model ) {
    if ( _model != nullptr ) {
        glm::vec3 pMin (_model->model.vertices()[0], _model->model.vertices()[1], _model->model.vertices()[2]);
        glm::vec3 pMax (_model->model.vertices()[0], _model->model.vertices()[1], _model->model.vertices()[2]);
        for ( unsigned int i = 3; i < _model->model.vertices().size(); i += 3 ) {
            if ( pMin.x > _model->model.vertices()[i] )       pMin.x = _model->model.vertices()[i];
            if ( pMin.y > _model->model.vertices()[i + 1] )   pMin.y = _model->model.vertices()[i + 1];
            if ( pMin.z > _model->model.vertices()[i + 2] )   pMin.z = _model->model.vertices()[i + 2];
            if ( pMax.x < _model->model.vertices()[i] )       pMax.x = _model->model.vertices()[i];
            if ( pMax.y < _model->model.vertices()[i + 1] )   pMax.y = _model->model.vertices()[i + 1];
            if ( pMax.z < _model->model.vertices()[i + 2] )   pMax.z = _model->model.vertices()[i + 2];
        }
        _model->puntMin = pMin;
        _model->puntMax = pMax;

    }
}

void MyGLWidget::initializeModel(int obj, myModel* _model) {
    if (obj == TERRA) {
        creaAllBuffers (obj, nullptr);
    } 
    else {
        creaAllBuffers (obj, _model);
        calculPunts (_model);
    }
    if (obj == PATRICIO) _model->escalar = 1.0/(_model->puntMax.y - _model->puntMin.y);
    else if (obj == TERRA) _model->escalar = 2.0;
    else _model->escalar = 1.0;
}
    
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    int dx = abs(e->x() - xClick);
    int dy = abs(e->y() - yClick);

    if (dx > dy) {
        if (e->x() > xClick) mainCam.angleY  = mainCam.angleY - abs(e->x() - xClick) * ((float) M_PI/180);
        else if (e->x() < xClick) mainCam.angleY  = mainCam.angleY + abs(e->x() - xClick) * ((float) M_PI/180);
    } 
    else {
        if (e->y() > yClick) mainCam.angleX = mainCam.angleX + abs(e->y() - yClick) * ((float) M_PI/180);
        else if (e->y() < yClick) mainCam.angleX = mainCam.angleX - abs(e->y() - yClick) * ((float) M_PI/180);
    }
    viewTransform(&mainCam);
    update();

    xClick = e->x();
    yClick = e->y();
}

