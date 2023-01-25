#include <gtest/gtest.h>

#include "../s21_stack.h"

TEST(stack, init_1) {
  s21::Stack<int> a;
  s21::Stack<double> b;
  s21::Stack<std::string> c;
  ASSERT_EQ(sizeof(a), sizeof(b));
  ASSERT_EQ(sizeof(a), sizeof(c));
  ASSERT_EQ(sizeof(b), sizeof(c));
}

TEST(stack, init_2) {
  s21::Stack<int> a{3};
  s21::Stack<double> b{1, 2};
  s21::Stack<std::string> c{"abc", "def", ""};
  ASSERT_EQ(sizeof(a), sizeof(b));
  ASSERT_EQ(sizeof(a), sizeof(c));
  ASSERT_EQ(sizeof(b), sizeof(c));
}

TEST(stack, init_3) {
  s21::Stack<int> a{3, 4, 5, 6, 7, 8};
  s21::Stack<int> b(a);
  ASSERT_EQ(sizeof(a), sizeof(b));
}

TEST(stack, init_4) {
  s21::Stack<int> a{3, 4, 5, 6, 7, 8};
  s21::Stack<int> b = a;
  ASSERT_EQ(sizeof(a), sizeof(b));
}

TEST(stack, init_5) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 5, 6, 7, 8};
  s21::Stack<int> a{3, 4, 5, 6, 7, 8};
  s21::list_Node<int> c(5);
  ASSERT_NE(sizeof(b), sizeof(c));
  ASSERT_EQ(sizeof(b), sizeof(a));
}

TEST(stack, top1) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 5, 6, 7, 8};
  s21::Stack<int> a{3, 4, 5, 6, 7, 8};
  ASSERT_EQ(a.top(), b.top());
}

TEST(stack, top2) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 5, 6, 7, 8};
  s21::Stack<int> a{5};
  ASSERT_NE(a.top(), b.top());
}

TEST(stack, empty1) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 5, 6, 7, 8};
  s21::Stack<int> a(b);
  ASSERT_FALSE(a.empty());
  s21::Stack<int> c;
  s21::Stack<int> d = c;
  ASSERT_TRUE(c.empty());
  ASSERT_TRUE(d.empty());
}

TEST(stack, empty2) {
  s21::Stack<int> b = s21::Stack<int>{3, 4};
  ASSERT_FALSE(b.empty());
  b.pop();
  ASSERT_FALSE(b.empty());
  b.pop();
  ASSERT_TRUE(b.empty());
}

TEST(stack, size1) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 6, 7, 8, 14};
  ASSERT_EQ(b.size(), 6);
  b.pop();
  ASSERT_EQ(b.size(), 5);
  b.pop();
  b.pop();
  ASSERT_EQ(b.size(), 3);
  b.pop();
  b.pop();
  b.pop();
  ASSERT_EQ(b.size(), 0);
}

TEST(stack, size2) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 6, 7, 8, 14};
  s21::Stack<int> c = b;
  ASSERT_EQ(c.size(), 6);
}

TEST(stack, size3) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 6, 7, 8, 14};
  s21::Stack<int> c(b);
  ASSERT_EQ(c.size(), 6);
}

TEST(stack, size4) {
  s21::Stack<int> c(s21::Stack<int>{3, 4, 6, 7, 8, 14});
  ASSERT_EQ(c.size(), 6);
}

TEST(stack, size5) {
  s21::Stack<int> c;
  ASSERT_EQ(c.size(), 0);
}

TEST(stack, push1) {
  s21::Stack<int> c;
  c.push(34);
  ASSERT_EQ(c.size(), 1);
  ASSERT_EQ(c.top(), 34);
}

TEST(stack, push2) {
  s21::Stack<int> c(s21::Stack<int>{3, 4, 6, 7, 8, 14});
  c.push(34);
  ASSERT_EQ(c.size(), 7);
  ASSERT_EQ(c.top(), 34);
  c.pop();
  ASSERT_EQ(c.size(), 6);
  ASSERT_EQ(c.top(), 14);
}

TEST(stack, push3) {
  s21::Stack<int> c(s21::Stack<int>{3});
  c.pop();
  c.push(34);
  c.push(35);
  c.push(36);
  ASSERT_EQ(c.size(), 3);
  ASSERT_EQ(c.top(), 36);
  c.pop();
  c.pop();
  ASSERT_EQ(c.size(), 1);
  ASSERT_EQ(c.top(), 34);
}

/*TEST(stack, runtime1) {
  s21::Stack<int> c;
  int d = 0;
  while (++d != 10000000) c.push(d);
  ASSERT_EQ(c.size(), 9999999);
  ASSERT_EQ(c.top(), 9999999);
}

TEST(stack, runtime2) {
  s21::Stack<int> c;
  int d = 0;
  while (++d != 10000000) c.push(d);
  ASSERT_EQ(c.size(), 9999999);
  ASSERT_EQ(c.top(), 9999999);
  while (--d != 0) c.pop();
  ASSERT_EQ(c.size(), 0);
}
*/
TEST(stack, swap1) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 6, 7, 8, 14};
  s21::Stack<int> d = s21::Stack<int>{3, 4, 6};
  b.swap(d);
  ASSERT_EQ(b.size(), 3);
  ASSERT_EQ(d.size(), 6);
}

TEST(stack, swap2) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 6, 7, 8, 14};
  s21::Stack<int> d;
  s21::Stack<int> e(d);
  b.swap(e);
  ASSERT_EQ(b.size(), 0);
  ASSERT_EQ(e.size(), 6);
  ASSERT_EQ(e.top(), 14);
}

TEST(stack, emplace_front) {
  s21::Stack<int> b = s21::Stack<int>{3, 4, 6, 7, 8, 14};
  b.s21::Stack<int>::emplace_front(11, 14, 18, 10);
  ASSERT_EQ(b.top(), 10);
  b.pop();
  ASSERT_EQ(b.top(), 18);
  b.pop();
  ASSERT_EQ(b.top(), 14);
  b.pop();
  ASSERT_EQ(b.top(), 11);
  b.pop();
  ASSERT_EQ(b.top(), 14);
}
