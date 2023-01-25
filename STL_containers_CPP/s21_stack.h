#ifndef _CONTAINERS_SRC_S21_STACK_
#define _CONTAINERS_SRC_S21_STACK_

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include "s21_node.h"
#include "s21_vector.h"

namespace s21 {

template <typename T>
class Stack {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

 private:
  list_Node<value_type> *head;
  size_type sizeOf;

 public:
  Stack() : head(nullptr), sizeOf(0){};
  Stack(const Stack &s) : Stack() { *this = s; };
  Stack(Stack &&s) : head(s.head), sizeOf(s.sizeOf){};
  Stack(std::initializer_list<value_type> const &buff) : Stack() {
    for (auto it : buff) {
      push(it);
    }
  }
  ~Stack() {
    while (sizeOf > 0) {
      pop();
    }
  };

  Stack &operator=(Stack &&s) {
    head = s.head;
    sizeOf = s.sizeOf;
    return *this;
  }

  Stack &operator=(const Stack &s) {
    while (!empty()) {
      pop();
    }
    if (s.head != nullptr) {
      list_Node<value_type> *chunk = new list_Node(*(s.head));
      head = chunk;
      sizeOf++;
      list_Node<value_type> *iterator(head);
      while (sizeOf < s.sizeOf) {
        chunk = new list_Node(*(chunk->back));
        sizeOf++;
        iterator->back = chunk;
        iterator = iterator->back;
      }
    }
    return *this;
  }

  const_reference top() const {
    if (empty()) throw std::out_of_range("Stack::top() - the Stack is empty");
    return head->get_elem();
  }

  bool empty() const { return (head) ? false : true; }

  size_type size() const { return sizeOf; }

  void push(const_reference value) {
    list_Node<value_type> *chunk = new list_Node<value_type>(value);
    chunk->back = head;
    head = chunk;
    ++sizeOf;
  }

  void pop() {
    if (head != nullptr) {
      list_Node<value_type> *buffNode = head;
      head = head->back;
      delete buffNode;
      --sizeOf;
    }
  }

  void swap(Stack &other) {
    if (this != &other) {
      size_type buff_size = sizeOf;
      list_Node<value_type> *buff_head = head;
      sizeOf = other.sizeOf;
      head = other.head;
      other.sizeOf = buff_size;
      other.head = buff_head;
    }
  }

  template <typename... Args>
  void emplace_front(Args... args) {
    s21::vector<value_type> itemVector = {args...};
    for (auto &it : itemVector) {
      push(it);
    }
  }
};

}  // namespace s21
#endif  // _CONTAINERS_SRC_S21_STACK_