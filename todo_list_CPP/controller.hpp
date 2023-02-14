#include "model.h"

class Controller {
 public:
  Controller() {}
  Controller(Model *m) : model_(m) {}
  void add_task(const std::string& name) const { model_->add_task(name); }

 private:
  Model *model_;
};