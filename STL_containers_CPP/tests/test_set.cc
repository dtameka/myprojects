#include <gtest/gtest.h>

#include <cmath>

#include "../s21_set.h"

TEST(Constructors_Set, Test_01) {
  s21::set<int> a;
  s21::set<int> b = {1, 2, 3, 8, 4, 6, 5, 9, 13, 11, 12, 18, 15, 14, 16, 17};
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

TEST(Capacity_Set, Test_01) {
  s21::set<int> a;
  bool res = a.empty();
  ASSERT_TRUE(res);
  a.insert(1);
  res = a.empty();
  ASSERT_FALSE(res);
  ASSERT_EQ(1, a.size());
  ASSERT_EQ((LLONG_MAX / sizeof(int)), a.max_size());
}

TEST(Modifiers_Set, Test_01) {
  s21::set<int> a = {1, 2, 3, 4, 5};
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

TEST(Modifiers_Set, Test_02) {
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

TEST(Lookup_Set, Test_01) {
  s21::set<int> a = {1, 2, 3, 8, 4, 5};
  auto it = a.find(3);
  it++;
  ASSERT_TRUE(*it == 4);

  bool res = a.contains(3);
  ASSERT_TRUE(res);
}

TEST(Set_Emplace, Test_01) {
  std::initializer_list<double> list = {1.2, 3.7, 3.14, 8.9};
  std::set<double> set1(list);
  s21::set<double> set2(list);
  set1.emplace(1.2);
  set1.emplace(3.7);
  set1.emplace(9.9);
  auto v2 = set2.emplace(1.2, 3.7, 9.9);
  auto it2 = v2.begin();
  for (int i = 0; i < 3; i++, ++it2) {
    if (i == 2) {
      ASSERT_TRUE((*it2).second);
    } else {
      ASSERT_FALSE((*it2).second);
    }
  }
  auto iter1 = set1.begin();
  auto iter2 = set2.begin();
  while (iter1 != set1.end() || iter2 != set2.end()) {
    ASSERT_DOUBLE_EQ(*iter1, *iter2);
    ++iter1;
    ++iter2;
  }
}

/*FOR TESTING YOU SHOULD CHANGE THE ACCESS RULE FROM 'PROTECTED'
TO 'PUBLIC' FOR SET::SET_BALANCE_FOR_ALL() AND FOR SET::GET_ROOT()*/

// TEST(balancing, Test_01) {
//     s21::set<int> a = {11, 9, 5, 6, 7, 10, 4, 3, 8, 16, 15};
//     s21::SetIterator<int> itr1 = a.begin();
//     s21::SetIterator<int> itr2 = a.end();
//     a.set_balance_for_all();
//     while (itr1.get_node() != itr2.get_node()) {
//             ASSERT_LT(abs(itr1.get_node()->balance), 2);
//         ++itr1;
//     }
// }

// TEST(balancing, Test_02) {
//     s21::set<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
//     s21::SetIterator<int> itr1 = a.begin();
//     s21::SetIterator<int> itr2 = a.end();
//     a.set_balance_for_all();
//     while (itr1.get_node() != itr2.get_node()) {
//             ASSERT_LT(abs(itr1.get_node()->balance), 2);
//         ++itr1;
//     }
// }

// TEST(balancing, Test_03) {
//     s21::set<int> a = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
//     s21::SetIterator<int> itr1 = a.begin();
//     s21::SetIterator<int> itr2 = a.end();
//     a.set_balance_for_all();
//     while (itr1.get_node() != itr2.get_node()) {
//             ASSERT_LT(abs(itr1.get_node()->balance), 2);
//         ++itr1;
//     }
// }