#ifndef S21_SMARTCALC_H_
#define S21_SMARTCALC_H_

typedef struct number_t number_t;
struct number_t {
    double number;
    number_t* next;
};

typedef struct operator_t operator_t;
struct operator_t {
    char operand;
    operator_t* next;
};

void number_stack_push(number_t** number_stack, double number);
double number_stack_pop(number_t** number_stack);
void operator_stack_push(operator_t** operator_stack, char selector);
char operator_stack_pop(operator_t** operator_stack);
char operator_stack_peek(operator_t* operator_stack);
double number_stack_peek(number_t* number_stack);
int is_operator_stack_empty(operator_t* operator_stack);
char* convert_infix_to_postfix(char* input_string);
void convert_unary(char* string);
int is_normal_input(char* string);
char tokenize_operator(char *token);
int operator_priority(char token);
char* s21_smartcalc(char* input_string, double x);
double process_stack(number_t** stack, int*** errors, char* rpn_string);
double count_numbers(char* operand, number_t** number_stack, int** errors);
double postfix_caclculator(char* rpn_string, double x, int* errors);

#endif  // S21_SMARTCALC_H_
