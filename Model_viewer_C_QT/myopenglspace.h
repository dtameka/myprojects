#ifndef MYOPENGLSPACE_H_
#define MYOPENGLSPACE_H_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSettings>
#include "viewer.h"

class MyOpenGLSpace : public QOpenGLWidget, public QOpenGLFunctions {
    Q_OBJECT

    private:
        void setDefault();

    public:
        void drawObj();
        int previousScale = 10;
        double *vertices;
        double *vertices2;
        int *indices;
        int *indices2;
        int vertCount;
        int edgeCount;
        int vertArrCount;
        int edgeArrCount;
        double min, max;

        float backRedColor;
        float backGreenColor;
        float backBlueColor;
        float lineRedColor;
        float lineGreenColor;
        float lineBlueColor;
        float vertRedColor;
        float vertGreenColor;
        float vertBlueColor;

        int verticeType;
        float verticeSize;

        int lineType;
        float lineWidth;

        int projection;


    MyOpenGLSpace(QWidget *parent = nullptr);

    protected:
        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w, int h) override;
};
#endif  // MYOPENGLSPACE_H_
