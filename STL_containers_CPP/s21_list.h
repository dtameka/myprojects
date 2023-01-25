#ifndef _CONTAINERS_SRC_S21_LIST_
#define _CONTAINERS_SRC_S21_LIST_

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include "s21_iterator.h"
#include "s21_node.h"
#include "s21_vector.h"

namespace s21 {

template <typename T>
class List {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using iterator = node_iterator<value_type>;
  using const_iterator = node_const_iterator<value_type>;

 public:
  List() : begin_(nullptr), end_(nullptr), sizeOf(0) {
    value_type zero;
    list_Node<value_type> *endNode = new list_Node<value_type>(zero);
    end_ = endNode;
  };
  List(size_type n, const_reference el) : List() {
    ++n;
    iterator iter = end();
    while (--n > 0) insert(iter, el);
  }
  List(std::initializer_list<value_type> const &items) : List() {
    auto el = items.begin();
    iterator iter = end();
    while (el != items.end()) {
      insert(iter, *el);
      ++el;
    }
  }
  List(const List &other) : List() {
    if (other.begin_ != nullptr) {
      iterator iter = end();
      iterator iter_2 = other.begin();
      while (iter_2.iterator::get_elem() != other.end().iterator::get_elem()) {
        insert(iter, *iter_2);
        ++iter_2;
      }
    }
  }
  List(List &&other) {
    begin_ = other.begin_;
    end_ = other.end_;
    sizeOf = other.sizeOf;
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.sizeOf = 0;
  }
  ~List() {
    clear();
    delete end_;
  }

  List &operator=(List &&other) {
    clear();
    delete end_;
    begin_ = other.begin_;
    end_ = other.end_;
    sizeOf = other.sizeOf;
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.sizeOf = 0;
    return *this;
  }

  List &operator=(const List &other) {
    if (this != &other) {
      clear();
      *this = List(other);
    }
    return *this;
  }

  void clear() {
    if (begin_ != nullptr) {
      list_Node<value_type> *next;
      while (begin_ != end_) {
        next = begin_->fwd;
        delete begin_;
        begin_ = next;
      }
      begin_ = nullptr;
      end_->back = nullptr;
    }
  }

  const_reference front() const {
    if (begin_ == nullptr)
      throw std::out_of_range(
          "s21::List::front - List is empty: can not get an elem");
    return begin_->get_elem();
  }

  const_reference back() const {
    if (begin_ == nullptr)
      throw std::out_of_range(
          "s21::List::back - List is empty: can not get an elem");
    return end_->back->get_elem();
  }

  bool empty() const { return begin_ == nullptr; }

  iterator begin() const { return iterator(begin_); }

  iterator end() const { return iterator(end_); }

  size_type size() const { return sizeOf; }

  size_type max_size() const { return size_type(-1); }

  iterator insert(iterator iter, const_reference value) {
    list_Node<value_type> *pos = iter.iterator::get_elem();
    list_Node<value_type> *chunck = new list_Node<value_type>(value);
    if (begin_ == nullptr) {
      begin_ = chunck;
      chunck->fwd = end_;
      end_->back = chunck;
    } else if (pos == begin_) {
      pos->back = chunck;
      chunck->fwd = pos;
      begin_ = chunck;
    } else {
      chunck->fwd = pos;
      chunck->back = pos->back;
      pos->back->fwd = chunck;
      pos->back = chunck;
    }
    ++sizeOf;
    iter = chunck;
    return iter;
  }

  void erase(iterator itr) {
    list_Node<value_type> *pos = itr.get_elem();
    if (pos == nullptr || pos == end_) return;
    if (pos == begin_) {
      pos->fwd->back = nullptr;
      begin_ = (begin_->fwd == end_) ? nullptr : begin_->fwd;
    } else {
      pos->back->fwd = pos->fwd;
      pos->fwd->back = pos->back;
    }
    --sizeOf;
    delete pos;
  }

  void push_back(const_reference value) { insert(end_, value); }

  void pop_back() { erase(end_->back); }

  void push_front(const_reference value) { insert(begin_, value); }

  void pop_front() { erase(begin_); }

  void swap(List &other) {
    if (this != &other) {
      List b(other);
      other = *this;
      *this = b;
    }
  }

