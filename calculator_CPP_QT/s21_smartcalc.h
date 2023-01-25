#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

namespace s21 {

class SmartCalculator {
 public:
  SmartCalculator() : result_("0") {}
  ~SmartCalculator() {}

  void s21_smartcalc(std::string &input_string, double x);
  std::string convert_infix_to_postfix(std::string &input_string);
  double postfix_calculator(std::string &rpn_string, double &x, int &errors);
  std::string get_data();

 private:
  std::string result_;

  void convert_unary(std::string &string);
  int is_normal_input(std::string &string);
  char tokenize_operator(std::string &token);
  int operator_priority(char token);
  double process_stack(std::stack<double> &stack, int &errors, char *operand);
  double count_numbers(char *operand, std::stack<double> &number_stack,
                       int &errors);
};
}  // namespace s21
