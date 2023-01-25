#include "myopenglspace.h"

MyOpenGLSpace::MyOpenGLSpace(QWidget *parent)
    : QOpenGLWidget { parent }
{
    setDefault();
}

void MyOpenGLSpace::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_BUFFER_BIT);
}

void MyOpenGLSpace::paintGL()
{
   glClearColor(backRedColor, backGreenColor, backBlueColor, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0, 0, -(max / 2));
    if (vertices != NULL && indices != NULL) drawObj();
}

void MyOpenGLSpace::resizeGL(int w, int h)
{
   glViewport(0, 0, w, h);
}

void MyOpenGLSpace::drawObj() {
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
  if (projection == 0) {
           glOrtho(min - 1, max + 1, (min - 1) * 3/4, (max + 1) * 3/4, -5 , max * 2);
       } else {
           glFrustum(-1.42815, 1.42815, -1.569686, 1.569686, 1, max * 2);
       }
  glVertexPointer(3, GL_DOUBLE, 0, vertices);
  glEnableClientState(GL_VERTEX_ARRAY);      
   if(verticeType > 0) {
            glColor3d(vertRedColor, vertGreenColor, vertBlueColor); // Цвет поменять
            glPointSize(verticeSize * 2.0);
            if (verticeType == 1) {
                glEnable(GL_POINT_SMOOTH);
                glDrawArrays(GL_POINTS, 0, vertCount);
                glDisable(GL_POINT_SMOOTH);
            } else {
                glDrawArrays(GL_POINTS, 0, vertCount);
            }
    }
    glColor3d(lineRedColor, lineGreenColor, lineBlueColor); // Цвет поменять
    glLineWidth(lineWidth);
    if (lineType == 1) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(2, 0x00FF);
        glDrawElements(GL_LINES, edgeArrCount, GL_UNSIGNED_INT, indices);
        glDisable(GL_LINE_STIPPLE);
    } else {
        glDrawElements(GL_LINES, edgeArrCount, GL_UNSIGNED_INT, indices);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void MyOpenGLSpace::setDefault() {
    vertices = NULL;
    indices = NULL;
}
