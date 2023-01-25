#include <gtest/gtest.h>

#include "../s21_array.h"

TEST(Constructors_Array, Test_01) {
  s21::array<int, 5> a;
  s21::array<int, 5> b = {1, 2, 3, 4, 5};
  s21::array<int, 5> c{1, 2, 3, 4, 5};
  s21::array<int, 5> g(c);

  auto it = b.begin();
  auto it2 = c.begin();
  ASSERT_EQ(*it, *it2);
  ASSERT_EQ(*(g.begin()), *it2);
  ASSERT_EQ(*(g.begin() + 3), *(it2 + 3));

  s21::array<int, 5> d = {std::move(b)};

  ASSERT_FALSE(b.empty());
  ASSERT_EQ(*(d.begin()), 1);
}

TEST(Element_access_Array, Test_01) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  ASSERT_EQ(*(a.data()), 1);
  ASSERT_EQ(a.at(0), 1);
  ASSERT_EQ(a[0], 1);
  ASSERT_EQ(a.front(), 1);
  ASSERT_EQ(a.back(), 5);
}

TEST(Iterators_Array, Test_01) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  ASSERT_EQ(*(a.begin()), 1);
  ASSERT_EQ(*(a.end() - 1), 5);
}

TEST(Capacity_Array, Test_01) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  ASSERT_FALSE(a.empty());
  ASSERT_EQ(a.size(), 5);
  ASSERT_EQ(a.max_size(), 5);
}

TEST(Modifiers_Array, Test_01) {
  s21::array<int, 5> a = {1, 2, 3, 4, 5};
  s21::array<int, 5> b = {5, 4, 3, 2, 1};
  s21::array<int, 5> b2 = {1, 2, 3, 4, 5};
  s21::array<int, 5> a2 = {5, 4, 3, 2, 1};

  a.swap(b);

  ASSERT_TRUE(a == a2);
  ASSERT_TRUE(b == b2);

  s21::array<int, 5> f;
  s21::array<int, 5> f2 = {5, 5, 5, 5, 5};
  f.fill(5);
  ASSERT_TRUE(f == f2);
}
