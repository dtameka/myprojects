#ifndef S21_CREDITCALC_H_
#define S21_CREDITCALC_H_

#define EPS 1E-7

typedef enum {NONE, ANNUITY, DECLINING} credit_type_t;

typedef struct credit_input_t credit_input_t;
struct credit_input_t {
    double amount;
    int term;
    double interest_rate;
    credit_type_t credit_type;
};

typedef struct credit_output_t credit_output_t;
struct credit_output_t {
    double monthly_payment;
    double overpayment_amount;
    double total_amount;
    double first_payment;
    double last_payment;
    char* error_string;
};

void initialize_struct_input(credit_input_t* input);
void initialize_struct_output(credit_output_t* output);
int is_input_true(credit_input_t* input);
credit_output_t s21_credit_calculate(credit_input_t* input);

#endif  // S21_CREDITCALC_H_
