#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QLocale::setDefault(QLocale::C);
    QApplication a(argc, argv);
    s21::SmartCalculator model;
    s21::Graphics model2;
    s21::CreditCalc model3;
    s21::DepositCalc model4;
    s21::Controller controller(&model4, &model3, &model2, &model);
    s21::MainWindow view(&controller);
    view.show();
    return a.exec();           
}
