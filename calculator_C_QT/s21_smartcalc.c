#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "s21_smartcalc.h"

static char NUMBERS[] = "1234567890.X";
static char OPERANDS[] = "+-*/%^&_#~";
static char BRACKETS[] = "()";
static char FUNCTIONS[] = "ATCSGOQILRN";

/*

void main() {
  char* string = "1+SQRT(100)";
  char* output = s21_smartcalc(string, 0);
  printf("%s\n", output);

}

*/

void number_stack_push(number_t** number_stack, double number) {
    number_t* temp = (number_t*)calloc(1, sizeof(number_t));
    if (temp != NULL) {
        temp->number = number;
        temp->next = *number_stack;
        *number_stack = temp;
    }
}

double number_stack_pop(number_t** number_stack) {
    double result = 0;
    if (number_stack != NULL) {
        number_t* temp = *number_stack;
        result = temp->number;
        *number_stack = temp->next;
        free(temp);
        temp = NULL;
    }
    return result;
}

void operator_stack_push(operator_t** operator_stack, char operator) {
    operator_t* temp = (operator_t*)calloc(1, sizeof(operator_t));
    if (temp != NULL) {
        temp->operand = operator;
        temp->next = *operator_stack;
        *operator_stack = temp;
    }
}

char operator_stack_pop(operator_t** operator_stack) {
    double result = 0;
    if (operator_stack != NULL) {
        operator_t* temp = *operator_stack;
        result = temp->operand;
        *operator_stack = temp->next;
        free(temp);
        temp = NULL;
    }
    return result;
}

char operator_stack_peek(operator_t* operator_stack) {
    return operator_stack->operand;
}

double number_stack_peek(number_t* number_stack) {
    return number_stack->number;
}

int is_operator_stack_empty(operator_t* operator_stack) {
    int result = 0;
    if (operator_stack != NULL)
        result = 1;
    return result;
}

char* convert_infix_to_postfix(char* input_string) {
    char* output_string = (char*)calloc(512, sizeof(char));
    if (input_string != NULL) {
        convert_unary(input_string);
        char* offset_pointer = output_string;  // point to output string
        char* token = NULL;
        if (is_normal_input(input_string) == 0) {
            operator_t* operator_stack = NULL;
            while (*input_string != '\0') {
                int match = strspn(input_string, NUMBERS);
                if (match != 0) {
                    strncpy(offset_pointer, input_string, match);
                    input_string = input_string + match;
                    offset_pointer = offset_pointer + match;
                    *offset_pointer++ = '|';
                } else {
                    match = strspn(input_string, OPERANDS);
                    if (match != 0) {
                        token = (char*)calloc(match+1, sizeof(char));
                        strncpy(token, input_string, 1);
                        input_string++;
                    } else {
                        match = strspn(input_string, FUNCTIONS);
                        if (match != 0) {
                            token = (char*)calloc(match+1, sizeof(char));
                            strncpy(token, input_string, match);
                            input_string += match;
                        } else if (*input_string == '(' || *input_string == ')') {
                        token = (char*)calloc(2, sizeof(char));
                        strncpy(token, input_string, 1);
                        input_string += 1;
                        }
                    }
                    char operator_token = tokenize_operator(token);
                    if (operator_token != '[' && operator_token != ']') {
                        if (is_operator_stack_empty(operator_stack) == 0) {
                            operator_stack_push(&operator_stack, operator_token);
                        } else {
                            if (operator_priority(operator_token) >
                                operator_priority(operator_stack_peek(operator_stack))) {
                                operator_stack_push(&operator_stack, operator_token);
                            } else {
                                while (is_operator_stack_empty(operator_stack) != 0 &&
                                        operator_priority(operator_token) <=
                                        operator_priority(operator_stack_peek(operator_stack))) {
                                    *offset_pointer = operator_stack_pop(&operator_stack);
                                    offset_pointer++;
                                    *offset_pointer++ = '|';
                                }
                                operator_stack_push(&operator_stack, operator_token);
                            }
                        }
                    } else {
                        if (operator_token == '[') {
                        operator_stack_push(&operator_stack, operator_token);
                        } else if (operator_token == ']') {
                            while (is_operator_stack_empty(operator_stack) != 0 &&
                                    operator_stack_peek(operator_stack) != '[') {
                                *offset_pointer = operator_stack_pop(&operator_stack);
                                offset_pointer++;
                                *offset_pointer++ = '|';
                            }
                            if (is_operator_stack_empty(operator_stack) != 0 &&
                                operator_stack_peek(operator_stack) == '[') {
                                operator_stack_pop(&operator_stack);
                            }
                        }
                    }
                free(token);
                token = NULL;
                match = 0;
                }
            }
            while (is_operator_stack_empty(operator_stack) != 0) {
                *offset_pointer = operator_stack_pop(&operator_stack);
                if (*offset_pointer != '[' || *offset_pointer != ']') {
                    offset_pointer++;
                    *offset_pointer++ = '|';
                }
            }
        } else {
            static char* error_string = "Error syntax";
            strncpy(output_string, error_string, 512);
        }
    } else {
        static char* error_string = "Error null";
        strncpy(output_string, error_string, 512);
    }
    return output_string;
}

