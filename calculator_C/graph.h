#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"

extern "C" {
#include "s21_smartcalc.h"
}

namespace Ui {
class Graphics;
}

class Graphics : public QDialog {
    Q_OBJECT

public:
    explicit Graphics(QWidget *parent, QString str, double x, double xmax, double ymax);
    ~Graph();

private:
    Ui::Graphics *ui;
    QString Str_I;
    double X_I, XMax_I, YMax_I;
    void DrawGraphics();
};

#endif // GRAPH_H
