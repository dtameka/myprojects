#include <iostream>
#include "view.h"

int main() {
  Model model;
  Controller controller(&model);
  View view(&controller);

  view.show_menu();
}
