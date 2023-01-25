#include <QMessageBox>
#include <QVector>
#include <QtGlobal>
#include "graph.h"
#include "ui_graph.h"
#include "qcustomplot.h"

Graph::Graph(QWidget *parent, QString str, double x, double xmax, double ymax) :
    QDialog(parent), ui(new Ui::Graph) {

    ui->setupUi(this);

    Str_I = str;
    X_I = x;
    XMax_I = xmax;
    YMax_I = ymax;
    ui->
    ui->widget->setInteraction(QCP::iRangeZoom, true);
    ui->widget->setInteraction(QCP::iRangeDrag, true);


    ui->widget->setGeometry(10, 10, 510, 510);
    DrawGraphics();
}

Graphics::~Graphics() {
    delete ui;
}

void Graphics::DrawGraphics() {
    ui->widget->addGraph();
    QPen pen;
    pen.setColor(QColor(132, 38, 87));
    ui->widget->addGraph()->setPen(pen);
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    int string_length = Str_Import.length();
    if (string_length) {
        double XMin = -XMax_Import;
        double XMax = XMax_Import;
        if (XMin > XMax) {
            XMin = XMax_Import;
            XMax = -XMax_Import;
        }
        double YMin = -YMax_Import;
        double YMax = YMax_Import;
        if (YMin > YMax) {
            YMin = YMax_Import;
            YMax = -YMax_Import;
        }

        ui->widget->xAxis->setRange(XMin * 1.05, XMax * 1.05);
        ui->widget->yAxis->setRange(YMin * 1.05, YMax * 1.05);
        // 10 - 0.002 | 100 - 0.005 | 1000 - 0.008 | 10000 - 0.1 | 100000 - 1
        double step_x;
        if (XMin > -1000 && XMax < 1000 && YMin > -1000 && YMax < 1000) step_x = 0.001;
        else if (XMin > -10000 && XMax < 10000 && YMin > -10000 && YMax < 10000) step_x = 0.01;
        else step_x = 0.1;

        QByteArray tmp_array = Str_Import.toLocal8Bit();
        char* tmp_input_string = tmp_array.data();
        char* output = s21_smartcalc(tmp_input_string, 0.0);
        char* rpn_string = convert_infix_to_postfix(tmp_input_string);
        double y_temp = 0.0;
        QVector<double> x_dot;
        QVector<double> y_dot;
        double counter = XMin;
        int processing_errors = 0;
        if (*output != 'E') {
            while (counter <= XMax) {
                y_temp = postfix_caclculator(rpn_string, counter, &processing_errors);
                if (y_temp > YMin && y_temp < YMax && qIsNaN(y_temp) == false) {
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
        QMessageBox::warning(this, "Attention!","Please re-check input parameters for plotting the graph\n");
        (this)->close();
    }
}
