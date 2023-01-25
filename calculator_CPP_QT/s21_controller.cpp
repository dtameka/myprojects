#include "s21_controller.h"

std::string s21::Controller::calculate(std::string &input_string, double &x) {
  calculator->s21_smartcalc(input_string, x);
  return calculator->get_data();
}

bool s21::Controller::graph(QString str, double x, double xmax, double ymax) {
  graphics->get_graphics(str, x, xmax, ymax);
  return graphics->paint_flag_;
}

void s21::Controller::setDefaultGraph() {
  graphics->setGeometry(200, 200, 500, 500);
  graphics->setModal(true);
  graphics->exec();
}

s21::CreditCalc::credit_output_t s21::Controller::CreditCalculate(
    double &amount, int &term, double &interest_rate, std::string &type) {
  creditCalc->s21_credit_calculate(amount, term, interest_rate, type);
  return creditCalc->get_output();
}

s21::DepositCalc::deposit_output_t s21::Controller::DepositCalculate(
    s21::DepositCalc::deposit_input_t &input) {
  depositCalc->s21_deposit_calc(input);
  return depositCalc->get_output();
}
