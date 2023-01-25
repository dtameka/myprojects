#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "s21_depositcalc.h"

void initialize_struct_input_deposit(deposit_input_t* deposit_input) {
    deposit_input->deposit_amount = 0.0;
    deposit_input->term = 0;
    deposit_input->interest_rate = 0.0;
    deposit_input->income_tax_rate = 0.0;
    deposit_input->income_tax_ceiling = 0.0;
    deposit_input->refinance_rate = 0.0;
    deposit_input->reinvest_schema = NOT;
    deposit_input->topup_schema = NOT;
    deposit_input->topup_amount = 0.0;
    deposit_input->withdraw_schema = NOT;
    deposit_input->draw_amount = 0.0;
}

void initialize_struct_output_deposit(deposit_output_t* deposit_output) {
    deposit_output->final_interest_amount = 0.0;
    deposit_output->final_deposit_amount = 0.0;
    deposit_output->tax_payment = 0.0;
    deposit_output->net_amount = 0.0;
    deposit_output->total_topup_amount = 0.0;
    deposit_output->total_draw_amount = 0.0;
    deposit_output->negate_check = 0;
    deposit_output->total_compound_amount = 0.0;
}


int investment_into_days(investment_schema_t schema) {
    int result = -1;
    if (schema == DAILY) {
        result = 1;
    } else if (schema == WEEKLY) {
        result = (int)(365/52);
    } else if (schema == MONTHLY) {
        result = (int)(365/12);
    } else if (schema == BIMONTHLY) {
        result = (int)(365/6);
    } else if (schema == TRIMESTERLY) {
        result = (int)(365/4);
    } else if (schema == QUADRIMESTERLY) {
        result = (int)(365/3);
    } else if (schema == SEMESTERLY) {
        result = (int)(365/2);
    } else if (schema == YEARLY) {
        result = 365;
    }
    return result;
}

deposit_output_t s21_deposit_calc(deposit_input_t* input) {
  deposit_output_t output;
  initialize_struct_output_deposit(&output);
  double deposit_balance[10000] = {0};
  int term_day = (int)((double)(input->term * 30.417));
  double temp_deposit_amount = input->deposit_amount;
    if (input->topup_schema != NOT) {
        for (int i = 0; i < term_day; i++) {
        int topup_step = investment_into_days(input->topup_schema);
            if ((i+1) % topup_step == 0 && topup_step != -1) {
                temp_deposit_amount = temp_deposit_amount + input->topup_amount;
                output.total_topup_amount = output.total_topup_amount + input->topup_amount;
            }
        deposit_balance[i] = temp_deposit_amount;
        }
    } else {
        for (int i = 0; i < term_day; i++) {
            deposit_balance[i] = temp_deposit_amount;
        }
    }
    if (input->withdraw_schema != NOT) {
        for (int i = 0; i < term_day; i++) {
        int draw_step = investment_into_days(input->withdraw_schema);
            if ((i+1) % draw_step == 0 && draw_step != -1) {
                temp_deposit_amount = temp_deposit_amount - input->draw_amount;
                output.total_draw_amount = output.total_draw_amount + input->draw_amount;
            }
            deposit_balance[i] = temp_deposit_amount;
        }
    } else {
        for (int i = 0; i < term_day; i++) {
            deposit_balance[i] = temp_deposit_amount;
        }
    }
    double interest_rate_d = input->interest_rate / (100.0 * 365.0);
    if (input->reinvest_schema != NOT) {
        double temp_compund_interest = 0.0;
        for (int i = 0; i < term_day; i++) {
        int compounding_step = investment_into_days(input->reinvest_schema);
            temp_compund_interest = temp_compund_interest + temp_deposit_amount * interest_rate_d;
            if ((i+1) % compounding_step == 0 && compounding_step != -1) {
                temp_deposit_amount = temp_deposit_amount + temp_compund_interest;
                output.total_compound_amount = output.total_compound_amount + temp_compund_interest;
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
        output.final_interest_amount = output.final_interest_amount + deposit_balance[i] * interest_rate_d;
        output.final_deposit_amount = deposit_balance[i];
    }
//  Tax calculate
    double free_deposit_amount = input->income_tax_ceiling * input->refinance_rate;
    if (free_deposit_amount > 0 && output.final_interest_amount > free_deposit_amount) {
        output.tax_payment = (output.final_interest_amount - free_deposit_amount) *
                              input->income_tax_rate / 100;
    } else {
        output.tax_payment = 0.0;
    }
    output.net_amount = output.final_interest_amount - output.tax_payment;
// Negate check
    if (output.final_deposit_amount > EPS && output.final_interest_amount > EPS)
        output.negate_check = 0;
    else
        output.negate_check = 1;

    return output;
}

/*
int main() {
    deposit_input_t input;
    initialize_struct_input(&input);
    deposit_output_t output;
    input.deposit_amount = 100000.0;
    input.term = 12;
    input.interest_rate = 10.0;
    input.income_tax_rate = 10.0;
    input.refinance_rate = 4.25;
    input.income_tax_ceiling = 10000.0;
    input.reinvest_schema = WEEKLY;
    input.topup_schema = NONE;
    input.topup_amount = 1000.0;
    input.draw_schema = NONE;
    input.draw_amount = 1000.0;
    output = s21_deposit_calc(&input);
    printf("total_topup_amount : %.2lf\n", output.total_topup_amount);
    printf("total_draw_amount : %.2lf\n", output.total_draw_amount);
    printf("final_interest_amount : %.2lf\n", output.final_interest_amount);
    printf("final_deposit_amount : %.2lf\n", output.final_deposit_amount);
    printf("tax_payment : %.2lf\n", output.tax_payment);
}
*/
