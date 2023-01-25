#include <gtest/gtest.h>

#include <vector>

#include "../s21_vector.h"

TEST(Constructors_Vector, vector_Test_01) {
  s21::vector<int> a;
  s21::vector<int> b(1);
  s21::vector<int> c = {10, 2};
  int size_b = 1;
  int size_c = 2;
  ASSERT_EQ(b.size(), size_b);
  ASSERT_EQ(c.size(), size_c);
  b = std::move(c);
  ASSERT_EQ(b.size(), size_c);
  ASSERT_EQ(c.size(), 0);
  ASSERT_FALSE(b == c);
  s21::vector<int> d(b);
  ASSERT_TRUE(b == d);
  s21::vector<int> f(std::move(b));
  ASSERT_TRUE(c == b);
  ASSERT_EQ(f.size(), size_c);
}

TEST(Element_Access_Vector, vector_Test_01) {
  s21::vector<int> a = {1, 2, 3, 4, 5};
  ASSERT_EQ(a.at(3), 4);
  ASSERT_EQ(a.at(4), 5);
  EXPECT_THROW(a.at(5), std::out_of_range);
}
TEST(Element_Access_Vector, vector_Test_02) {
  s21::vector<int> a = {1, 2, 3, 4, 5};
  ASSERT_EQ(a[3], 4);
  ASSERT_EQ(a[4], 5);
}
TEST(Element_Access_Vector, vector_Test_03) {
  s21::vector<int> a = {1, 2, 3, 4, 5};
  ASSERT_EQ(a.front(), 1);
  ASSERT_EQ(a.back(), 5);
  ASSERT_EQ(*(a.data()), 1);
}
TEST(Element_Access_Vector, vector_Test_04) {
  s21::vector<int> a = {1, 2, 3, 4, 5};
  ASSERT_EQ(*(a.begin()), 1);
  ASSERT_EQ(*(a.end() - 1), 5);
}
TEST(Capacity_Vector, vector_Test_01) {
  s21::vector<int> a;
  std::vector<int> b;
  ASSERT_TRUE(a.empty());
  ASSERT_EQ(a.size(), 0);
  ASSERT_EQ(a.max_size(), b.max_size());
}
TEST(Capacity_Vector, vector_Test_02) {
  s21::vector<int> a;
  a.reserve(10);
  ASSERT_EQ(a.capacity(), 10);
  a.push_back(5);
  a.shrink_to_fit();
  ASSERT_EQ(a.capacity(), 1);
}

TEST(Modifiers_Vector, vector_Test_01) {
  s21::vector<int> a;
  a.push_back(5);
  ASSERT_TRUE(a[0] == 5);
  ASSERT_TRUE(a.size() == 1);
  a.push_back(10);
  ASSERT_TRUE(a[1] == 10);
  ASSERT_TRUE(a.size() == 2);
  a.pop_back();
  ASSERT_TRUE(a.size() == 1);
  a.clear();
  ASSERT_TRUE(a.data() == nullptr);
}
TEST(Modifiers_Vector, vector_Test_02) {
  s21::vector<int> a = {1, 2, 3, 4, 5};
  s21::vector<int> b = {1, 5, 2, 3, 4, 5};
  a.insert(a.begin() + 1, 5);
  ASSERT_TRUE(a == b);
  s21::vector<int>::iterator it = a.begin();
  a.insert(it + 2, 10);
  s21::vector<int> c = {1, 5, 10, 2, 3, 4, 5};
  ASSERT_TRUE(a == c);
}
TEST(Modifiers_Vector, vector_Test_03) {
  s21::vector<int> a = {1, 5, 10, 2, 3, 4, 5};
  s21::vector<int> b = {1, 2, 3, 4, 5};
  a.erase(a.begin() + 1);
  a.erase(a.begin() + 1);
  ASSERT_TRUE(a == b);
}
TEST(Modifiers_Vector, vector_Test_04) {
  s21::vector<int> a = {1, 5, 10, 2, 3, 4, 5};
  s21::vector<int> b = {1, 2, 3, 4, 5};
  a.swap(b);
  ASSERT_EQ(a.size(), 5);
  ASSERT_EQ(b.size(), 7);
  ASSERT_EQ(a[2], 3);
  ASSERT_EQ(b[2], 10);
}
TEST(Emplace_vector, vector_test_01) {
  s21::vector<int> a = {1};
  a.emplace(a.begin(), 2);
  ASSERT_EQ(*(a.begin()), 2);
  a.emplace_back(6);
  ASSERT_EQ(*(a.begin() + 2), 6);
}