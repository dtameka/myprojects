#include "s21_smartcalc.h"

static char NUMBERS[] = "1234567890.XE";
static char OPERANDS[] = "+-*/%^&_#~";
static char BRACKETS[] = "()";
static char FUNCTIONS[] = "ATCSGOQILRN";

void s21::SmartCalculator::convert_unary(std::string& string) {
  if (string[0] == '+') string[0] = '&';
  if (string[0] == '-') string[0] = '_';
  if ((string[0] == '&' || string[0] == '_') && string[1] == '+' &&
      string.size() > 2)
    string[1] = '#';
  if ((string[0] == '&' || string[0] == '_') && string[1] == '-' &&
      string.size() > 2)
    string[1] = '~';
  for (size_t i = 0; i < string.size(); i++) {
    if ((string[i] == 't' || string[i] == 'd' || string[i] == 'g' ||
         string[i] == 'n' || string[i] == 's' || string[i] == '+' ||
         string[i] == '-' || string[i] == '*' || string[i] == '/' ||
         string[i] == '^' || string[i] == '&' || string[i] == '~') &&
        string[i + 1] == '+') {
      string[i + 1] = '#';
    }
    if ((string[i] == '(') && string[i + 1] == '+') string[i + 1] = '&';
    if ((string[i] == 't' || string[i] == 'd' || string[i] == 'g' ||
         string[i] == 'n' || string[i] == 's' || string[i] == '+' ||
         string[i] == '-' || string[i] == '*' || string[i] == '/' ||
         string[i] == '^' || string[i] == '&' || string[i] == '~') &&
        string[i + 1] == '-') {
      string[i + 1] = '~';
    }
    if ((string[i] == '(') && string[i + 1] == '-') string[i + 1] = '_';
  }
}

