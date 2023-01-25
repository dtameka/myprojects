#include <gtest/gtest.h>

#include "../s21_queue.h"
#include "../s21_stack.h"

#ifndef __APPLE__
TEST(queue, throw1) {
  s21::Queue<int> a;
  ASSERT_THROW(a.front(), std::out_of_range);
}

TEST(queue, throw2) {
  s21::Queue<int> a;
  ASSERT_THROW(a.back(), std::out_of_range);
}

TEST(stack, top3) {
  s21::Stack<int> a;
  ASSERT_THROW(a.top(), std::out_of_range);
}
#endif

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}