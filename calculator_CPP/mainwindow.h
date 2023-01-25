#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractButton>
#include <QDialog>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <string>

#include "s21_controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Controller *c, QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;

  int rscob_counter;
  int lscob_counter;

  QString input_point;
  s21::Controller *controller;

  void initialize_rules();
  void eq_rules();
  void digit_rules();
  void binary_rules();
  void unary_rules();
  void plusminus_rules();
  void lscob_rules();
  void rscob_rules();
  void x_rules();
  void dot_rules();
  void output_padding_credit(s21::CreditCalc::credit_output_t &output);
  void input_padding_deposit(s21::DepositCalc::deposit_input_t &input);
  void output_padding_deposit(s21::DepositCalc::deposit_output_t &output);

 private slots:
  void digits_show(QAbstractButton *button);

  void on_Button_Lscob_clicked();
  void on_Button_Rscob_clicked();
  void on_pushButton_X_clicked();
  void on_pushButton_dot_clicked();
  void on_pushButton_eq_clicked();
  void on_pushButton_Call_clicked();
  void on_pushButton_Graph_clicked();
  void on_pushButton_CreditCalc_clicked();
  void on_pushButton_Deposit_calculate_clicked();
  void on_pushButton_Deposit_clear_clicked();
  void on_pushButton_CreditClear_clicked();
  void on_pushButton_E_clicked();
};
}  // namespace s21
#endif  // MAINWINDOW_H
