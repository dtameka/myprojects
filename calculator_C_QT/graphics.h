#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QDialog>
#include <QWidget>
#include "qcustomplot.h"

extern "C" {
#include "s21_smartcalc.h"
}

namespace Ui {
class Graphics;
}

class Graphics : public QDialog
{
    Q_OBJECT

public:
    explicit Graphics(QWidget *parent, QString str, double x, double xmax, double ymax);
    ~Graphics();

private:
    Ui::Graphics *ui;
    QString Str_I;
    double X_I, Xmax_I, Ymax_I;
    void Graphics_Paint();
};

#endif // GRAPHICS_H
