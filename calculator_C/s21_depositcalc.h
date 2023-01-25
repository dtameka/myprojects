#ifndef S21_DEPOSITCALC_H_
#define S21_DEPOSITCALC_H_

#define EPS 1E-7

typedef enum {NOT, DAILY, WEEKLY, MONTHLY, BIMONTHLY, TRIMESTERLY, QUADRIMESTERLY,
              SEMESTERLY, YEARLY} investment_schema_t;


typedef struct deposit_input_t deposit_input_t;
struct deposit_input_t {
    double deposit_amount;
    int term;
    double interest_rate;
    double income_tax_rate;
    double income_tax_ceiling;
    double refinance_rate;
    investment_schema_t reinvest_schema;
    investment_schema_t topup_schema;
    double topup_amount;
    investment_schema_t withdraw_schema;
    double draw_amount;
};

typedef struct deposit_output_t deposit_output_t;
struct deposit_output_t {
    double final_interest_amount;
    double final_deposit_amount;
    double net_amount;
    double tax_payment;
    double total_topup_amount;
    double total_draw_amount;
    double total_compound_amount;
    int negate_check;
};

int investment_into_days(investment_schema_t schema);
void initialize_struct_input_deposit(deposit_input_t* deposit_input);
void initialize_struct_output_deposit(deposit_output_t* deposit_output);
deposit_output_t s21_deposit_calc(deposit_input_t* input);

#endif  // S21_DEPOSITCALC_H_
