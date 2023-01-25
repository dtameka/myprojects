#include "graphics.h"
#include "ui_graphics.h"

Graphics::Graphics(QWidget *parent, QString str, double x, double xmax, double ymax) :
    QDialog(parent), ui(new Ui::Graphics) {
    ui->setupUi(this);
    Str_I = str;
    X_I = x;
    Xmax_I = xmax;
    Ymax_I = ymax;
    ui->widget->setMouseTracking(true);
    ui->widget->setInteraction(QCP::iRangeZoom, true);
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->setGeometry(10, 10, 500, 500);
    Graphics_Paint();
}

Graphics::~Graphics() {
    delete ui;
}

void Graphics::Graphics_Paint() {
    ui->widget->addGraph();
    QPen pen;
    pen.setColor(QColor(132, 38, 87));
    ui->widget->addGraph()->setPen(pen);
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    int string_lengh = Str_I.length();
    if (string_lengh) {
        double Xmin = -Xmax_I;
        double Xmax = Xmax_I;
        if (Xmin > Xmax) {
            Xmin = Xmax_I;
            Xmax = -Xmax_I;
        }
        double Ymin = -Ymax_I;
        double Ymax = Ymax_I;
        if (Ymin > Ymax) {
            Ymin = Ymax_I;
            Ymax = -Ymax_I;
        }

        ui->widget->xAxis->setRange(Xmin * 1.05, Xmax * 1.05);
        ui->widget->yAxis->setRange(Ymin * 1.05, Ymax * 1.05);
        double step_x;
        if (Xmin > -1000 && Xmax < 1000 && Ymin > -1000 && Ymax < 1000) step_x = 0.001;
        else if (Xmin > -10000 && Xmax < 10000 && Ymin > -10000 && Ymax < 10000) step_x = 0.01;
        else step_x = 0.1;

        QByteArray tmp_arr = Str_I.toLocal8Bit();
        char* tmp_input = tmp_arr.data();
        char* output = s21_smartcalc(tmp_input, 0.0);
        char* rpn_string = convert_infix_to_postfix(tmp_input);
        double y_temp = 0.0;
        QVector<double> x_dot;
        QVector<double> y_dot;
        double counter = Xmin;
        int errors = 0;
        if (*output != 'E') {
            while (counter <= Xmax) {
                y_temp = postfix_caclculator(rpn_string, counter, &errors);
                if (y_temp > Ymin && y_temp < Ymax && qIsNaN(y_temp) == false) {
                    x_dot.push_back(counter);
                    y_dot.push_back(y_temp);
                }
                counter+=step_x;
            }
        }
        ui->widget->graph(0)->addData(x_dot, y_dot);
        ui->widget->replot();
        x_dot.clear();
        y_dot.clear();
        free(output);
        free(rpn_string);
    } else {
        QMessageBox::warning(this, "Warn", "Please check input parameters\n");
        (this)->close();
    }
}
