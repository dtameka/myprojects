#include "s21_depositcalc.h"

int s21::DepositCalc::investment_into_days(investment_schema_t &schema) {
  int result = -1;
  if (schema == investment_schema_t::DAILY) {
    result = 1;
  } else if (schema == investment_schema_t::WEEKLY) {
    result = (int)(365 / 52);
  } else if (schema == investment_schema_t::MONTHLY) {
    result = (int)(365 / 12);
  } else if (schema == investment_schema_t::BIMONTHLY) {
    result = (int)(365 / 6);
  } else if (schema == investment_schema_t::TRIMESTERLY) {
    result = (int)(365 / 4);
  } else if (schema == investment_schema_t::QUADRIMESTERLY) {
    result = (int)(365 / 3);
  } else if (schema == investment_schema_t::SEMESTERLY) {
    result = (int)(365 / 2);
  } else if (schema == investment_schema_t::YEARLY) {
    result = 365;
  }
  return result;
}

void s21::DepositCalc::s21_deposit_calc(deposit_input_t &input) {
  result_.clear();
  double deposit_balance[10000] = {0};
  int term_day = (int)((double)(input.term_ * 30.417));
  double temp_deposit_amount = input.deposit_amount_;
  if (input.topup_schema_ != investment_schema_t::NOT) {
    for (int i = 0; i < term_day; i++) {
      int topup_step = investment_into_days(input.topup_schema_);
      if ((i + 1) % topup_step == 0 && topup_step != -1) {
        temp_deposit_amount = temp_deposit_amount + input.topup_amount_;
        result_.total_topup_amount_ += input.topup_amount_;
      }
      deposit_balance[i] = temp_deposit_amount;
    }
  } else {
    for (int i = 0; i < term_day; i++) {
      deposit_balance[i] = temp_deposit_amount;
    }
  }
  if (input.withdraw_schema_ != investment_schema_t::NOT) {
    for (int i = 0; i < term_day; i++) {
      int draw_step = investment_into_days(input.withdraw_schema_);
      if ((i + 1) % draw_step == 0 && draw_step != -1) {
        temp_deposit_amount -= input.draw_amount_;
        result_.total_draw_amount_ += input.draw_amount_;
      }
      deposit_balance[i] = temp_deposit_amount;
    }
  } else {
    for (int i = 0; i < term_day; i++) {
      deposit_balance[i] = temp_deposit_amount;
    }
  }
  double interest_rate_d = input.interest_rate_ / (100.0 * 365.0);
  if (input.reinvest_schema_ != investment_schema_t::NOT) {
    double temp_compund_interest = 0.0;
    for (int i = 0; i < term_day; i++) {
      int compounding_step = investment_into_days(input.reinvest_schema_);
      temp_compund_interest =
          temp_compund_interest + temp_deposit_amount * interest_rate_d;
      if ((i + 1) % compounding_step == 0 && compounding_step != -1) {
        temp_deposit_amount = temp_deposit_amount + temp_compund_interest;
        result_.total_compound_amount_ += temp_compund_interest;
        temp_compund_interest = 0.0;
      }
      deposit_balance[i] = temp_deposit_amount;
    }
  } else {
    for (int i = 0; i < term_day; i++) {
      deposit_balance[i] = temp_deposit_amount;
    }
  }
  //  Total calculate
  for (int i = 0; i < term_day; i++) {
    result_.final_interest_amount_ =
        result_.final_interest_amount_ + deposit_balance[i] * interest_rate_d;
    result_.final_deposit_amount_ = deposit_balance[i];
  }
  //  Tax calculate
  double free_deposit_amount =
      input.income_tax_ceiling_ * input.refinance_rate_;
  if (free_deposit_amount > 0 &&
      result_.final_interest_amount_ > free_deposit_amount) {
    result_.tax_payment_ =
        (result_.final_interest_amount_ - free_deposit_amount) *
        input.income_tax_rate_ / 100;
  } else {
    result_.tax_payment_ = 0.0;
  }
  result_.net_amount_ = result_.final_interest_amount_ - result_.tax_payment_;
  // Negate check
  const double EPS = 1e-7;
  if (result_.final_deposit_amount_ > EPS &&
      result_.final_interest_amount_ > EPS)
    result_.negate_check_ = true;
  else
    result_.negate_check_ = false;
}

s21::DepositCalc::deposit_output_t s21::DepositCalc::get_output() {
  return result_;
}

void s21::DepositCalc::deposit_output_t::clear() {
  final_interest_amount_ = 0.0;
  final_deposit_amount_ = 0.0;
  net_amount_ = 0.0;
  tax_payment_ = 0.0;
  total_topup_amount_ = 0.0;
  total_draw_amount_ = 0.0;
  total_compound_amount_ = 0.0;
  negate_check_ = true;
}
