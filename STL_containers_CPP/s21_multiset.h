#ifndef _CONTAINERS_SRC_S21_MULTISET_
#define _CONTAINERS_SRC_S21_MULTISET_

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include "s21_iterator.h"
#include "s21_set.h"

namespace s21 {

template <typename Key>
class Multiset : public set<Key> {
  using node = Node<Key>;
  using Sset = set<Key>;
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = SetIterator<Key>;
  using const_iterator = SetConstIterator<Key>;
  using size_type = size_t;

 public:
  Multiset() : Sset::set(){};

  Multiset(const Multiset<Key>& ms) : Sset::set(ms){};

  Multiset(std::initializer_list<key_type> const& items) : Sset::set() {
    for (auto it = items.begin(); it != items.end(); ++it) {
      insert(*it);
    }
  };

  Multiset(Multiset<Key>&& ms) : Sset::set(ms){};

  ~Multiset(){};

  std::pair<iterator, bool> insert(const value_type& value) {
    if (Sset::size_ >= Sset::max_size_) {
      return std::make_pair(iterator(nullptr), false);
    }
    Node<Key>* node = new Node<Key>(value);
    insert_node(node);
    std::pair<iterator, bool> result = std::make_pair(iterator(node), true);
    Sset::set_balance_for_all();
    return result;
  }

  void merge(Multiset& other) {
    iterator it = other.begin();
    while (it != other.end()) {
      insert(*it);
      ++it;
    }
    other.clear();
  }

  size_type count(const Key& val) {
    size_type i = Sset::size_;
    size_type result = 0;
    iterator itr1 = Sset::begin();
    while (--i != 0) {
      if (val == *itr1) {
        ++result;
      }
      ++itr1;
    }
    return result;
  }
  /* if not found - returns and iterator to the end_ */
  iterator find(const Key& key) {
    iterator itr1 = Sset::begin();
    while (itr1 != Sset::end()) {
      if (key == *itr1) {
        return itr1;
      }
      ++itr1;
    }
    return itr1;
  }

  bool contains(const Key& key) {
    return find(key) != Sset::end() ? true : false;
  }

  std::pair<iterator, iterator> equal_range(const Key& key) {
    iterator itr1 = Sset::begin();
    while (itr1 != Sset::end() && *itr1 != key) {
      ++itr1;
    }
    iterator itr2 = itr1;
    if (itr1 != Sset::end()) {
      while (itr2.get_node()->left != nullptr && *itr2 == *itr1) {
        ++itr2;
      }
      if (*itr2 != *itr1) {
        --itr2;
      }
    }
    std::pair<iterator, iterator> result = std::make_pair(itr1, itr2);
    return result;
  }
  /* if not found - returns last element */
  iterator lower_bound(const Key& key) {
    iterator itr1 = Sset::begin();
    while (itr1 != Sset::end()) {
      if (key <= *itr1) {
        return itr1;
      }
      ++itr1;
    }
    --itr1;
    return itr1;
  }
  /* if not found - returns last element */
  iterator upper_bound(const Key& key) {
    iterator itr1 = Sset::begin();
    while (itr1 != Sset::end()) {
      if (key < *itr1) {
        return itr1;
      }
      ++itr1;
    }
    --itr1;
    return itr1;
  }

 private:
  void insert_node(Node<Key>* node) {
    if (node == nullptr) {
      return;
    }
    node->balance = 0;
    ++(Sset::size_);

    if (Sset::root_ == nullptr) {
      Sset::end_ = new Node<Key>(0);
      node->right = Sset::end_;
      Sset::root_ = node;
      Sset::end_->parent = node;
    } else {
      Node<Key>* tmp = Sset::root_;
      while (tmp != Sset::end_ && tmp != nullptr) {
        if (node->value < tmp->value) {
          if (tmp->left == nullptr) {
            tmp->left = node;
            node->parent = tmp;
            break;
          } else {
            tmp = tmp->left;
          }
        } else if (node->value > tmp->value) {
          if (tmp->right == Sset::end_) {
            tmp->right = node;
            node->parent = tmp;
            Sset::end_->parent = node;
            node->right = Sset::end_;
            break;
          } else if (tmp->right == nullptr) {
            tmp->right = node;
            node->parent = tmp;
            break;
          } else {
            tmp = tmp->right;
          }
        } else {
          if (tmp->left != nullptr) {
            tmp->left->parent = node;
            node->left = tmp->left;
          }
          tmp->left = node;
          node->parent = tmp;
          break;
        }
      }
    }
  }
};

}  // namespace s21

#endif  // _CONTAINERS_SRC_S21_MULTISET_