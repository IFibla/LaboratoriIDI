// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;
#define colGris glm::vec3(0.5, 0.5, 0.5)
#define TERRA 0
#define HOMER 1
#define PATRICIO 2

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

void MyGLWidget::creaBuffers() {

    #pragma region Terra
    glm::vec3 posicio[6] = {
        glm::vec3(-1, -1, -1),
        glm::vec3( 1, -1, -1),
        glm::vec3(-1, -1,  1),
        glm::vec3( 1, -1, -1),
        glm::vec3(-1, -1,  1),
        glm::vec3( 1, -1,  1)
    }; 
    glm::vec3 color[6] = {
        colGris, colGris, colGris, colGris, colGris, colGris
    };

    glGenVertexArrays(1, &VAO_Casa);
    glBindVertexArray(VAO_Casa);

    GLuint VBO_Casa[2];
    glGenBuffers(2, VBO_Casa);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Casa[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Casa[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
    #pragma endregion

    #pragma region Homer
    homerProves.load ( "./models/HomerProves.obj" );

    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    GLuint VBO_Homer[2];
    glGenBuffers(2, VBO_Homer);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * homerProves.faces().size() * 9, homerProves.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * homerProves.faces().size() * 9, homerProves.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
    #pragma endregion

    #pragma region Patricio
    patricio.load ( "./models/Patricio.obj" );

    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    GLuint VBO_Patricio[2];
    glGenBuffers(2, VBO_Patricio);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * patricio.faces().size() * 9, patricio.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * patricio.faces().size() * 9, patricio.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
    #pragma endregion

}

void MyGLWidget::paintGL ( ) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    modelTransform (TERRA);
    glBindVertexArray (VAO_Casa);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    modelTransform (HOMER);
    glBindVertexArray (VAO_Homer);
    glDrawArrays(GL_TRIANGLES, 0, homerProves.vertices().size() * 3);

    modelTransform (PATRICIO);
    glBindVertexArray (VAO_Patricio);
    glDrawArrays(GL_TRIANGLES, 0, homerProves.vertices().size() * 3);

    glBindVertexArray (0);
}

void MyGLWidget::initializeGL ( ) {
    initializeOpenGLFunctions();  
    glEnable ( GL_DEPTH_TEST );
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    carregaShaders();
    creaBuffers();
    zoom = 2.0f;
    escala = 1.0f;
    angles.x = (float) M_PI/4;
    initializeCamera();
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

void MyGLWidget::modelTransform ( int VAO ) {
    glm::mat4 transform (1.0f);
    
    if ( VAO == TERRA ) {
        transform = glm::scale(transform, glm::vec3(2.0, 1, 2.0));
    }
    else if ( VAO == HOMER ) {
        transform = glm::scale(transform, glm::vec3(escala));
        transform = glm::translate(transform, glm::vec3(-1, 0, 0));
    }
    else if ( VAO == PATRICIO ) { 
        float altura = abs(puntMaxPatricio.y - puntMinPatricio.y);       
        transform = glm::scale(transform, glm::vec3( 2.0 / altura ));
        transform = glm::translate(transform, glm::vec3(-puntMinPatricio.x, -puntMinPatricio.y - 3, -puntMinPatricio.z));
    }  
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::viewTransform () {
    glm::mat4 viewMatrix (1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -radi * (zoom + 1)));
    viewMatrix = glm::rotate(viewMatrix, angles.z, glm::vec3( 0, 0, 1));
    viewMatrix = glm::rotate(viewMatrix, angles.x, glm::vec3( 1, 0, 0));
    viewMatrix = glm::rotate(viewMatrix, angles.y, glm::vec3( 0, 1, 0));
    viewMatrix = glm::translate(viewMatrix, -VRP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
}

void MyGLWidget::projectTransform ( float _FOV, float _ra) {
    glm::mat4 projMatrix (1.0f);
    projMatrix = glm::perspective(_FOV, _ra, zNear, zFar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projMatrix[0][0]);
}

void MyGLWidget::initializeCamera () {
    calculaPunts();
    
    float x = puntMaxHomer.x - puntMinHomer.x;
    float y = puntMaxHomer.y - puntMinHomer.y;
    float z = puntMaxHomer.z - puntMinHomer.z;

    radi = sqrt( (x * x) + (y * y) + (z * z) ) / 2;

    FOV = 2 * asin( radi / (radi * zoom) );
    ra  = 1.0;
    zNear = radi;
    zFar = radi * (escala + 4);

    projectTransform(FOV, ra);
    viewTransform();
}

void MyGLWidget::calculaPunts () {

    #pragma region Homer
    
    puntMinHomer = glm::vec3(homerProves.vertices()[0],
                                homerProves.vertices()[1],
                                homerProves.vertices()[2]);

    puntMaxHomer = glm::vec3(homerProves.vertices()[0],
                                homerProves.vertices()[1],
                                homerProves.vertices()[2]);

    for ( unsigned int i = 0; i < homerProves.vertices().size(); i += 3 ) {
        if ( puntMinHomer.x > homerProves.vertices()[i] ) puntMinHomer.x = homerProves.vertices()[i];
        if ( puntMinHomer.y > homerProves.vertices()[i + 1] ) puntMinHomer.y = homerProves.vertices()[i + 1];
        if ( puntMinHomer.z > homerProves.vertices()[i + 2] ) puntMinHomer.z = homerProves.vertices()[i + 2];
        if ( puntMaxHomer.x < homerProves.vertices()[i] ) puntMaxHomer.x = homerProves.vertices()[i];
        if ( puntMaxHomer.y < homerProves.vertices()[i + 1] ) puntMaxHomer.y = homerProves.vertices()[i + 1];
        if ( puntMaxHomer.z < homerProves.vertices()[i + 2] ) puntMaxHomer.z = homerProves.vertices()[i + 2];
    }

    #pragma endregion

    #pragma region Patricio

    puntMinPatricio = glm::vec3(patricio.vertices()[0],
                                patricio.vertices()[1],
                                patricio.vertices()[2]);

    puntMaxPatricio = glm::vec3(patricio.vertices()[0],
                                patricio.vertices()[1],
                                patricio.vertices()[2]);

    for ( unsigned int i = 0; i < patricio.vertices().size(); i += 3 ) {
        if ( puntMinPatricio.x > patricio.vertices()[i] ) puntMinPatricio.x = patricio.vertices()[i];
        if ( puntMinPatricio.y > patricio.vertices()[i + 1] ) puntMinPatricio.y = patricio.vertices()[i + 1];
        if ( puntMinPatricio.z > patricio.vertices()[i + 2] ) puntMinPatricio.z = patricio.vertices()[i + 2];
        if ( puntMaxPatricio.x < patricio.vertices()[i] ) puntMaxPatricio.x = patricio.vertices()[i];
        if ( puntMaxPatricio.y < patricio.vertices()[i + 1] ) puntMaxPatricio.y = patricio.vertices()[i + 1];
        if ( puntMaxPatricio.z < patricio.vertices()[i + 2] ) puntMaxPatricio.z = patricio.vertices()[i + 2];
    }

    #pragma endregion

}

void MyGLWidget::resizeGL (int w, int h) {
    glViewport(0, 0, w, h);
    float _ra = (float) w / (float) h;
    float _FOV = 2.0 * atan ( tan ( FOV / 2.0 ) / ra );
    projectTransform (_FOV, _ra);
}

void MyGLWidget::keyPressEvent (QKeyEvent *event) {
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_S: { // escalar a més gran
            zoom += 0.05;
            break;
        }
        case Qt::Key_D: { // escalar a més petit
            zoom -= 0.05;
            break;
        }
        default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::mouseMoveEvent (QMouseEvent *event) {
    makeCurrent();
    float diffX = abs(event->x() - x);
    float diffY = abs(event->y() - y);

    if (diffX > diffY) {
        if (event->x() > x) {
            angles.y += diffX * ( (float) M_PI / 180.0 );
        }
        else {
            angles.y -= diffX * ( (float) M_PI / 180.0 );
        }
    }
    else {
        if (event->y() > y) {
            angles.x += diffY * ( (float) M_PI / 180.0 );
        }
        else {
            angles.x -= diffY * ( (float) M_PI / 180.0 );
        }
    }
    viewTransform();
    x = event->x();
    y = event->y();
    update();
}

void MyGLWidget::sendValue(int value) {
    makeCurrent();
    FOV = (float) value / 10.0;
    projectTransform(FOV, ra);
    update();
}
