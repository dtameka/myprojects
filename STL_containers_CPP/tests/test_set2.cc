#include <gtest/gtest.h>

#include "../s21_set.h"

TEST(Constructors_Set2, Test_01) {
  s21::set<int> a;
  s21::set<int> b = {1, 2, 3, 4, 5};
  s21::set<int> g{1, 2, 3, 4, 5};
  s21::set<int> c(b);
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

  s21::set<int> d(std::move(b));
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

TEST(Capacity_Set2, Test_01) {
  s21::set<int> a;
  bool res = a.empty();
  ASSERT_TRUE(res);
  a.insert(1);
  res = a.empty();
  ASSERT_FALSE(res);
  ASSERT_EQ(1, a.size());
  ASSERT_EQ((LLONG_MAX / sizeof(int)), a.max_size());
}

TEST(Modifiers_Set2, Test_01) {
  s21::set<int> a = {1, 2, 3, 4, 5};
  s21::set<int> b = {5, 4, 3, 2, 1};
  a.swap(b);
  ASSERT_EQ(*(b.begin()), 1);
  a.erase(a.begin());
  ASSERT_EQ(*(a.begin()), 2);
  auto it = a.end();
  it--;
  ASSERT_EQ(*it, 5);
  a.erase(it);
  it = a.end();
  it--;
  ASSERT_EQ(*it, 4);
}
TEST(Modifiers_Set2, Test_02) {
  s21::set<int> a = {1, 2, 3, 4, 5};
  s21::set<int> b = {5, 4, 30, 3, 2, 1, 20};
  a.merge(b);
  auto it = a.end();
  it--;
  ASSERT_TRUE(*it = 30);
  auto itb = b.begin();
  itb++;
  itb++;
  ASSERT_TRUE(*itb == 3);
}

TEST(Lookup_Set2, Test_01) {
  s21::set<int> a = {1, 2, 3, 8, 4, 5};
  auto it = a.find(3);
  it++;
  ASSERT_TRUE(*it == 4);

  bool res = a.contains(3);
  ASSERT_TRUE(res);
}