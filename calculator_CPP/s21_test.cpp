#include <gtest/gtest.h>

#include <cmath>

#include "s21_creditcalc.h"
#include "s21_depositcalc.h"
#include "s21_smartcalc.h"

TEST(SmartCalc_2, math_1) {
  s21::SmartCalculator model;
  std::string input_string = "(7+5)+(6+7)+5";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "30.0000000");
}

TEST(SmartCalc_2, math_2) {
  s21::SmartCalculator model;
  std::string input_string = "SIN(0)+COS(0)";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "1.0000000");
}

TEST(SmartCalc_2, math_3) {
  s21::SmartCalculator model;
  std::string input_string = "-1E1+COS(0)";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "-9.0000000");
}

TEST(SmartCalc_2, math_4) {
  s21::SmartCalculator model;
  std::string input_string = "2E5000+3E5000";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "inf");
}

TEST(SmartCalc_2, math_5) {
  s21::SmartCalculator model;
  std::string input_string = "(12*2/3)%3";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "2.0000000");
}

TEST(SmartCalc_2, math_6) {
  s21::SmartCalculator model;
  std::string input_string = "2^3";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "8.0000000");
}

TEST(SmartCalc_2, math_7) {
  s21::SmartCalculator model;
  std::string input_string = "2^2";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "4.0000000");
}

TEST(SmartCalc_2, math_8) {
  s21::SmartCalculator model;
  std::string input_string = "LOG(5)+LN(3)+SQRT(2)";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "3.2117959");
}

TEST(SmartCalc_2, math_9) {
  s21::SmartCalculator model;
  std::string input_string = "ACOS(0.2)/-(ASIN(0.3))";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "-4.4944910");
}

TEST(SmartCalc_2, math_10) {
  s21::SmartCalculator model;
  std::string input_string = "1-2*3^4/5";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "-31.4000000");
}

TEST(SmartCalc_2, math_11) {
  s21::SmartCalculator model;
  std::string input_string =
      "8*9+3/2*SIN(2+7.295)-LN(74*2)/2.2+10.389/(2*9.2)+2+9.000";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "81.4872804");
}

TEST(SmartCalc_2, math_12) {
  s21::SmartCalculator model;
  std::string input_string = "ACOS(0.0)+ASIN(0.0)-ATAN(-0.0)";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "1.5707963");
}

TEST(SmartCalc_2, math_13) {
  s21::SmartCalculator model;
  std::string input_string = "2^X+X%2-COS(X)";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "0.0000000");
}

TEST(SmartCalc_2, math_14) {
  s21::SmartCalculator model;
  std::string input_string = "-COS(-X)+LOG(X)-(TAN(-X))";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "-inf");
}

TEST(SmartCalc_2, math_15) {
  s21::SmartCalculator model;
  std::string input_string = "(11+22)-(((33*23)/(6-12.25)+3)-11)";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "162.4400000");
}

TEST(SmartCalc_2, math_16) {
  s21::SmartCalculator model;
  std::string input_string = "1/12/100*(1+1/12/100)^12";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "0.0008417");
}

TEST(SmartCalc_2, math_17) {
  s21::SmartCalculator model;
  std::string input_string = "SIN(SIN(5)+SIN(5)+COS(2))";
  model.s21_smartcalc(input_string, 0);
  ASSERT_EQ(model.get_data(), "-0.7226284");
}

TEST(SmartCalc_2, credit_1) {
  s21::CreditCalc model;
  double amount = 1000.0;
  int term = 1;
  double interest_rate = 120.00;
  std::string type = "Declining";
  model.s21_credit_calculate(amount, term, interest_rate, type);
  s21::CreditCalc::credit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.overpayment_amount_, 100.0);
}

TEST(SmartCalc_2, credit_2) {
  s21::CreditCalc model;
  double amount = 1000.0;
  int term = 1;
  double interest_rate = 120.00;
  std::string type = "Annuity";
  model.s21_credit_calculate(amount, term, interest_rate, type);
  s21::CreditCalc::credit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.overpayment_amount_, 100.0);
}

