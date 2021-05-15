#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersEscac();
  creaBuffersHomer();
  iniEscena();
  iniCamera0();
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

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (xH == -3) angleH = M_PI/2;
    else if (xH == 4) angleH = 3 * M_PI/2;
    else if (zH == -3) angleH = 0;
    else if (zH == 4) angleH = M_PI;

  // Homer
  homerTransform(xH, zH);
  glBindVertexArray (VAO_Homer);
  glUniform1i(indexColorLoc, 2);  // color = 2 -- color per vèrtex
  glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);
  glBindVertexArray (0);

  // Escac
  for (int i = -3; i <= 4; ++i)
    for (int j = -3; j <= 4; ++j) {
        escacTransform(i, j);
        glBindVertexArray (VAO_Escac);
        glUniform1i(indexColorLoc, ((i + j) % 2) == 0);  // color = 1 -- color escac blanc (0.8, 0.8, 0.8)
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray (0);
    }
}

void MyGLWidget::iniCamera0()
{
    obs = glm::vec3(0.5, 2, 12);
    vrp = glm::vec3(0.5, 0, 0.5);
    up = glm::vec3(0, 1, 0);
    fov = float(M_PI)/3.3f;
    ra  = 1.0;
    znear =  6.5;
    zfar  = 20;
    cameraValue = 0;
    viewTransform();
    projectTransform();
}

void MyGLWidget::iniCamera1()
{
    obs = glm::vec3(0.5, 10, 0.5);
    vrp = glm::vec3(0.5, 0, 0.5);
    up = glm::vec3(0, 0, -1);
    fov = float(M_PI)/3.3f;
    ra  = 1.0;
    znear =  6.5;
    zfar  = 20;
    cameraValue = 1;
    viewTransform();
    projectTransform();
}

void MyGLWidget::iniEscena()
{
  angleH     = 0.0;
  xH         = zH  = 0;  // posició x i z final del Homer
  iniCamera0();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
      switch (event->key()) {
    default: event->ignore(); break;
    case Qt::Key_1: { 
        if ( xH + deltai[0] > -4 && xH + deltai[0] <= 4 && zH + deltaj[0] > -4 && zH + deltaj[0] <= 4 ) {
            zH += deltaj[0];
            xH += deltai[0]; 
            angleH = M_PI;
        }
        break;
    }
    case Qt::Key_2: { 
        if ( xH + deltai[1] > -4 && xH + deltai[1] <= 4 && zH + deltaj[1] > -4 && zH + deltaj[1] <= 4 ) {
            zH += deltaj[1];
            xH += deltai[1]; 
            angleH = 3 * M_PI/2;
        }
        break;
    }
    case Qt::Key_3: { 
        if ( xH + deltai[2] > -4 && xH + deltai[2] <= 4 && zH + deltaj[2] > -4 && zH + deltaj[2] <= 4 ) {
            zH += deltaj[2];
            xH += deltai[2]; 
            angleH = 3 * M_PI/2;
        }
        break;
    }
    case Qt::Key_4: { 
        if ( xH + deltai[3] > -4 && xH + deltai[3] <= 4 && zH + deltaj[3] > -4 && zH + deltaj[3] <= 4 ) {
            zH += deltaj[3];
            xH += deltai[3]; 
            angleH = 0;
        }
        break;
    }
    case Qt::Key_5: { 
        if ( xH + deltai[4] > -4 && xH + deltai[4] <= 4 && zH + deltaj[4] > -4 && zH + deltaj[4] <= 4 ) {
            zH += deltaj[4];
            xH += deltai[4]; 
            angleH = 0;
        }
        break;
    }
    case Qt::Key_6: { 
        if ( xH + deltai[5] > -4 && xH + deltai[5] <= 4 && zH + deltaj[5] > -4 && zH + deltaj[5] <= 4 ) {
            zH += deltaj[5];
            xH += deltai[5]; 
            angleH = M_PI/2;
        }
        break;
    }
    case Qt::Key_7: { 
        if ( xH + deltai[6] > -4 && xH + deltai[6] <= 4 && zH + deltaj[6] > -4 && zH + deltaj[6] <= 4 ) {
            zH += deltaj[6];
            xH += deltai[6]; 
            angleH = M_PI/2;
        }
        break;
    }
    case Qt::Key_8: { 
        if ( xH + deltai[7] > -4 && xH + deltai[7] <= 4 && zH + deltaj[7] > -4 && zH + deltaj[7] <= 4 ) {
            zH += deltaj[7];
            xH += deltai[7]; 
            angleH = M_PI;
        }
        break;
    }
    case Qt::Key_C: { 
        if (cameraValue) iniCamera0();
        else iniCamera1();
        break;
    }
    case Qt::Key_R: { 
        iniEscena();
        break;
    }
  }
  update();
}

void MyGLWidget::homerTransform (int x, int z) 
{
  // Matriu de transformació de Homer
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(x, -0.5, z));
  TG = glm::rotate(TG, angleH, glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(escalaH));
  TG = glm::translate(TG, -baseHomer);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}