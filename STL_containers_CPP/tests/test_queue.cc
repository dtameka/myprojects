#include <gtest/gtest.h>

#include "../s21_queue.h"

TEST(queue, init_1) {
  s21::Queue<int> a;
  s21::Queue<double> b;
  s21::Queue<std::string> c;
  ASSERT_EQ(sizeof(a), sizeof(b));
  ASSERT_EQ(sizeof(a), sizeof(c));
  ASSERT_EQ(sizeof(b), sizeof(c));
}

TEST(queue, init_2) {
  s21::Queue<int> a{3};
  s21::Queue<double> b{1, 2};
  s21::Queue<std::string> c{"abc", "def", ""};
  ASSERT_EQ(sizeof(a), sizeof(b));
  ASSERT_EQ(sizeof(a), sizeof(c));
  ASSERT_EQ(sizeof(b), sizeof(c));
}

TEST(queue, init_3) {
  s21::Queue<int> a{3, 4, 5, 6, 7, 8};
  s21::Queue<int> b(a);
  ASSERT_EQ(sizeof(a), sizeof(b));
}

TEST(queue, init_4) {
  s21::Queue<int> a{3, 4, 5, 6, 7, 8};
  s21::Queue<int> b = a;
  ASSERT_EQ(sizeof(a), sizeof(b));
}

TEST(queue, init_5) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 5, 6, 7, 8};
  s21::Queue<int> a{3, 4, 5, 6, 7, 8};
  s21::list_Node<int> c(5);
  ASSERT_EQ(sizeof(b), sizeof(c));
  ASSERT_EQ(sizeof(b), sizeof(a));
}

TEST(queue, front1) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 5, 6, 7, 8};
  s21::Queue<int> a{3, 4, 5, 6, 7, 8};
  ASSERT_EQ(a.front(), b.front());
}

TEST(queue, front2) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 5, 6, 7, 8};
  s21::Queue<int> a{5};
  ASSERT_NE(a.front(), b.front());
}

TEST(queue, back1) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 5, 6, 7, 8};
  s21::Queue<int> a{3, 4, 5, 6, 7, 8};
  ASSERT_EQ(a.back(), b.back());
}

TEST(queue, back2) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 5, 6, 7, 8};
  s21::Queue<int> a{5};
  ASSERT_NE(a.back(), b.back());
}

TEST(queue, empty1) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 5, 6, 7, 8};
  s21::Queue<int> a(b);
  ASSERT_FALSE(a.empty());
  s21::Queue<int> c;
  s21::Queue<int> d = c;
  ASSERT_TRUE(c.empty());
  ASSERT_TRUE(d.empty());
}

TEST(queue, empty2) {
  s21::Queue<int> b = s21::Queue<int>{3, 4};
  ASSERT_FALSE(b.empty());
  b.pop();
  ASSERT_FALSE(b.empty());
  b.pop();
  ASSERT_TRUE(b.empty());  //  wtf WTF WTF WTF WTF WTF WTF
}

TEST(queue, size1) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 6, 7, 8, 14};
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

TEST(queue, size2) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 6, 7, 8, 14};
  s21::Queue<int> c = b;
  ASSERT_EQ(c.size(), 6);
}

TEST(queue, size3) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 6, 7, 8, 14};
  s21::Queue<int> c(b);
  ASSERT_EQ(c.size(), 6);
}

TEST(queue, size4) {
  s21::Queue<int> c(s21::Queue<int>{3, 4, 6, 7, 8, 14});
  ASSERT_EQ(c.size(), 6);
}

TEST(queue, size5) {
  s21::Queue<int> c;
  ASSERT_EQ(c.size(), 0);
}

TEST(queue, push1) {
  s21::Queue<int> c;
  c.push(34);
  ASSERT_EQ(c.size(), 1);
  ASSERT_EQ(c.back(), 34);
}

TEST(queue, push2) {
  s21::Queue<int> c(s21::Queue<int>{3, 4, 6, 7, 8, 14});
  c.push(34);
  ASSERT_EQ(c.size(), 7);
  ASSERT_EQ(c.back(), 34);
  c.pop();
  ASSERT_EQ(c.size(), 6);
  ASSERT_EQ(c.back(), 34);
}

TEST(queue, push3) {
  int a = 3;
  s21::Queue<int> c;
  c.push(a);
  c.pop();
  c.push(35);
  c.push(36);
  c.push(37);
  ASSERT_EQ(c.size(), 3);
  ASSERT_EQ(c.back(), 37);
  c.pop();
  c.pop();
  ASSERT_EQ(c.size(), 1);
  ASSERT_EQ(c.back(), 37);
  ASSERT_EQ(c.front(), 37);
}

/*TEST(queue, runtime1) {
  s21::Queue<int> c;
  int d = 0;
  while (++d != 10000000) c.push(d);
  ASSERT_EQ(c.size(), 9999999);
  ASSERT_EQ(c.back(), 9999999);
}

TEST(queue, runtime2) {
  s21::Queue<int> c;
  int d = 0;
  while (++d != 10000000) c.push(d);
  ASSERT_EQ(c.size(), 9999999);
  ASSERT_EQ(c.back(), 9999999);
  while (--d != 0) c.pop();
  ASSERT_EQ(c.size(), 0);
}
*/
TEST(queue, swap1) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 6, 7, 8, 14};
  s21::Queue<int> d = s21::Queue<int>{3, 4, 6};
  b.swap(d);
  ASSERT_EQ(b.size(), 3);
  ASSERT_EQ(d.size(), 6);
}

TEST(queue, swap2) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 6, 7, 8, 14};
  s21::Queue<int> d;
  s21::Queue<int> e(d);
  b.swap(e);
  ASSERT_EQ(b.size(), 0);
  ASSERT_EQ(e.size(), 6);
  ASSERT_EQ(e.back(), 14);
}

TEST(queue, emplace_back) {
  s21::Queue<int> b = s21::Queue<int>{3, 4, 6, 7, 8, 14};
  b.emplace_back(3, 6, 12);
  ASSERT_EQ(b.back(), 12);
  ASSERT_EQ(b.size(), 9);
}
