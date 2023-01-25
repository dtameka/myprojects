#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QDialog>
#include <QWidget>

#include "qcustomplot.h"
#include "s21_smartcalc.h"

namespace Ui {
class Graphics;
}
namespace s21 {
class Graphics : public QDialog {
  Q_OBJECT

 public:
  explicit Graphics();
  void get_graphics(QString str, double x, double xmax, double ymax);
  bool paint_flag_;
  ~Graphics();

 private:
  Ui::Graphics *ui;
  QString Str_;
  double X_, Xmax_, Ymax_;
  bool Graphics_Paint();
};
}  // namespace s21

#endif  // GRAPHICS_H
