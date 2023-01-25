#include <gtest/gtest.h>

#include <cmath>

#include "../s21_map.h"

TEST(Constructors_Map, map_Test_01) {
  s21::map<int, char> a;
  int c = 1;
  char b = 'a';
  a.insert(c, b);
}