void convert_unary(char* string) {
    if (string[0] == '+') string[0] = '&';
    if (string[0] == '-') string[0] = '_';
    if ((string[0] == '&' || string[0] == '_') && string[1] == '+' && strlen(string) > 2) string[1] = '#';
    if ((string[0] == '&' || string[0] == '_') && string[1] == '-' && strlen(string) > 2) string[1] = '~';
    for (int i = 0; i < (int)strlen(string); i++) {
        if ((string[i] == 't' || string[i] == 'd' || string[i] == 'g' ||
             string[i] == 'n' || string[i] == 's' || string[i] == '+' ||
             string[i] == '-' || string[i] == '*' || string[i] == '/' ||
             string[i] == '^' || string[i] == '&' || string[i] == '~') && string[i+1] == '+') {
                string[i+1] = '#';
            }
        if ((string[i] == '(') && string[i+1] == '+') string[i+1] = '&';
        if ((string[i] == 't' || string[i] == 'd' || string[i] == 'g' ||
             string[i] == 'n' || string[i] == 's' || string[i] == '+' ||
             string[i] == '-' || string[i] == '*' || string[i] == '/' ||
             string[i] == '^' || string[i] == '&' || string[i] == '~') && string[i+1] == '-') {
                string[i+1] = '~';
            }
        if ((string[i] == '(') && string[i+1] == '-') string[i+1] = '_';
    }
}

