#ifndef SRC_S21_ITERATOR_H_
#define SRC_S21_ITERATOR_H_

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include "s21_node.h"

namespace s21 {
template <class T>
class SetIterator {
 public:
  explicit SetIterator(Node<T> *node) : node(node) {}

  T &value() const { return node->value; }

  void operator++() { increase(); }
  void operator++(int) { increase(); }
  void operator--() { decrease(); }
  void operator--(int) { decrease(); }
  bool operator==(const SetIterator &other) { return node == other.node; }
  bool operator!=(const SetIterator &other) { return node != other.node; }
  T &operator*() const {
    if (node != nullptr && node->parent != nullptr &&
        node->parent->right == node && node->parent->value > node->value) {
      return node->parent->value;
    }
    if (node != nullptr) {
      return node->value;
    } else {
      throw std::out_of_range("out of range");
    }
  }

  int show_balance() const { return node->balance; }
  Node<T> *get_node() const { return node; }  // service func for tests

 protected:
  Node<T> *node;

  void increase() {
    if (node == nullptr) return;

    if (node->parent == nullptr && node->right->value < node->value) {
      node = node->right;
      return;
    }

    // Проверка на end, чтобы итератор не уходил в nullptr
    if (node->parent != nullptr && node->parent->right == node &&
        node->parent->value > node->value)
      return;
    if (node->parent != nullptr && node->value == node->parent->value) {
      node = node->parent;
      return;
    }
    if (node->right != nullptr) {
      node = node->right;
      while (node->left != nullptr) {
        node = node->left;
      }
    } else {
      if (node->parent != nullptr && node->parent->right == node) {
        while (node->parent != nullptr && node->parent->left != node) {
          node = node->parent;
        }
      }
      node = node->parent;
    }
  }

  void decrease() {
    if (node == nullptr) return;
    bool end = false;

    if (node->parent != nullptr && node->parent->right == node &&
        node->parent->value > node->value) {
      node = node->parent;
      end = true;
    }

    if (node->left != nullptr && (!end)) {
      node = node->left;
      if (node->parent->value == node->value) {
        return;
      }
      while (node->right != nullptr) {
        node = node->right;
      }
    } else if (!end) {
      if (node->parent != nullptr && node->parent->left == node) {
        while (node->parent != nullptr && node->parent->right != node) {
          node = node->parent;
        }
      }
      node = node->parent;
    }
  }
};

template <class T>
class SetConstIterator : public SetIterator<T> {
 public:
  explicit SetConstIterator(Node<T> *node) : SetIterator<T>(node) {}

  const T value() const { return SetIterator<T>::node->value; }

  bool operator==(const SetConstIterator &other) {
    return SetIterator<T>::node == other.node;
  }
  bool operator!=(const SetConstIterator &other) {
    return SetIterator<T>::node != other.node;
  }
};

template <typename T>
class node_iterator {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

 public:
  node_iterator() : elem_(nullptr){};
  node_iterator(const list_Node<value_type> &elem) : elem_(&elem){};
  node_iterator(const node_iterator<value_type> &elem) : elem_(elem.elem_){};
  node_iterator(list_Node<value_type> *elem) : elem_(elem){};
  ~node_iterator(){};

  void operator++() {
    if (elem_ != nullptr && elem_->fwd != nullptr) elem_ = elem_->fwd;
  }
  void operator--() {
    if (elem_ != nullptr && elem_->back != nullptr) elem_ = elem_->back;
  }

  node_iterator &operator++(int) {
    node_iterator<value_type> &buff(*this);
    if (elem_ != nullptr) elem_ = elem_->fwd;
    return buff;
  }

  node_iterator &operator--(int) {
    node_iterator<value_type> &buff(*this);
    if (elem_ != nullptr) elem_ = elem_->back;
    return buff;
  }

  node_iterator &operator-(const int &rv) {
    node_iterator<value_type> &buff(*this);
    for (int i = 0; i < rv; ++i) {
      if (buff.elem_ != nullptr && buff.elem_->back != nullptr)
        buff.elem_ = buff.elem_->back;
    }
    return buff;
  }

  node_iterator &operator+(const int &rv) {
    node_iterator<value_type> &buff(*this);
    for (int i = 0; i < rv; ++i) {
      if (buff.elem_ != nullptr && buff.elem_->fwd != nullptr)
        buff.elem_ = buff.elem_->fwd;
    }
    return buff;
  }

