#include <iostream>
#include <vector>
#include <fstream>

enum status {
 start = 0,
 completed,
 failed
};

class Model {
 public:
  Model() {
  }
  ~Model() {
  }

  void add_task(const std::string& task) const {
    std::ofstream file("tasks.txt", std::ios::app);
    if (!file.is_open()) {
      std::cout << "File is not opened";
      exit(EXIT_FAILURE);
    }

    file << task << std::endl;

    file.close();

    std::cout << "Task was added" << std::endl;

  }
};