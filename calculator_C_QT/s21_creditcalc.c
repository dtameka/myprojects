#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "s21_creditcalc.h"

/*
int main() {
    printf("---Loan------------------------------------------\n");
    credit_input_t input;
    credit_output_t result;
    input.amount = 100000.0;
    input.term = 12;
    input.interest_rate = 10.0;
    input.credit_type = DECLINING;
    result = credit_calculator(&input);

    printf("first_payment : %.2lf\n", result.first_payment);
    printf("last_payment : %.2lf\n", result.last_payment);
    printf("over_payment : %.2lf\n", result.overpayment_amount);
    printf("total_payment : %.2lf\n", result.total_amount);
} 
*/

void initialize_struct_input(credit_input_t* input) {
    input->interest_rate = 0.0;
    input->amount = 0.0;
    input->term = 0;
    input->credit_type = NONE;
}

void initialize_struct_output(credit_output_t* output) {
    output->overpayment_amount = 0.0;
    output->total_amount = 0.0;
    output->monthly_payment = 0.0;
    output->first_payment = 0.0;
    output->last_payment = 0.0;
    output->error_string = '\0';
}

int is_input_true(credit_input_t* input) {
  int result = 1;
    if (input->amount >= 1 && input->credit_type != NONE
        && input->interest_rate >= EPS && input->term >= 1)
        result = 0;
  return result;
}

credit_output_t s21_credit_calculate(credit_input_t* input) {
    credit_output_t output;
    initialize_struct_output(&output);
    if (is_input_true(input) == 0) {
      double interest_rate_m = input->interest_rate / (100.0*12.0);
        if (input->credit_type == ANNUITY) {
            output.monthly_payment = ceil((input->amount * (interest_rate_m + (interest_rate_m /
                                          ((pow(1.0 + interest_rate_m, input->term) - 1.0))))));
            output.total_amount = output.monthly_payment * input->term;
            output.overpayment_amount = output.total_amount - input->amount;
        } else if (input->credit_type == DECLINING) {
              double monthly_payment =  input->amount / input->term;
              double monthly_interest[200] = {0};
              double monthly_capital[200] = {0};
              double temp_input = input->amount;
              for (int i = 0; i < (int)input->term; i++) {
                  monthly_interest[i] = temp_input * interest_rate_m;
                  monthly_capital[i] = monthly_payment;
                  temp_input = temp_input - monthly_capital[i];
                  output.total_amount = output.total_amount + monthly_capital[i] + monthly_interest[i];
                  if (i == 0) {
                      output.first_payment = output.total_amount;
                  } else {
                      output.last_payment = 0.0;
                      output.last_payment = monthly_capital[i] + monthly_interest[i];
                  }
              }
          output.overpayment_amount = output.total_amount - input->amount;
        }
    } else {
      static char* error_string = "Error - Please check if the input is correct.";
        strncpy(output.error_string, error_string, 512);
    }
    return output;
}
