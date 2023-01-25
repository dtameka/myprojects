/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "s21_test.check" instead.
 */

#include <check.h>

#line 1 "s21_test.check"
#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include "s21_smartcalc.h"
#include "s21_creditcalc.h"
#include "s21_depositcalc.h"


START_TEST(s21_smartcalc_test_1) {
  char str[] = "-SIN(-X)+COS(X";
  char output[] = "Error - rpn";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_2) {
  char str[] = "1++1+/2";
  char output[] = "Error - rpn";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_3) {
  char str[] = "1.1111111+2.0000003";
  char output[] = "3.1111114";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_4) {
  char str[] = "-";
  char output[] = "Error - rpn";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_5) {
  char str[] = "-1.0/0.0";
  char output[] = "-inf";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_6) {
  char str[] = "1.0/0.0";
  char output[] = "inf";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_7) {
  char str[] = "COS(56)";
  char output[] = "0.8532201";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_8) {
  char str[] = "SIN(96^X)";
  char output[] = "0.6263764";
  char* calc_output = s21_smartcalc(str, 3);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_9) {
  char str[] = "COS(56)+SIN(96^3)";
  char output[] = "1.4795966";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_10) {
  char str[] = "20.20+(20+20)*2";
  char output[] = "100.2000000";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_11) {
  char str[] = "SQRT(112/13)";
  char output[] = "2.9351975";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_12) {
  char str[] = "SQRT(-2)";
  char output[] = "-nan";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_13) {
  char str[] = "TAN(54)+5";
  char output[] = "5.6738001";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_14) {
  char str[] = "LOG(45)/2";
  char output[] = "0.8266063";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_15) {
  char str[] = "LN(45)/2";
  char output[] = "1.9033312";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_16) {
  char str[] = "ACOS(45)/2";
  char output[] = "nan";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_17) {
  char str[] = "ACOS(0.28)*6";
  char output[] = "7.7220133";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_smartcalc_test_18) {
  char str[] = "ASING(0.28)/2";
  char output[] = "0.1418971";
  char* calc_output = s21_smartcalc(str, 0);
  ck_assert_str_eq(calc_output, output);
  free(calc_output);
}
END_TEST

START_TEST(s21_creditcalc_test_1) {
  int max_length = 256;
  credit_input_t input;
  credit_output_t output;
  initialize_struct_input(&input);
  initialize_struct_output(&output);
  input.amount = 100000.0;
  input.term = 12;
  input.interest_rate = 10.0;
  input.credit_type = ANNUITY;
  output = s21_credit_calculate(&input);

  char result1[] = "5504.00";
  char* output1 = (char*)calloc(256, sizeof(char));
  snprintf(output1, max_length, "%.2lf", output.overpayment_amount);
  ck_assert_str_eq(result1, output1);
  free(output1);

  char result2[] = "8792.00";
  char* output2 = (char*)calloc(256, sizeof(char));
  snprintf(output2, max_length, "%.2lf", output.monthly_payment);
  ck_assert_str_eq(result2, output2);
  free(output2);

  char result3[] = "105504.00";
  char* output3 = (char*)calloc(256, sizeof(char));
  snprintf(output3, max_length, "%.2lf", output.total_amount);
  ck_assert_str_eq(result3, output3);
  free(output3);
}
END_TEST

START_TEST(s21_creditcalc_test_2) {
  int max_length = 256;
  credit_input_t input;
  credit_output_t output;
  initialize_struct_input(&input);
  initialize_struct_output(&output);
  input.amount = 100000.0;
  input.term = 12;
  input.interest_rate = 10.0;
  input.credit_type = DECLINING;
  output = s21_credit_calculate(&input);

  char result1[] = "5416.67";
  char* output1 = (char*)calloc(256, sizeof(char));
  snprintf(output1, max_length, "%.2lf", output.overpayment_amount);
  ck_assert_str_eq(result1, output1);
  free(output1);

  char result2[] = "9166.67";
  char* output2 = (char*)calloc(256, sizeof(char));
  snprintf(output2, max_length, "%.2lf", output.first_payment);
  ck_assert_str_eq(result2, output2);
  free(output2);

  char result3[] = "105416.67";
  char* output3 = (char*)calloc(256, sizeof(char));
  snprintf(output3, max_length, "%.2lf", output.total_amount);
  ck_assert_str_eq(result3, output3);
  free(output3);

  char result4[] = "8402.78";
  char* output4 = (char*)calloc(256, sizeof(char));
  snprintf(output4, max_length, "%.2lf", output.last_payment);
  ck_assert_str_eq(result4, output4);
  free(output4);
}
END_TEST

