#ifndef ENCRYPTER_H_
#define ENCRYPTER_H_

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

#include "../../interfaces/interfaces_for_encrypter.h"

class Encrypter : public IPreparator, public IEncrypter {
 public:
  Encrypter() = default;
  explicit Encrypter(std::string &main_buffer);
  ~Encrypter();

  bool setData() override;
  void bindBuffer(std::string &main_buffer) override;
  void startEncrypt() override;

 private:
  void setDefaultForStrings();

  // сортируем полученный массив
  void sort() override;

  // Шифрует чётные значения и погружает всё в выходной буффер в виде строки.
  void encryptArray();

  // Переводит полученную строку в массив интов.
  void parseEnter() override;

  // Проверка, что массив состоит исключитеьно из чисел(нету поддержки ввода со
  // знаком "+1/-1")
  bool isNumeric();

  int validateEnter() override;

  std::string *p_main_buffer_ = nullptr;

  std::string input_str_{};
  std::vector<int> int_str_{};
  bool want_exit = false;
};

#endif  // ENCRYPTER_H_