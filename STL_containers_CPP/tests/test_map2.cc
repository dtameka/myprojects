#include <gtest/gtest.h>

#include "../s21_map.h"

TEST(Constructors_Map2, Test_01) {
  s21::map<int, int> a;
  s21::map<int, int> b = {std::pair<int, int>(1, 10),
                          std::pair<int, int>(2, 20),
                          std::pair<int, int>(3, 30)};
  s21::map<int, int> g{std::pair<int, int>(1, 10), std::pair<int, int>(2, 20),
                       std::pair<int, int>(3, 30)};
  s21::map<int, int> c(b);
  auto it1 = b.begin();
  auto it2 = c.begin();
  bool res = (*it1 == *it2);
  ASSERT_TRUE(res);
  it1++;
  it2++;
  res = (*it1 == *it2);
  ASSERT_TRUE(res);
  it1 = b.end();
  it2 = c.end();
  it1--;
  it2--;
  res = (*it1 == *it2);
  ASSERT_TRUE(res);

  s21::map<int, int> d(std::move(b));
  auto it3 = d.begin();

  it2 = c.begin();
  ASSERT_TRUE(*it2 == *it3);
  it2++;
  it3++;
  ASSERT_TRUE(*it2 == *it3);
  it2 = c.end();
  it3 = d.end();
  it2--;
  it3--;
  ASSERT_TRUE(*it2 == *it3);
}

TEST(Capacity_Map2, Test_01) {
  s21::map<int, int> a;
  bool res = a.empty();
  ASSERT_TRUE(res);
  a.insert(std::pair<int, int>(1, 10));
  res = a.empty();
  ASSERT_FALSE(res);
  ASSERT_EQ(1, a.size());
  ASSERT_EQ((LLONG_MAX / sizeof(std::pair<int, int>)), a.max_size());
}

TEST(Modifiers_Map2, Test_01) {
  s21::map<int, int> a = {
      std::pair<int, int>(1, 10), std::pair<int, int>(2, 20),
      std::pair<int, int>(3, 30), std::pair<int, int>(4, 40),
      std::pair<int, int>(5, 50)};

  s21::map<int, int> b = {
      std::pair<int, int>(5, 50), std::pair<int, int>(4, 40),
      std::pair<int, int>(3, 30), std::pair<int, int>(2, 20),
      std::pair<int, int>(1, 10)};

  a.swap(b);
  ASSERT_EQ(*(b.begin()), 10);
  a.erase(a.begin());
  ASSERT_EQ(*(a.begin()), 20);
  auto it = a.end();
  it--;
  ASSERT_EQ(*it, 50);
  a.erase(it);
  it = a.end();
  it--;
  ASSERT_EQ(*it, 40);
}

TEST(Modifiers_Map2, Test_02) {
  s21::map<int, int> a = {
      std::pair<int, int>(1, 10), std::pair<int, int>(2, 20),
      std::pair<int, int>(3, 30), std::pair<int, int>(4, 40),
      std::pair<int, int>(5, 50)};

  s21::map<int, int> b = {
      std::pair<int, int>(5, 10),  std::pair<int, int>(4, 20),
      std::pair<int, int>(30, 30), std::pair<int, int>(3, 40),
      std::pair<int, int>(2, 50),  std::pair<int, int>(1, 50),
      std::pair<int, int>(20, 50)};

  a.merge(b);
  auto it = a.end();
  it--;
  ASSERT_TRUE(*it = 30);
  auto itb = b.begin();
  itb++;
  itb++;
  ASSERT_TRUE(*itb == 40);
}

TEST(Lookup_Map2, Test_01) {
  s21::map<int, int> a = {
      std::pair<int, int>(1, 10), std::pair<int, int>(2, 20),
      std::pair<int, int>(3, 30), std::pair<int, int>(4, 40),
      std::pair<int, int>(5, 50)};

  bool res = a.contains(3);
  ASSERT_TRUE(res);
}
TEST(Emplace_Map2, Test_01) {
  s21::map<int, int> a;
  a.emplace(1, 10);
  a.emplace(2, 20);
  auto it = a.begin();
  ASSERT_EQ(*it, 10);
  it++;
  ASSERT_EQ(*it, 20);
}