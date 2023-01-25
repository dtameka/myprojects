#include "s21_graphics.h"

#include "ui_s21_graphics.h"

s21::Graphics::Graphics()
    : ui(nullptr), Str_("\0"), X_(0), Xmax_(0), Ymax_(0) {}

void s21::Graphics::get_graphics(QString str, double x, double xmax,
                                 double ymax) {
  ui = new Ui::Graphics;
  ui->setupUi(this);
  Str_ = str;
  X_ = x;
  Xmax_ = xmax;
  Ymax_ = ymax;
  ui->widget->setMouseTracking(true);
  ui->widget->setInteraction(QCP::iRangeZoom, true);
  ui->widget->setInteraction(QCP::iRangeDrag, true);
  ui->widget->setGeometry(10, 10, 500, 500);
  paint_flag_ = Graphics_Paint();
}

s21::Graphics::~Graphics() { delete ui; }

bool s21::Graphics::Graphics_Paint() {
  bool paint_tmp = true;
  if (Xmax_ != 0 && Ymax_ != 0 && Str_.contains("X") && X_ != 0) {
    ui->widget->addGraph();
    QPen pen;
    pen.setColor(QColor(132, 38, 87));
    ui->widget->addGraph()->setPen(pen);
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    if (Str_.length()) {
      double Xmin = -Xmax_;
      double Xmax = Xmax_;
      if (Xmin > Xmax) {
        Xmin = Xmax_;
        Xmax = -Xmax_;
      }
      double Ymin = -Ymax_;
      double Ymax = Ymax_;
      if (Ymin > Ymax) {
        Ymin = Ymax_;
        Ymax = -Ymax_;
      }
      ui->widget->xAxis->setRange(Xmin * 1.05, Xmax * 1.05);
      ui->widget->yAxis->setRange(Ymin * 1.05, Ymax * 1.05);
      double step_x;
      if (Xmin > -1000 && Xmax < 1000 && Ymin > -1000 && Ymax < 1000)
        step_x = 0.001;
      else if (Xmin > -10000 && Xmax < 10000 && Ymin > -10000 && Ymax < 10000)
        step_x = 0.01;
      else
        step_x = 0.1;

      std::string tmp_input = Str_.toLocal8Bit().constData();
      s21::SmartCalculator result;
      result.s21_smartcalc(tmp_input, 0.0);
      std::string output = result.get_data();
      if (*output.data() != 'E') {
        std::string rpn_string = result.convert_infix_to_postfix(tmp_input);
        double y_temp = 0.0;
        QVector<double> x_dot;
        QVector<double> y_dot;
        double counter = Xmin;
        int errors = 0;
        if (*output.data() != 'E') {
          while (counter <= Xmax) {
            y_temp = result.postfix_calculator(rpn_string, counter, errors);
            if (y_temp > Ymin && y_temp < Ymax && qIsNaN(y_temp) == false) {
              x_dot.push_back(counter);
              y_dot.push_back(y_temp);
            }
            counter += step_x;
          }
        }
        ui->widget->graph(0)->addData(x_dot, y_dot);
        ui->widget->replot();
        x_dot.clear();
        y_dot.clear();
      } else {
        paint_tmp = false;
      }
    } else {
      (this)->close();
    }
  } else {
    paint_tmp = false;
  }
  return paint_tmp;
}
