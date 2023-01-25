#ifndef SRC_S21_SET_H_
#define SRC_S21_SET_H_

#include <climits>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

#include "s21_iterator.h"
#include "s21_node.h"
#include "s21_vector.h"

namespace s21 {

template <class Key>
class set {
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = SetIterator<Key>;
  using const_iterator = SetConstIterator<Key>;
  using size_type = size_t;

 public:
  set() {}

  set(std::initializer_list<value_type> const& items) {
    for (auto it = items.begin(); it != items.end(); ++it) {
      insert(*it);
    }
  }

  set(const set& s) { CopyTree(s.root_); }

  set(set&& s) {
    size_ = s.size_;
    root_ = s.root_;
    end_ = s.end_;
    s.size_ = 0;
    s.root_ = nullptr;
    s.end_ = nullptr;
  }

  ~set() { DeleteTree(root_); }

  void operator=(set&& s) {
    if (this != &s) {
      size_ = s.size_;
      CopyTree(s.root_);
    }
  }

  iterator begin() {
    Node<Key>* node = root_;
    while (node != nullptr && node->left != nullptr) {
      node = node->left;
    }
    return iterator(node);
  }

  iterator end() { return iterator(end_); }
  const_iterator cbegin() { return begin(); }
  const_iterator cend() { return end(); }

  bool empty() { return size_ == 0; }
  size_type size() { return size_; }
  size_type max_size() { return max_size_; }

  void clear() {
    DeleteTree(root_);
    size_ = 0;
    root_ = nullptr;
  }

  virtual std::pair<iterator, bool> insert(const value_type& value) {
    if (size_ >= max_size_) {
      return std::make_pair(iterator(nullptr), false);
    }
    Node<Key>* node = new Node<Key>(value);
    std::pair<iterator, bool> result = std::make_pair(iterator(node), true);
    ++size_;

    if (root_ == nullptr) {
      end_ = new Node<Key>(0);
      node->right = end_;
      node->right->parent = node;
      root_ = node;
    } else {
      Node<Key>* tmp = root_;
      while (tmp != end_ || tmp != nullptr) {
        if (value < tmp->value) {
          if (tmp->left == nullptr) {
            tmp->left = node;
            node->parent = tmp;
            break;
          } else {
            tmp = tmp->left;
          }
        } else if (value > tmp->value) {
          if (tmp->right == end_) {
            tmp->right = node;
            node->parent = tmp;
            end_->parent = node;
            node->right = end_;
            break;
          } else if (tmp->right == nullptr) {
            tmp->right = node;
            node->parent = tmp;
            break;
          } else {
            tmp = tmp->right;
          }
        } else {
          result = std::make_pair(iterator(nullptr), false);
          --size_;
          delete node;
          break;
        }
      }
    }
    Node<Key>* buff = result.first.get_node();
    balancing(buff);
    return result;
  }

  void erase(iterator pos) {
    if (pos == end()) {
      throw std::out_of_range("invalid pointer");
    }
    Node<Key>* node = pos.get_node();
    if ((node != nullptr && node != end_) ||
        (node->left == nullptr && node->right == nullptr)) {
      if (node->left == nullptr && node->right == nullptr) {
        if (node->parent != nullptr) {
          if (node->parent->left == node) {
            node->parent->left = nullptr;
          }
          if (node->parent->right == node) {
            node->parent->right = nullptr;
          }
        } else {
          root_ = nullptr;
          delete end_;
          end_ = nullptr;
        }
      } else if (node->left == nullptr) {
        if (node->parent != nullptr) {
          if (node->parent->left == node) {
            node->parent->left = node->right;
          }
          if (node->parent->right == node) {
            node->parent->right = node->right;
          }
          node->right->parent = node->parent;
        } else {
          node->right->parent = nullptr;
          root_ = node->right;
        }
      } else if (node->right == nullptr || node->right == end_) {
        if (node->parent != nullptr) {
          if (node->parent->left == node) {
            node->parent->left = node->left;
          }
          if (node->parent->right == node) {
            node->parent->right = node->left;
          }
          node->left->parent = node->parent;
          node->left->right = end_;
          node->left->right->parent = node->left;
          node->parent = nullptr;
        } else {
          node->left->parent = nullptr;
          node->left->right = node->right;
          node->left->right->parent = node->left;
          root_ = node->left;
        }
      } else {
        Node<Key>* tmp = node->right;
        while (tmp->left != nullptr) {
          tmp = tmp->left;
        }
        tmp->left = node->left;
        tmp->left->parent = tmp;
        if (node->parent != nullptr) {
          if (node->parent->left == node) {
            node->parent->left = node->right;
            node->right->parent = node->parent;
          }
          if (node->parent->right == node) {
            node->parent->right = node->right;
            node->right->parent = node->parent;
          }
        } else {
          node->right->parent = nullptr;
          root_ = node->right;
        }
      }
      --size_;
      delete node;
      set_balance_for_all();
    }
  }

