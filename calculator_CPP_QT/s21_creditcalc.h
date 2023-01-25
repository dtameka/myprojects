#ifndef CREDITCALC_H_
#define CREDITCALC_H_

#include <cmath>
#include <string>

namespace s21 {

class CreditCalc {
 public:
  enum class credit_type_t { NONE, ANNUITY, DECLINING };

  CreditCalc() {}
  ~CreditCalc() {}
  void s21_credit_calculate(double &amount, int &term, double &interest_rate,
                            std::string &type);

  class credit_output_t {
   public:
    double monthly_payment_ = 0.0;
    double overpayment_amount_ = 0.0;
    double total_amount_ = 0.0;
    double first_payment_ = 0.0;
    double last_payment_ = 0.0;
    std::string error_string_ = "\0";
  };

  class credit_input_t {
   public:
    credit_input_t(double &amount, int &term, double &interest_rate,
                   std::string &type);
    double amount_ = 0.0;
    int term_ = 0;
    double interest_rate_ = 0.0;
    credit_type_t credit_type_;
  };

  credit_output_t get_output();

 private:
  credit_output_t output_;

  bool is_input_true(credit_input_t &input);
};
}  // namespace s21
#endif  // CREDITCALC_H_
