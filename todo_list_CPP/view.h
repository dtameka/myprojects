#include <iostream>
#include <vector>
#include "controller.hpp"

class IScreen {
  public:
  void virtual paint() = 0;
  virtual ~IScreen() {}
};

class MainScreen : public IScreen {
  public:
  void paint() override {
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|Todo list by Dtameka    |" << std::endl;
  std::cout << "|Version - v0.1          |" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[1] - Добавить задачу   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[2] - Все задачи        |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[3] - Найти задачу      |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[4] - Удалить задачу    |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[5] - Удалить всё       |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[x] - Выход             |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << std::endl << std::endl;
  }
};

class AddScreen : public IScreen {
  public:
  AddScreen() {}
  ~AddScreen() {}
  void paint() override {
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "| Создание новой задачи  |" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  }
};

class DeleteScreen : public IScreen {
 public:
  DeleteScreen() {}
  ~DeleteScreen() {}
  void paint() override {
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|    Удаление задачи     |" << std::endl;
  std::cout << "| Выберите для удаления  |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "|[0] - Вернуться назад   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  }
};

class StartScreen : public IScreen {
  public:
  StartScreen() {}
  ~StartScreen() {}
  void paint() override {
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|       Ваши задачи      |" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "|[0] - Вернуться назад   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  }
};

class View {
 public:
  View() {
    mainSc = new MainScreen;
    addSc = new AddScreen;
    deleteSc = new DeleteScreen;
    startSc = new StartScreen;
  }

  View(Controller *controller) : controller_(controller) {
    mainSc = new MainScreen;
    addSc = new AddScreen;
    deleteSc = new DeleteScreen;
    startSc = new StartScreen;
  }

  ~View() {
    delete startSc;
    delete deleteSc;
    delete addSc;
    delete mainSc;
  }

  void print_main() {
    mainSc->paint();
  }
  void print_add() {
    addSc->paint();
  }
  void print_delete() {
    deleteSc->paint();
  }
  void print_start() {
    startSc->paint();
  }

  void show_menu() {
    while(1) {
      print_main();
      std::cout << "Ваш выбор: ";
      char choice = ' ';
      std::cin >> choice;
      if (choice == '1') {
        system("clear");
        
        std::string name;
        std::cout << "Введите вашу задачу: ";
        getline(std::cin, name); // пропуск переноса после предыдущего cin
        getline(std::cin, name);
        controller_->add_task(name);

        system("pause");


      } else if (choice == '2') {

      } else if (choice == '3') {

      } else if (choice == '4') {

      } else if (choice == '5') {

      } else if (choice == 'x') {
        exit(1);
      }
    }
  }

 private:
  MainScreen *mainSc;
  AddScreen *addSc;
  DeleteScreen *deleteSc;
  StartScreen *startSc;
  Controller *controller_;
};