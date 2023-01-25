#include "s21_creditcalc.h"
#include "s21_depositcalc.h"
#include "s21_graphics.h"

#ifndef GRAPHICS_H
#include "smartcalc.h"
#endif

namespace s21 {

class Controller {
 public:
  std::string calculate(std::string &input_string, double &x);

  bool graph(QString str, double x, double xmax, double ymax);

  s21::CreditCalc::credit_output_t CreditCalculate(double &amount, int &term,
                                                   double &interest_rate,
                                                   std::string &type);

  Controller(DepositCalc *deposit, CreditCalc *credit, Graphics *g,
             SmartCalculator *c)
      : depositCalc(deposit), creditCalc(credit), graphics(g), calculator(c) {}

  void setDefaultGraph();

  s21::DepositCalc::deposit_output_t DepositCalculate(
      s21::DepositCalc::deposit_input_t &input);

 private:
  DepositCalc *depositCalc;
  CreditCalc *creditCalc;
  Graphics *graphics;
  SmartCalculator *calculator;
};
}  // namespace s21