START_TEST(s21_depositcalc_test_1) {
  int max_length = 256;
  deposit_input_t input;
  deposit_output_t output;
  initialize_struct_input_deposit(&input);
  initialize_struct_output_deposit(&output);
  input.deposit_amount = 100000.0;
  input.term = 12;
  input.interest_rate = 10.0;
  input.income_tax_rate = 13.0;
  input.income_tax_ceiling = 0.0;
  input.refinance_rate = 8;
  input.reinvest_schema = NOT;
  input.topup_schema = NOT;
  input.topup_amount = 10.0;
  input.withdraw_schema = NOT;
  input.draw_amount = 5.0;
  output = s21_deposit_calc(&input);

  char result1[] = "100000.00";
  char* output1 = (char*)calloc(256, sizeof(char));
  snprintf(output1, max_length, "%.2lf", output.final_deposit_amount);
  ck_assert_str_eq(output1, result1);
  free(output1);

  char result2[] = "10000.00";
  char* output2 = (char*)calloc(256, sizeof(char));
  snprintf(output2, max_length, "%.2lf", output.final_interest_amount);
  ck_assert_str_eq(output2, result2);
  free(output2);

  char result3[] = "0.00";
  char* output3 = (char*)calloc(256, sizeof(char));
  snprintf(output3, max_length, "%.2lf", output.tax_payment);
  ck_assert_str_eq(output3, result3);
  free(output3);

  char result4[] = "10000.00";
  char* output4 = (char*)calloc(256, sizeof(char));
  snprintf(output4, max_length, "%.2lf", output.net_amount);
  ck_assert_str_eq(output4, result4);
  free(output4);

  char result5[] = "0.00";
  char* output5 = (char*)calloc(256, sizeof(char));
  snprintf(output5, max_length, "%.2lf", output.total_compound_amount);
  ck_assert_str_eq(output5, result5);
  free(output5);

  char result6[] = "0.00";
  char* output6 = (char*)calloc(256, sizeof(char));
  snprintf(output6, max_length, "%.2lf", output.total_topup_amount);
  ck_assert_str_eq(output6, result6);
  free(output6);

  char result7[] = "0.00";
  char* output7 = (char*)calloc(256, sizeof(char));
  snprintf(output7, max_length, "%.2lf", output.total_draw_amount);
  ck_assert_str_eq(output7, result7);
  free(output7);
}
END_TEST

START_TEST(s21_depositcalc_test_2) {
  int max_length = 256;
  deposit_input_t input;
  deposit_output_t output;
  initialize_struct_input_deposit(&input);
  initialize_struct_output_deposit(&output);
  input.deposit_amount = 100000.0;
  input.term = 12;
  input.interest_rate = 10.0;
  input.income_tax_rate = 13.0;
  input.income_tax_ceiling = 0.0;
  input.refinance_rate = 8;
  input.reinvest_schema = DAILY;
  input.topup_schema = SEMESTERLY;
  input.topup_amount = 10000.0;
  input.withdraw_schema = NOT;
  input.draw_amount = 5.0;
  output = s21_deposit_calc(&input);

  char result1[] = "132618.69";
  char* output1 = (char*)calloc(256, sizeof(char));
  snprintf(output1, max_length, "%.2lf", output.final_deposit_amount);
  ck_assert_str_eq(output1, result1);
  free(output1);

  char result2[] = "12622.15";
  char* output2 = (char*)calloc(256, sizeof(char));
  snprintf(output2, max_length, "%.2lf", output.final_interest_amount);
  ck_assert_str_eq(output2, result2);
  free(output2);

  char result3[] = "0.00";
  char* output3 = (char*)calloc(256, sizeof(char));
  snprintf(output3, max_length, "%.2lf", output.tax_payment);
  ck_assert_str_eq(output3, result3);
  free(output3);

  char result4[] = "12622.15";
  char* output4 = (char*)calloc(256, sizeof(char));
  snprintf(output4, max_length, "%.2lf", output.net_amount);
  ck_assert_str_eq(output4, result4);
  free(output4);

  char result5[] = "12618.69";
  char* output5 = (char*)calloc(256, sizeof(char));
  snprintf(output5, max_length, "%.2lf", output.total_compound_amount);
  ck_assert_str_eq(output5, result5);
  free(output5);

  char result6[] = "20000.00";
  char* output6 = (char*)calloc(256, sizeof(char));
  snprintf(output6, max_length, "%.2lf", output.total_topup_amount);
  ck_assert_str_eq(output6, result6);
  free(output6);

  char result7[] = "0.00";
  char* output7 = (char*)calloc(256, sizeof(char));
  snprintf(output7, max_length, "%.2lf", output.total_draw_amount);
  ck_assert_str_eq(output7, result7);
  free(output7);
}
END_TEST

