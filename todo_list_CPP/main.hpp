#include <iostream>
#include <vector>

enum status {
 start = 0,
 completed,
 failed
};

class Data {
  int taskId;
  std::string taskName;
  std::vector<std::pair<int, status>> taskStatus;
  std::string taskDuration;
};

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
  std::cout << "|[2] - Удалить задачу    |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[3] - Начатые задачи    |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[4] - Законченые задачи |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[x] - Выход             |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << std::endl << std::endl;
  }
};

class AddScreen : public IScreen {
  public:
  AddScreen(Data *data) : data_(data) {}
  ~AddScreen() {}
  void paint() override {
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "| Создание новой задачи  |" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[1] - Название задачи   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[2] - До какого числа   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|[0] - Вернуться назад   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  }
  private:
  Data *data_;
};

class DeleteScreen : public IScreen {
 public:
  DeleteScreen(Data *data) : data_(data) {}
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
 private:
  Data *data_;
};

class StartScreen : public IScreen {
  public:
  StartScreen(Data *data) : data_(data) {}
  ~StartScreen() {}
  void paint() override {
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|     Начатые задачи     |" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "|[0] - Вернуться назад   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  }
 private:
  Data *data_;
};

class FinishedScreen : public IScreen {
  public:
  FinishedScreen(Data *data) : data_(data) {}
  ~FinishedScreen() {}
  void paint() override {
  std::cout << "|------------------------|" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|   Законченые задачи    |" << std::endl;
  std::cout << "|                        |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "|[0] - Вернуться назад   |" << std::endl;
  std::cout << "|------------------------|" << std::endl;
  }
 private:
  Data *data_;
};

class View {
 public:
  View() {
    data_ = new Data;
    mainSc = new MainScreen;
    addSc = new AddScreen(data_);
    deleteSc = new DeleteScreen(data_);
    startSc = new StartScreen(data_);
    finishSc = new FinishedScreen(data_);
  }

  ~View() {
    delete finishSc;
    delete startSc;
    delete deleteSc;
    delete addSc;
    delete mainSc;
    delete data_;
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

  void print_finished() {
    finishSc->paint();
  }

 private:
  MainScreen *mainSc;
  AddScreen *addSc;
  DeleteScreen *deleteSc;
  StartScreen *startSc;
  FinishedScreen *finishSc;
  Data *data_;

};