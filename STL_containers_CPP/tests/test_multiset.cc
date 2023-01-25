#include <gtest/gtest.h>

#include "../s21_multiset.h"

TEST(multiset, init_1) {
  s21::Multiset<int> a;
  s21::Multiset<int> a1(a);
  s21::Multiset<int> b1{1, 2, 3, 4, 5};
  s21::set<int> b2{1, 2, 3, 4, 5};
  s21::Multiset<int> e = a;
  s21::Multiset<int> c = s21::Multiset<int>();
}

TEST(multiset, begin) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  s21::SetIterator<int> itr1 = b1.begin();
  ASSERT_EQ(*itr1, 1);
  itr1++;
  ASSERT_EQ(*itr1, 1);
  itr1++;
  ASSERT_EQ(*itr1, 3);
  itr1++;
  ASSERT_EQ(*itr1, 4);
  itr1++;
  ASSERT_EQ(*itr1, 5);
}

TEST(multiset, begin_2) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  b1.insert(-3);
  s21::SetIterator<int> itr1 = b1.begin();
  ASSERT_EQ(*itr1, -3);
  itr1++;
  ASSERT_EQ(*itr1, 1);
  itr1++;
  ASSERT_EQ(*itr1, 1);
  itr1++;
  ASSERT_EQ(*itr1, 3);
  itr1++;
  ASSERT_EQ(*itr1, 4);
  itr1++;
  ASSERT_EQ(*itr1, 5);
}

TEST(multiset, empty) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  s21::Multiset<int> b2;
  ASSERT_TRUE(b2.empty());
  ASSERT_FALSE(b1.empty());
}

TEST(multiset, empty_2) {
  s21::Multiset<int> b1{1};
  s21::SetIterator<int> itr1 = b1.begin();
  b1.erase(itr1);
  ASSERT_TRUE(b1.empty());
}

TEST(multiset, size) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  s21::Multiset<int> b2{1, 1, 2, 2, 3, 3, 3, 4};
  s21::Multiset<int> b3;
  ASSERT_EQ(b1.size(), 5);
  ASSERT_EQ(b2.size(), 8);
  ASSERT_EQ(b3.size(), 0);
  s21::SetIterator<int> itr1 = b2.begin();
  b2.erase(itr1);
  ASSERT_EQ(b2.size(), 7);
}

TEST(multiset, max_size) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  s21::Multiset<int> b2{1, 1, 2, 2, 3, 3, 3, 4};
  s21::Multiset<int> b3;
  ASSERT_EQ(b1.max_size(), LLONG_MAX / sizeof(int));
  ASSERT_EQ(b2.max_size(), b3.max_size());
}

TEST(multiset, clear) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  s21::Multiset<int> b2;
  b1.clear();
  b2.clear();
  ASSERT_EQ(b1.size(), 0);
  ASSERT_EQ(b2.size(), 0);
}

TEST(multiset, insert) {
  s21::Multiset<int> b1{1, 2, 3, 4, 5};
  s21::SetIterator<int> itr1 = b1.begin();
  b1.insert(1);
  itr1--;
  ASSERT_EQ(*itr1, 1);
  ASSERT_EQ(b1.size(), 6);
}

TEST(multiset, insert2) {
  s21::Multiset<int> b1;
  b1.insert(1);
  s21::SetIterator<int> itr1 = b1.begin();
  ASSERT_EQ(*itr1, 1);
  ASSERT_EQ(b1.size(), 1);
}

TEST(multiset, insert3) {
  s21::Multiset<int> b1{3, 4, 2, 8, 6, 5};
  b1.insert(1);
  s21::SetIterator<int> itr1 = b1.begin();
  ASSERT_EQ(*itr1, 1);
  b1.insert(-6);
  --itr1;
  ASSERT_EQ(*itr1, -6);
  b1.insert(-1);
  ++itr1;
  ++itr1;
  ASSERT_EQ(*itr1, 1);
  ++itr1;
  ASSERT_EQ(*itr1, 2);
}

TEST(multiset, erase) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  s21::SetIterator<int> itr1 = b1.begin();
  ASSERT_EQ(*itr1, 1);
  ASSERT_EQ(b1.size(), 5);
  b1.erase(itr1);
  itr1 = b1.begin();
  ASSERT_EQ(*itr1, 1);
  ASSERT_EQ(b1.size(), 4);
  b1.erase(itr1);
  itr1 = b1.begin();
  ASSERT_EQ(*itr1, 3);
  ASSERT_EQ(b1.size(), 3);
}

TEST(multiset, swap) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  s21::Multiset<int> b2{1, 1, 3, 4, 5, 1, 3, 4, 5};
  ASSERT_EQ(b2.size(), 9);
  b1.swap(b2);
  ASSERT_EQ(b1.size(), 9);
  ASSERT_EQ(b2.size(), 5);
}

TEST(multiset, merge) {
  s21::Multiset<int> b1{6, 3, 3, 4, 5};
  s21::Multiset<int> b2{1, 1, 3, 4, 5, 1, 3, 4, 5};
  b1.merge(b2);
  ASSERT_EQ(b1.size(), 14);
  s21::SetIterator<int> itr1 = b1.begin();
  ASSERT_EQ(*itr1, 1);
}

TEST(multiset, count) {
  s21::Multiset<int> b1{1, 1, 3, 4, 5};
  ASSERT_EQ(b1.count(1), 2);
}

TEST(multiset, count_2) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  ASSERT_EQ(b1.count(1), 5);
  ASSERT_EQ(b1.count(3), 1);
  ASSERT_EQ(b1.count(7), 0);
}

TEST(multiset, find) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  s21::SetIterator<int> itr1 = b1.find(3);
  ASSERT_EQ(*itr1, 3);
}

TEST(multiset, contains) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  ASSERT_TRUE(b1.contains(3));
  ASSERT_FALSE(b1.contains(7));
}

TEST(multiset, equal_range) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  std::pair<s21::SetIterator<int>, s21::SetIterator<int>> result =
      b1.equal_range(1);
  ASSERT_EQ(*(result.first), 1);
}

TEST(multiset, lower_bound) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  s21::SetIterator<int> itr1 = b1.lower_bound(1);
  ASSERT_EQ(*itr1, 1);
}

TEST(multiset, lower_bound_2) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  s21::SetIterator<int> itr1 = b1.lower_bound(2);
  ASSERT_EQ(*itr1, 3);
}

TEST(multiset, lower_bound_4) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  s21::SetIterator<int> itr1 = b1.lower_bound(6);
  ASSERT_EQ(*itr1, 5);
}

TEST(multiset, upper_bound) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  s21::SetIterator<int> itr1 = b1.upper_bound(1);
  ASSERT_EQ(*itr1, 3);
}

TEST(multiset, upper_bound_2) {
  s21::Multiset<int> b1{1, 1, 3, 1, 1, 1, 4, 5};
  s21::SetIterator<int> itr1 = b1.upper_bound(6);
  ASSERT_EQ(*itr1, 5);
}

TEST(multiset, node_balance) {
  s21::Multiset<int> b1{1, 1, 1, 1, 2, 3, 4, 5};
  s21::SetIterator<int> itr1 = b1.begin();
  ASSERT_EQ(itr1.show_balance(), 0);
  ++itr1;
  ASSERT_EQ(itr1.show_balance(), -1);
  ++itr1;
  ++itr1;
  ASSERT_EQ(itr1.show_balance(), 1);
  ++itr1;
  ++itr1;
  ++itr1;
  ASSERT_EQ(itr1.show_balance(), 1);
}