START_TEST(s21_depositcalc_test_3) {
  int max_length = 256;
  deposit_input_t input;
  deposit_output_t output;
  initialize_struct_input_deposit(&input);
  initialize_struct_output_deposit(&output);
  input.deposit_amount = 100000.0;
  input.term = 12;
  input.interest_rate = 10.0;
  input.income_tax_rate = 13.0;
  input.income_tax_ceiling = 100.0;
  input.refinance_rate = 8;
  input.reinvest_schema = DAILY;
  input.topup_schema = SEMESTERLY;
  input.topup_amount = 10000.0;
  input.withdraw_schema = WEEKLY;
  input.draw_amount = 1000.0;
  output = s21_deposit_calc(&input);

  char result1[] = "75150.59";
  char* output1 = (char*)calloc(256, sizeof(char));
  snprintf(output1, max_length, "%.2lf", output.final_deposit_amount);
  ck_assert_str_eq(output1, result1);
  free(output1);

  char result2[] = "7152.55";
  char* output2 = (char*)calloc(256, sizeof(char));
  snprintf(output2, max_length, "%.2lf", output.final_interest_amount);
  ck_assert_str_eq(output2, result2);
  free(output2);

  char result3[] = "825.83";
  char* output3 = (char*)calloc(256, sizeof(char));
  snprintf(output3, max_length, "%.2lf", output.tax_payment);
  ck_assert_str_eq(output3, result3);
  free(output3);

  char result4[] = "6326.72";
  char* output4 = (char*)calloc(256, sizeof(char));
  snprintf(output4, max_length, "%.2lf", output.net_amount);
  ck_assert_str_eq(output4, result4);
  free(output4);

  char result5[] = "7150.59";
  char* output5 = (char*)calloc(256, sizeof(char));
  snprintf(output5, max_length, "%.2lf", output.total_compound_amount);
  ck_assert_str_eq(output5, result5);
  free(output5);

  char result6[] = "20000.00";
  char* output6 = (char*)calloc(256, sizeof(char));
  snprintf(output6, max_length, "%.2lf", output.total_topup_amount);
  ck_assert_str_eq(output6, result6);
  free(output6);

  char result7[] = "52000.00";
  char* output7 = (char*)calloc(256, sizeof(char));
  snprintf(output7, max_length, "%.2lf", output.total_draw_amount);
  ck_assert_str_eq(output7, result7);
  free(output7);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("s21_smartcalc");
    TCase *tc1_1 = tcase_create("s21_smartcalc");
    TCase *tc1_2 = tcase_create("s21_creditcalc");
    TCase *tc1_3 = tcase_create("s21_depositcalc");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, s21_smartcalc_test_1);
    tcase_add_test(tc1_1, s21_smartcalc_test_2);
    tcase_add_test(tc1_1, s21_smartcalc_test_3);
    tcase_add_test(tc1_1, s21_smartcalc_test_4);
    tcase_add_test(tc1_1, s21_smartcalc_test_5);
    tcase_add_test(tc1_1, s21_smartcalc_test_6);
    tcase_add_test(tc1_1, s21_smartcalc_test_7);
    tcase_add_test(tc1_1, s21_smartcalc_test_8);
    tcase_add_test(tc1_1, s21_smartcalc_test_9);
    tcase_add_test(tc1_1, s21_smartcalc_test_10);
    tcase_add_test(tc1_1, s21_smartcalc_test_11);
    tcase_add_test(tc1_1, s21_smartcalc_test_12);
    tcase_add_test(tc1_1, s21_smartcalc_test_13);
    tcase_add_test(tc1_1, s21_smartcalc_test_14);
    tcase_add_test(tc1_1, s21_smartcalc_test_15);
    tcase_add_test(tc1_1, s21_smartcalc_test_16);
    tcase_add_test(tc1_1, s21_smartcalc_test_17);
    tcase_add_test(tc1_1, s21_smartcalc_test_18);
    suite_add_tcase(s1, tc1_2);
    tcase_add_test(tc1_2, s21_creditcalc_test_1);
    tcase_add_test(tc1_2, s21_creditcalc_test_2);
    suite_add_tcase(s1, tc1_3);
    tcase_add_test(tc1_3, s21_depositcalc_test_1);
    tcase_add_test(tc1_3, s21_depositcalc_test_2);
    tcase_add_test(tc1_3, s21_depositcalc_test_3);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