int s21::SmartCalculator::is_normal_input(std::string& string) {
  int result = -1;
  int number_counter = 0;
  int operand_counter = 0;
  int function_counter = 0;
  int left_bracket_counter = 0;
  int right_bracket_counter = 0;
  auto pointer_on_string = &string[0];

  while (*pointer_on_string != '\0') {
    int match = std::strspn(pointer_on_string, NUMBERS);
    if (match) {
      number_counter++;
      pointer_on_string += match;
    } else {
      match = strspn(pointer_on_string, OPERANDS);
      if (match) {
        operand_counter++;
        pointer_on_string += match;
      } else {
        match = strspn(pointer_on_string, FUNCTIONS);
        if (match) {
          function_counter++;
          pointer_on_string += match;
        } else {
          match = strspn(pointer_on_string, BRACKETS);
          if (match) {
            if (match >= 1) {
              if (*pointer_on_string == '(') {
                left_bracket_counter++;
                pointer_on_string++;
              }
              if (*pointer_on_string == ')') {
                right_bracket_counter++;
                pointer_on_string++;
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
    else if (string.size() > 255)
      result = 4;
    else
      result = 0;
  }
  return result;
}

char s21::SmartCalculator::tokenize_operator(std::string& token) {
  char result = '\0';
  if (!(token.empty())) {
    auto iterator = token.begin();
    size_t i = 0;
    while (2 > i) {  // 2 because we check the first 2 letters
      switch (*iterator) {
        case 'C':
          result = 'c';  // COS
          break;
        case 'S':
          iterator++;
          if (*iterator == 'I') {
            result = 's';  // SIN
          } else if (*iterator == 'Q') {
            result = 'q';  // SQRT
            token.clear();
          }
          break;
        case 'T':
          result = 't';  // TAN
          break;
        case 'L':
          iterator++;
          if (*iterator == 'N') {
            result = 'L';  // LN
          } else if (*iterator == 'O') {
            result = 'l';  // LOG
          }
          break;
        case 'A':
          iterator++;
          if (*iterator == 'C') {
            result = 'C';  // ACOS
          } else if (*iterator == 'S') {
            result = 'S';  // ASIN
          } else if (*iterator == 'T') {
            result = 'T';  // ATAN
          }
          break;
        case '^':
          result = '^';  // POW
          break;
        case '%':
          result = '%';  // MOD
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
          result = '&';  // UNARY +
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
      iterator++;
      i++;
    }
  }
  return result;
}

int s21::SmartCalculator::operator_priority(char token) {
  int result = 0;
  if (token == '#' || token == '~') {
    result = 5;
  } else if (token == 'c' || token == 's' || token == 't' || token == 'C' ||
             token == 'S' || token == 'T' || token == 'q' || token == 'l' ||
             token == 'L' || token == 'e') {
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

std::string s21::SmartCalculator::convert_infix_to_postfix(
    std::string& input_string) {
  std::vector<char> output_string(512);
  if (!(input_string.empty())) {
    convert_unary(input_string);

    char* offset_pointer = output_string.data();  // pointer to output string
    std::string token;
    char* pointer_on_input = &input_string[0];
    if (is_normal_input(input_string) == 0) {
      std::stack<char> operator_stack{};

      while (*pointer_on_input != '\0') {
        int match = strspn(pointer_on_input, NUMBERS);
        if (match != 0) {
          std::strncpy(offset_pointer, pointer_on_input, match);
          pointer_on_input += match;
          offset_pointer += match;
          *offset_pointer++ = '|';
        } else {
          match = strspn(pointer_on_input, OPERANDS);
          if (match != 0) {
            token.reserve(match + 1);
            token.push_back(*pointer_on_input);
            pointer_on_input++;
          } else {
            match = strspn(pointer_on_input, FUNCTIONS);
            if (match != 0) {
              token.reserve(match + 1);
              token.append(pointer_on_input, match);
              pointer_on_input += match;
            } else if (*pointer_on_input == '(' || *pointer_on_input == ')') {
              token.reserve(2);
              token.push_back(*pointer_on_input);
              pointer_on_input += 1;
            }
          }
          char operator_token = tokenize_operator(token);
          if (operator_token != '[' && operator_token != ']') {
            if (operator_stack.empty()) {
              operator_stack.push(operator_token);
            } else {
              if (operator_priority(operator_token) >
                  operator_priority(operator_stack.top())) {
                operator_stack.push(operator_token);
              } else {
                while (!(operator_stack.empty()) &&
                       operator_priority(operator_token) <=
                           operator_priority(operator_stack.top())) {
                  *offset_pointer = operator_stack.top();
                  operator_stack.pop();
                  offset_pointer++;
                  *offset_pointer++ = '|';
                }
                operator_stack.push(operator_token);
              }
            }
          } else {
            if (operator_token == '[') {
              operator_stack.push(operator_token);
            } else if (operator_token == ']') {
              while (!(operator_stack.empty()) && operator_stack.top() != '[') {
                *offset_pointer = operator_stack.top();
                operator_stack.pop();
                offset_pointer++;
                *offset_pointer++ = '|';
              }
              if (!(operator_stack.empty()) && operator_stack.top() == '[') {
                operator_stack.pop();
              }
            }
          }
          token.clear();
          token.shrink_to_fit();
          match = 0;
        }
      }
      while (!(operator_stack.empty())) {
        *offset_pointer = operator_stack.top();
        operator_stack.pop();
        if (*offset_pointer != '[' || *offset_pointer != ']') {
          offset_pointer++;
          *offset_pointer++ = '|';
        }
      }
    } else {
      *output_string.begin() = 'E';
    }
    offset_pointer = nullptr;
    pointer_on_input = nullptr;
  } else {
    *output_string.begin() = 'E';
  }
  std::string str(output_string.begin(), output_string.end());

  return str;
}

double s21::SmartCalculator::process_stack(std::stack<double>& stack,
                                           int& errors, char* operand) {
  double result = 0;
  if (stack.empty() && strlen(operand) != 0) {
    errors = 2;
  } else {
    result = stack.top();
    stack.pop();
  }
  return result;
}

double s21::SmartCalculator::count_numbers(char* operand,
                                           std::stack<double>& number_stack,
                                           int& errors) {
  double result = 0;
  double left_number = 0;
  double right_number = 0;
  switch (*operand) {
    case '+': {
      right_number = process_stack(number_stack, errors, operand);
      left_number = process_stack(number_stack, errors, operand);
      result = left_number + right_number;
      break;
    }
    case '-': {
      right_number = process_stack(number_stack, errors, operand);
      left_number = process_stack(number_stack, errors, operand);
      result = left_number - right_number;
      break;
    }
    case '*': {
      right_number = process_stack(number_stack, errors, operand);
      left_number = process_stack(number_stack, errors, operand);
      result = left_number * right_number;
      break;
    }
    case '/': {
      right_number = process_stack(number_stack, errors, operand);
      left_number = process_stack(number_stack, errors, operand);
      result = left_number / right_number;
      break;
    }
    case '%': {
      right_number = process_stack(number_stack, errors, operand);
      left_number = process_stack(number_stack, errors, operand);
      result = fmod(left_number, right_number);
      break;
    }
    case '^': {
      right_number = process_stack(number_stack, errors, operand);
      left_number = process_stack(number_stack, errors, operand);
      result = pow(left_number, right_number);
      break;
    }
    case 'q': {
      right_number = process_stack(number_stack, errors, operand);
      result = sqrt(right_number);
      break;
    }
    case '#':
    case '&': {
      right_number = process_stack(number_stack, errors, operand);
      result = right_number;
      break;
    }
    case '~':
    case '_': {
      right_number = process_stack(number_stack, errors, operand);
      result = right_number * (-1.0);
      break;
    }
    case 'c': {
      right_number = process_stack(number_stack, errors, operand);
      result = cos(right_number);
      break;
    }
    case 's': {
      right_number = process_stack(number_stack, errors, operand);
      result = sin(right_number);
      break;
    }
    case 't': {
      right_number = process_stack(number_stack, errors, operand);
      result = tan(right_number);
      break;
    }
    case 'T': {
      right_number = process_stack(number_stack, errors, operand);
      result = atan(right_number);
      break;
    }
    case 'C': {
      right_number = process_stack(number_stack, errors, operand);
      result = acos(right_number);
      break;
    }
    case 'S': {
      right_number = process_stack(number_stack, errors, operand);
      result = asin(right_number);
      break;
    }
    case 'l': {
      right_number = process_stack(number_stack, errors, operand);
      result = log10(right_number);
      break;
    }
    case 'L': {
      right_number = process_stack(number_stack, errors, operand);
      result = log(right_number);
      break;
    }
    case 'e': {
      right_number = process_stack(number_stack, errors, operand);
      result = exp(right_number);
    }
    default: {
      break;
    }
  }
  return result;
}

double s21::SmartCalculator::postfix_calculator(std::string& rpn_string,
                                                double& x, int& errors) {
  std::stack<double> number_stack;
  char* nstr = nullptr;
  double result = 0.0;
  auto pointer_on_string = &rpn_string[0];
  while (pointer_on_string != nullptr && *pointer_on_string != '\0') {
    if (*pointer_on_string == '|') {
      pointer_on_string++;
    }
    int change = strspn(pointer_on_string, NUMBERS);
    if (change != 0 && *pointer_on_string != '\0') {
      if (*pointer_on_string == 'X') {
        number_stack.push(x);
      } else {
        number_stack.push(strtod(pointer_on_string, &nstr));
      }
      pointer_on_string += change;
    } else if (*pointer_on_string != '\0') {
      number_stack.push(count_numbers(pointer_on_string, number_stack, errors));
      pointer_on_string++;
    }
  }
  result = number_stack.top();
  number_stack.pop();
  pointer_on_string = nullptr;
  return result;
}

void s21::SmartCalculator::s21_smartcalc(std::string& input_string, double x) {
  int calculation_error = 0;
  double calculated = 0;
  std::string rpn_string = convert_infix_to_postfix(input_string);
  if (rpn_string[0] != 'E') {
    calculated = postfix_calculator(rpn_string, x, calculation_error);
  }
  if (rpn_string[0] != 'E' && calculation_error == 0) {
    int max_length = 256;
    char convert_result[max_length];
    snprintf(convert_result, max_length, "%.7lf", calculated);
    result_ = convert_result;
  } else {
    result_ = "Error!";
  }
}

std::string s21::SmartCalculator::get_data() { return result_; }
