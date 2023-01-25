#include "s21_creditcalc.h"

bool s21::CreditCalc::is_input_true(credit_input_t &input) {
  bool result = false;
  const double EPS = 1e-7;
  if (input.amount_ >= 1 &&
      input.credit_type_ != CreditCalc::credit_type_t::NONE &&
      input.interest_rate_ >= EPS && input.term_ >= 1)
    result = true;
  return result;
}

void s21::CreditCalc::s21_credit_calculate(double &amount, int &term,
                                           double &interest_rate,
                                           std::string &type) {
  credit_input_t input(amount, term, interest_rate, type);
  if (is_input_true(input)) {
    double interest_rate_m = input.interest_rate_ / (100.0 * 12.0);
    if (input.credit_type_ == CreditCalc::credit_type_t::ANNUITY) {
      output_.monthly_payment_ =
          ceil((input.amount_ *
                (interest_rate_m +
                 (interest_rate_m /
                  ((pow(1.0 + interest_rate_m, input.term_) - 1.0))))) *
               100) /
          100;
      output_.total_amount_ = output_.monthly_payment_ * input.term_;
      output_.overpayment_amount_ = output_.total_amount_ - input.amount_;
    } else if (input.credit_type_ == CreditCalc::credit_type_t::DECLINING) {
      double monthly_payment = input.amount_ / input.term_;
      double monthly_interest[200] = {0};
      double monthly_capital[200] = {0};
      double temp_input = input.amount_;
      output_.total_amount_ = 0.0;
      for (int i = 0; i < (int)input.term_; i++) {
        monthly_interest[i] = temp_input * interest_rate_m;
        monthly_capital[i] = monthly_payment;
        temp_input = temp_input - monthly_capital[i];
        output_.total_amount_ =
            output_.total_amount_ + monthly_capital[i] + monthly_interest[i];
        if (i == 0) {
          output_.first_payment_ = output_.total_amount_;
        } else {
          output_.last_payment_ = 0.0;
          output_.last_payment_ = monthly_capital[i] + monthly_interest[i];
        }
      }
      output_.overpayment_amount_ = output_.total_amount_ - input.amount_;
    }
  } else {
    std::string error_string = "Error - Please check if the input is correct.";
    output_.error_string_ = error_string;
  }
}

s21::CreditCalc::credit_input_t::credit_input_t(double &amount, int &term,
                                                double &interest_rate,
                                                std::string &type) {
  amount_ = amount;
  term_ = term;
  interest_rate_ = interest_rate;
  if (type == "Annuity") {
    credit_type_ = CreditCalc::credit_type_t::ANNUITY;
  } else {
    credit_type_ = CreditCalc::credit_type_t::DECLINING;
  }
}

s21::CreditCalc::credit_output_t s21::CreditCalc::get_output() {
  return output_;
}