int is_normal_input(char* string) {
    int result = -1;
    int number_counter = 0;
    int operand_counter = 0;
    int function_counter = 0;
    int left_bracket_counter = 0;
    int right_bracket_counter = 0;

    while (*string != '\0') {
        int match = strspn(string, NUMBERS);
        if (match) {
            number_counter++;
            string += match;
        } else {
            match = strspn(string, OPERANDS);
            if (match) {
                operand_counter++;
                string += match;
            } else {
                match = strspn(string, FUNCTIONS);
                if (match) {
                    function_counter++;
                    string += match;
                } else {
                    match = strspn(string, BRACKETS);
                    if (match) {
                        if (match >= 1) {
                            if (string[0] == '(') {
                                left_bracket_counter++;
                                string++;
                            }
                            if (string[0] == ')') {
                                right_bracket_counter++;
                                string++;
                            }
                        }
                    } else {
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    if (result != 1) {
            if (number_counter == 0)
                result = 2;
            else if (left_bracket_counter != right_bracket_counter)
                result = 3;
            else if (strlen(string) > 255)
                result = 4;
            else
                result = 0;
        }
    return result;
}

char tokenize_operator(char *token) {
    char result = '\0';
    if (token != NULL) {
        int i = 0;
        while ( (int)strlen(token) > i ) {
            switch (*token) {
                case 'C':
                    result = 'c';               // COS
                    break;
                case 'S':
                    token++;
                    if (*token == 'I') {
                        result = 's';           // SIN
                    } else if (*token == 'Q') {
                        result = 'q';           // SQRT
                    }
                    break;
                case 'T':
                    result = 't';               // TAN
                    break;
                case 'L':
                    token++;
                    if (*token == 'N') {
                        result = 'L';           // LN
                    } else if (*token == 'O') {
                        result = 'l';           // LOG
                    }
                    break;
                case 'A':
                    token++;
                    if (*token == 'C') {
                        result = 'C';           // ACOS
                    } else if (*token == 'S') {
                        result = 'S';           // ASIN
                    } else if (*token == 'T') {
                        result = 'T';           // ATAN
                    }
                    break;
                case '^':
                    result = '^';               // POW
                    break;
                case '%':
                    result = '%';               // MOD
                    break;
                case '+':
                    result = '+';
                    break;
                case '-':
                    result = '-';
                    break;
                case '*':
                    result = '*';
                    break;
                case '/':
                    result = '/';
                    break;
                case '&':
                    result = '&';   // UNARY +
                    break;
                case '#':
                    result = '#';
                    break;
                case '~':
                    result = '~';
                    break;
                case '_':
                    result = '_';
                    break;
                case '(':
                    result = '[';
                    break;
                case ')':
                    result = ']';
                    break;
            }
        token++;
        i++;
        }
    }
    return result;
}

int operator_priority(char token) {
    int result = 0;
    if (token == '#' || token == '~') {
        result = 5;
    } else if (token == 'c' || token == 's' || token == 't' || token == 'C' || token == 'S' ||
               token == 'T' || token == 'q' || token == 'l' || token == 'L') {
        result = 4;
    } else if (token == '^' || token == '&' || token == '_') {
        result = 3;
    } else if (token == '*' || token == '/' || token == '%') {
        result = 2;
    } else if (token == '+' || token == '-') {
        result = 1;
    } else if (token == '[' || token == ']') {
        result = 0;
    }
    return result;
}


char* s21_smartcalc(char* input_string, double x) {
    int max_length = 256;
    char* result = (char*)calloc(256, sizeof(char));
    int calculation_error = 0;
    double calculated = 0;
    char* rpn_string = convert_infix_to_postfix(input_string);
    if (*rpn_string != 'E') {
        calculated = postfix_caclculator(rpn_string, x, &calculation_error);
    }
    if (*rpn_string != 'E' && calculation_error == 0) {
        snprintf(result, max_length, "%.7lf", calculated);
    } else {
        char* output_string = "Error - rpn";
        strncpy(result, output_string, 256);
    }
    free(rpn_string);
    return result;
}


double postfix_caclculator(char* rpn_string, double x, int* errors) {
    number_t* number_stack = NULL;
    char* nstr;
    double result = 0;
    while (rpn_string != NULL && *rpn_string != '\0') {
        if (*rpn_string == '|') {
            rpn_string++;
        }
        int change = strspn(rpn_string, NUMBERS);
        if (change != 0 && *rpn_string != '\0') {
            if (*rpn_string == 'X') {
                number_stack_push(&number_stack, x);
            } else {
                number_stack_push(&number_stack, strtod(rpn_string, &nstr));
            }
            rpn_string += change;
        } else if (*rpn_string != '\0') {
            number_stack_push(&number_stack, count_numbers(rpn_string, &number_stack, &errors));
            rpn_string++;
        }
    }
    result = number_stack_pop(&number_stack);
    return result;
}

double count_numbers(char* operand, number_t** number_stack, int** errors) {
    double result = 0;
    double left_number = 0;
    double right_number = 0;
    switch (*operand) {
        case '+': {
            right_number = process_stack(number_stack, &errors, operand);
            left_number = process_stack(number_stack, &errors, operand);
            result = left_number + right_number;
            break;
        } case '-': {
            right_number = process_stack(number_stack, &errors, operand);
            left_number = process_stack(number_stack, &errors, operand);
            result = left_number - right_number;
            break;
        } case '*': {
            right_number = process_stack(number_stack, &errors, operand);
            left_number = process_stack(number_stack, &errors, operand);
            result = left_number * right_number;
            break;
        } case '/': {
            right_number = process_stack(number_stack, &errors, operand);
            left_number = process_stack(number_stack, &errors, operand);
            result = left_number / right_number;
            break;
        } case '%': {
            right_number = process_stack(number_stack, &errors, operand);
            left_number = process_stack(number_stack, &errors, operand);
            result = fmod(left_number, right_number);
            break;
        } case '^': {
            right_number = process_stack(number_stack, &errors, operand);
            left_number = process_stack(number_stack, &errors, operand);
            result = pow(left_number, right_number);
            break;
        } case 'q': {
            right_number = process_stack(number_stack, &errors, operand);
            result = sqrt(right_number);
            break;
        } case '#': case '&': {
            right_number = process_stack(number_stack, &errors, operand);
            result = right_number;
            break;
        } case '~': case '_': {
            right_number = process_stack(number_stack, &errors, operand);
            result = right_number * (-1.0);
            break;
        } case 'c': {
            right_number = process_stack(number_stack, &errors, operand);
            result = cos(right_number);
            break;
        } case 's': {
            right_number = process_stack(number_stack, &errors, operand);
            result = sin(right_number);
            break;
        } case 't': {
            right_number = process_stack(number_stack, &errors, operand);
            result = tan(right_number);
            break;
        } case 'T': {
            right_number = process_stack(number_stack, &errors, operand);
            result = atan(right_number);
            break;
        } case 'C': {
            right_number = process_stack(number_stack, &errors, operand);
            result = acos(right_number);
            break;
        } case 'S': {
            right_number = process_stack(number_stack, &errors, operand);
            result = asin(right_number);
            break;
        } case 'l': {
            right_number = process_stack(number_stack, &errors, operand);
            result = log10(right_number);
            break;
        } case 'L': {
            right_number = process_stack(number_stack, &errors, operand);
            result = log(right_number);
            break;
        } default: {
            break;
        }
    }
    return result;
}

double process_stack(number_t** stack, int*** errors, char* rpn_string) {
    double result = 0;
        if (*stack == NULL && strlen(rpn_string) != 0) {
            ***errors = 2;
        } else {
            result = number_stack_pop(stack);
        }
    return result;
}
