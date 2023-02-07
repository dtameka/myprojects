#include <iostream>
#include "main.hpp"

int main() {
  View v;
  char variant;
  bool exit = false;
  bool first = false; 
  bool second = false;
  while (1) {
    v.print_main();
    std::cin >> variant;
    switch (variant) {
    case '1':
      v.print_add();
      while (1) {
      if (first && second) {
        break;
      }
      std::cin >> variant;
      if (variant == '1' && !first) {
      // Выбираем имя для нашей задачи
      first = true;
      } 
      if (variant == '2' && !second) {
      // Выбираем дату сдачи
      second = true;
      } 
      if (variant == '0') {
        break;
      }
      }
      break;
    case 'x':
      exit = true;
    }
    if (exit) {
      break;
    }
  }
}