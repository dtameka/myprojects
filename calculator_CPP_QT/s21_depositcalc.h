#ifndef S21_DEPOSITCALC_H_
#define S21_DEPOSITCALC_H_

#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

namespace s21 {

class DepositCalc {
 public:
  class deposit_output_t;
  class deposit_input_t;

  enum class investment_schema_t {
    NOT,
    DAILY,
    WEEKLY,
    MONTHLY,
    BIMONTHLY,
    TRIMESTERLY,
    QUADRIMESTERLY,
    SEMESTERLY,
    YEARLY
  };

  DepositCalc() {}
  ~DepositCalc() {}

  void s21_deposit_calc(deposit_input_t &input);

  deposit_output_t get_output();

  class deposit_input_t {
   public:
    double deposit_amount_ = 0.0;
    int term_ = 0;
    double interest_rate_ = 0.0;
    double income_tax_rate_ = 0.0;
    double income_tax_ceiling_ = 0.0;
    double refinance_rate_ = 0.0;
    investment_schema_t reinvest_schema_ = investment_schema_t::NOT;
    investment_schema_t topup_schema_ = investment_schema_t::NOT;
    double topup_amount_ = 0.0;
    investment_schema_t withdraw_schema_ = investment_schema_t::NOT;
    double draw_amount_ = 0.0;
  };

  class deposit_output_t {
   public:
    double final_interest_amount_ = 0.0;
    double final_deposit_amount_ = 0.0;
    double net_amount_ = 0.0;
    double tax_payment_ = 0.0;
    double total_topup_amount_ = 0.0;
    double total_draw_amount_ = 0.0;
    double total_compound_amount_ = 0.0;
    bool negate_check_ = true;
    void clear();
  };

 private:
  deposit_output_t result_;

  int investment_into_days(investment_schema_t &schema);
};
}  // namespace s21
#endif  // S21_DEPOSITCALC_H_