  bool operator==(const node_iterator other) const {
    return elem_ == other.elem;
  }
  bool operator!=(const node_iterator other) const {
    return elem_ != other.elem;
  }
  node_iterator &operator=(const node_iterator &other) {
    elem_ = other.elem_;
    return *this;
  }
  node_iterator &operator=(node_iterator &&other) {
    elem_ = other.elem_;
    other.elem_ = nullptr;
    return *this;
  }
  node_iterator &operator=(list_Node<value_type> *other) {
    elem_ = other;
    return *this;
  }
  list_Node<value_type> *get_elem() const { return elem_; }
  reference operator*() const {
    if (elem_ != nullptr) {
      return elem_->list_Node<value_type>::get_elem();
    } else {
      throw std::out_of_range("node_iterator::node is empty");
    }
  }

 protected:
  list_Node<value_type> *elem_;
};

template <class T>
class node_const_iterator : public node_iterator<T> {
 public:
  explicit node_const_iterator(Node<T> *node) : node_iterator<T>(node) {}
  explicit node_const_iterator(node_iterator<T> iter)
      : node_iterator<T>(iter.node_iterator<T>::get_elem()) {}

  bool operator==(const node_const_iterator &other) {
    return node_iterator<T>::node == other.node;
  }
  bool operator!=(const node_const_iterator &other) {
    return node_iterator<T>::node != other.node;
  }

  const T operator*() const {
    if (node_iterator<T>::elem_ != nullptr) {
      return node_iterator<T>::elem_->list_Node<T>::get_const_elem();
    } else {
      throw std::out_of_range("node_iterator::node is empty");
    }
  }
};

template <class Key, class T>
class MapIterator {
 public:
  explicit MapIterator(map_Node<Key, T> *node) : node(node) {}

  T &value() { return node->value; }
  const Key key() const { return node->key; }

  void operator++() { increase(); }
  void operator++(int) { increase(); }
  void operator--() { decrease(); }
  void operator--(int) { decrease(); }
  bool operator==(const MapIterator &other) { return node == other.node; }
  bool operator!=(const MapIterator &other) { return node != other.node; }
  T &operator*() const {
    if (node != nullptr && node->parent != nullptr &&
        node->parent->right == node && node->parent->key > node->key) {
      return node->parent->value;
    }
    if (node != nullptr) {
      return node->value;
    } else {
      throw std::out_of_range("out of range");
    }
  }
  int show_balance() const { return node->balance; }
  map_Node<Key, T> *get_node() const { return node; }

 protected:
  map_Node<Key, T> *node;

  void increase() {
    if (node == nullptr) return;

    if (node->parent == nullptr && node->right->key < node->key) {
      node = node->right;
      return;
    }

    // Проверка на end, чтобы итератор не уходил в nullptr
    if (node->parent != nullptr && node->parent->right == node &&
        node->parent->key > node->key)
      return;
    if (node->parent != nullptr && node->key == node->parent->key) {
      node = node->parent;
      return;
    }
    if (node->right != nullptr) {
      node = node->right;
      while (node->left != nullptr) {
        node = node->left;
      }
    } else {
      if (node->parent != nullptr && node->parent->right == node) {
        while (node->parent != nullptr && node->parent->left != node) {
          node = node->parent;
        }
      }
      node = node->parent;
    }
  }

  void decrease() {
    if (node == nullptr) return;
    bool end = false;

    if (node->parent != nullptr && node->parent->right == node &&
        node->parent->key > node->key) {
      node = node->parent;
      end = true;
    }

    if (node->left != nullptr && (!end)) {
      node = node->left;
      if (node->parent->key == node->key) {
        return;
      }
      while (node->right != nullptr) {
        node = node->right;
      }
    } else if (!end) {
      if (node->parent != nullptr && node->parent->left == node) {
        while (node->parent != nullptr && node->parent->right != node) {
          node = node->parent;
        }
      }
      node = node->parent;
    }
  }
};

template <class Key, class T>
class MapConstIterator : public MapIterator<Key, T> {
 public:
  explicit MapConstIterator(map_Node<Key, T> *node)
      : MapIterator<Key, T>(node) {}

  const Key key() const { return MapIterator<Key, T>::node->key; }
  const T value() const { return MapIterator<Key, T>::node->key; }

  bool operator==(const MapConstIterator &other) {
    return MapIterator<Key, T>::node == other.node;
  }
  bool operator!=(const MapConstIterator &other) {
    return MapIterator<Key, T>::node != other.node;
  }
};

}  // namespace s21
#endif  // SRC_S21_ITERATOR_H_