  void merge(List &other) {
    if (other.begin_ == nullptr) return;
    if (begin_ == nullptr) {
      *this = List(other);
      return;
    }
    sizeOf += other.sizeOf;
    list_Node<value_type> *buff = begin_;
    list_Node<value_type> *buff_2 = other.begin_;
    list_Node<value_type> *newList;
    list_Node<value_type> *chunck;
    if (buff->get_elem() < buff_2->get_elem()) {
      newList = buff;
      buff = buff->fwd;
    } else {
      newList = buff_2;
      buff_2 = buff_2->fwd;
    }
    begin_ = newList;
    while (buff != end_ && buff_2 != other.end_) {
      if (buff->get_elem() < buff_2->get_elem()) {
        chunck = buff;
        buff = buff->fwd;
      } else {
        chunck = buff_2;
        buff_2 = buff_2->fwd;
      }
      chunck->back = newList;
      newList->fwd = chunck;
      newList = newList->fwd;
    }
    if (buff == end_) {
      delete buff;
      newList->fwd = buff_2;
      buff_2->back = newList;
      end_ = other.end_;
      other.end_ = nullptr;
    } else {
      newList->fwd = buff;
      buff->back = newList;
      other.end_->back = nullptr;
    }
    other.begin_ = nullptr;
    other.sizeOf = 0;
  }

  void splice(const iterator poshn, List &other) {
    if (other.begin_ == nullptr) return;
    list_Node<value_type> *pos = poshn.get_elem();
    pos->back = other.end_->back;
    other.end_->back->fwd = pos;
    begin_ = other.begin_;
    sizeOf += other.sizeOf;
    delete other.end_;
    other.end_ = nullptr;
    other.begin_ = nullptr;
    other.sizeOf = 0;
  }

  void reverse() {
    if (begin_ == nullptr) return;
    list_Node<value_type> *buff;
    list_Node<value_type> *swapper = begin_;
    while (begin_ != end_) {
      buff = begin_->fwd;
      begin_->fwd = begin_->back;
      if (buff != end_) begin_->back = buff;
      begin_ = buff;
    }
    swapper->fwd = end_;
    begin_ = end_->back;
    end_->back = swapper;
  }

  void unique() {
    if (begin_ == nullptr) return;
    list_Node<value_type> *buff = begin_;
    list_Node<value_type> *for_del;
    while (buff->fwd != end_) {
      if (buff->fwd->get_elem() == buff->get_elem() && buff->fwd != end_) {
        for_del = buff->fwd;
        buff->fwd = for_del->fwd;
        for_del->fwd->back = buff;
        sizeOf--;
        delete for_del;
      } else {
        buff = buff->fwd;
      }
    }
  }

  void sort() {
    if (begin_ == nullptr || end_->back == begin_) return;
    list_Node<value_type> *buff = begin_;
    list_Node<value_type> *chunck;
    list_Node<value_type> *optimizer = end_;
    while (optimizer->back != begin_) {
      while (buff != optimizer->back) {
        chunck = buff->fwd;
        if (chunck->get_elem() < buff->get_elem()) {
          if (buff == begin_) {
            begin_ = chunck;
          }
          chunck->back = buff->back;
          if (chunck != begin_) buff->back->fwd = chunck;
          buff->back = chunck;
          buff->fwd = chunck->fwd;
          chunck->fwd->back = buff;
          chunck->fwd = buff;
        } else {
          buff = chunck;
        }
      }
      buff = begin_;
      optimizer = optimizer->back;
    }
  }

  template <typename... Args>
  void emplace_front(Args... args) {
    const_iterator buff(begin());
    s21::vector<value_type> itemVector = {args...};
    for (auto &it : itemVector) {
      insert(buff, it);
    }
  }

  template <typename... Args>
  void emplace(const_iterator pos, Args... args) {
    s21::vector<value_type> itemVector = {args...};
    for (auto &it : itemVector) {
      insert(pos, it);
    }
  }

  template <typename... Args>
  void emplace_back(Args... args) {
    const_iterator buff(end());
    s21::vector<value_type> itemVector = {args...};
    for (auto &it : itemVector) {
      insert(buff, it);
    }
  }

 private:
  list_Node<value_type> *begin_;
  list_Node<value_type> *end_;
  size_type sizeOf;
};

}  // namespace s21

#endif  // _CONTAINERS_SRC_S21_LIST_