  void swap(set& other) {
    if (this != &other) {
      std::swap(root_, other.root_);
      std::swap(end_, other.end_);
      std::swap(size_, other.size_);
    }
  }

  void merge(set& other) {
    for (iterator it = other.begin(); it != other.end();) {
      if (!contains(it.value())) {
        insert(it.value());
        other.erase(it);
        it = other.begin();
      } else {
        ++it;
      }
    }
  }

  bool contains(const Key& key) { return find(key) != end() ? true : false; }

  iterator find(const Key& key) {
    Node<Key>* node = root_;
    while (node != nullptr && node->value != key) {
      if (key < node->value) {
        node = node->left;
      } else {
        node = node->right;
      }
    }
    return iterator(node);
  }

  void balancing(Node<Key>* buff) {
    set_balance_for_all();
    while (buff != nullptr) {
      Node<Key>* new_buff = (buff == root_) ? nullptr : buff->parent;
      if (buff->balance > 1) {
        new_buff = buff->right;
        if (buff->right->balance < 0) r_rotate(buff->right);
        l_rotate(buff);
      } else if (buff->balance < -1) {
        new_buff = buff->left;
        if (buff->left->balance > 0) l_rotate(buff->left);
        r_rotate(buff);
      }
      buff = new_buff;
      set_balance_for_all();
    }
  }

  void set_balance_for_all() {
    iterator itr1 = begin();
    while (itr1.get_node() != end_) {
      set_balance(itr1.get_node());
      ++itr1;
    }
  }

  Node<Key>* get_root() { return root_; }

  template <class... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args&&... args) {
    std::vector<std::pair<iterator, bool>> result{};
    std::vector<value_type> itemVector = {args...};
    for (auto& items : itemVector) {
      result.push_back(insert(items));
    }
    return result;
  }

 protected:
  Node<Key>* end_ = nullptr;
  Node<Key>* root_ = nullptr;
  size_type size_ = 0;
  size_type max_size_ = LLONG_MAX / sizeof(value_type);

  void CopyTree(Node<Key>* node) {
    if (node == nullptr || node == end_) return;
    bool end = false;
    if (node->parent != nullptr && node->parent->right == node &&
        node->value < node->parent->value) {
      end = true;
    }
    if (!end) {
      insert(node->value);
      if (node->left != nullptr) {
        CopyTree(node->left);
      }
      if (node->right != nullptr && node->right != end_) {
        CopyTree(node->right);
      }
    }
  }

  void DeleteTree(Node<Key>* node) {
    if (node == nullptr) return;

    if (node->left != nullptr) {
      DeleteTree(node->left);
    }
    if (node->right != nullptr) {
      DeleteTree(node->right);
    }
    delete node;
  }

  void r_rotate(Node<Key>* chunck) {
    if (chunck != root_) {
      if (chunck->parent->left == chunck) chunck->parent->left = chunck->left;
      if (chunck->parent->right == chunck) chunck->parent->right = chunck->left;
    }
    chunck->left->parent = (chunck == root_) ? nullptr : chunck->parent;
    chunck->parent = chunck->left;
    if (chunck->left->right != nullptr) chunck->left->right->parent = chunck;
    Node<Key>* buff = chunck->left->right;
    chunck->left->right = chunck;
    chunck->left = buff;
    if (chunck == root_) root_ = chunck->parent;
  }

  void l_rotate(Node<Key>* chunck) {
    if (chunck != root_) {
      if (chunck->parent->left == chunck) chunck->parent->left = chunck->right;
      if (chunck->parent->right == chunck)
        chunck->parent->right = chunck->right;
    }
    chunck->right->parent = (chunck == root_) ? nullptr : chunck->parent;
    chunck->parent = chunck->right;
    if (chunck->right->left != nullptr) {
      chunck->right->left->parent = chunck;
    }
    Node<Key>* buff = chunck->right->left;
    chunck->right->left = chunck;
    chunck->right = buff;
    if (chunck == root_) root_ = chunck->parent;
  }

  void set_balance(Node<Key>* chunck) {
    chunck->balance = height(chunck->right) - height(chunck->left);
  }

  int height(Node<Key>* chunck) {
    if (chunck == nullptr || chunck == end_) return 0;
    int h_left = height(chunck->left);
    int h_right = height(chunck->right);
    if (h_left > h_right) {
      return h_left + 1;
    } else {
      return h_right + 1;
    }
  }
};

}  // namespace s21

#endif  // SRC_S21_SET_H_