TEST(SmartCalc_2, credit_3) {
  s21::CreditCalc model;
  double amount = 1000.0;
  int term = 1;
  double interest_rate = 120.00;
  std::string type = "Annuity";
  model.s21_credit_calculate(amount, term, interest_rate, type);
  s21::CreditCalc::credit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.overpayment_amount_, 100.0);
}

TEST(SmartCalc_2, deposit_1) {
  s21::DepositCalc model;
  s21::DepositCalc::deposit_input_t input;
  input.deposit_amount_ = 1000.00;
  input.term_ = 1;
  input.interest_rate_ = 120;
  model.s21_deposit_calc(input);
  s21::DepositCalc::deposit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.final_interest_amount_, 98.630135);
}

TEST(SmartCalc_2, deposit_2) {
  s21::DepositCalc model;
  s21::DepositCalc::deposit_input_t input;
  input.deposit_amount_ = 1000.00;
  input.term_ = 1;
  input.interest_rate_ = 120;
  input.reinvest_schema_ = s21::DepositCalc::investment_schema_t::DAILY;
  model.s21_deposit_calc(input);
  s21::DepositCalc::deposit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.final_interest_amount_, 103.81969);
}

TEST(SmartCalc_2, deposit_3) {
  s21::DepositCalc model;
  s21::DepositCalc::deposit_input_t input;
  input.deposit_amount_ = 1000.00;
  input.term_ = 1;
  input.interest_rate_ = 120;
  input.reinvest_schema_ = s21::DepositCalc::investment_schema_t::DAILY;
  input.topup_schema_ = s21::DepositCalc::investment_schema_t::MONTHLY;
  input.topup_amount_ = 1000.0;
  model.s21_deposit_calc(input);
  s21::DepositCalc::deposit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.final_interest_amount_, 207.63939);
}

TEST(SmartCalc_2, deposit_4) {
  s21::DepositCalc model;
  s21::DepositCalc::deposit_input_t input;
  input.deposit_amount_ = 1000.00;
  input.term_ = 6;
  input.interest_rate_ = 120;
  input.reinvest_schema_ = s21::DepositCalc::investment_schema_t::DAILY;
  input.withdraw_schema_ = s21::DepositCalc::investment_schema_t::BIMONTHLY;
  input.draw_amount_ = 10.0;
  model.s21_deposit_calc(input);
  s21::DepositCalc::deposit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.final_interest_amount_, 795.4278);
}

TEST(SmartCalc_2, deposit_5) {
  s21::DepositCalc model;
  s21::DepositCalc::deposit_input_t input;
  input.deposit_amount_ = 1000.00;
  input.term_ = 6;
  input.interest_rate_ = 120;
  input.reinvest_schema_ = s21::DepositCalc::investment_schema_t::DAILY;
  input.withdraw_schema_ = s21::DepositCalc::investment_schema_t::SEMESTERLY;
  input.draw_amount_ = 10.0;
  input.income_tax_ceiling_ = 10.00;
  input.refinance_rate_ = 10;
  input.income_tax_rate_ = 13;

  model.s21_deposit_calc(input);
  s21::DepositCalc::deposit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.tax_payment_, 92.537689);
}

TEST(SmartCalc_2, deposit_6) {
  s21::DepositCalc model;
  s21::DepositCalc::deposit_input_t input;
  input.deposit_amount_ = 1000.00;
  input.term_ = 6;
  input.interest_rate_ = 120;
  input.reinvest_schema_ = s21::DepositCalc::investment_schema_t::DAILY;
  input.withdraw_schema_ = s21::DepositCalc::investment_schema_t::SEMESTERLY;
  input.draw_amount_ = 10.0;
  input.income_tax_ceiling_ = 1000.00;
  input.refinance_rate_ = 10;
  input.income_tax_rate_ = 13;

  model.s21_deposit_calc(input);
  s21::DepositCalc::deposit_output_t result = model.get_output();
  ASSERT_FLOAT_EQ(result.tax_payment_, 0.0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
