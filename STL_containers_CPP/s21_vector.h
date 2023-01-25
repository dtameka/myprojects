#ifndef SRC_S21_VECTOR_H_
#define SRC_S21_VECTOR_H_

#include <climits>
#include <cmath>
#include <initializer_list>
#include <stdexcept>

namespace s21 {

template <class T>
class vector {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = size_t;

  vector() {}

  explicit vector(size_type n) {
    if (n > 0 && n <= max_size_) {
      capacity_ = size_ = n;
      data_ = new T[n]{0};
    }
  }

  vector(std::initializer_list<value_type> const& items)
      : size_(items.size()), capacity_(size_) {
    data_ = new T[size_];
    int i = 0;
    for (auto it = items.begin(); it != items.end(); ++it, ++i) data_[i] = *it;
  }

  vector(const vector& v) : size_(v.size_), capacity_(v.capacity_) {
    data_ = new T[size_];
    for (size_type i = 0; i < size_; ++i) data_[i] = v.data_[i];
  }

  vector(vector&& v) : size_(v.size_), capacity_(v.capacity_), data_(v.data_) {
    v.data_ = nullptr;
    v.size_ = 0;
    v.capacity_ = 0;
  }

  ~vector() { delete[] data_; }

  void operator=(vector&& v) {
    if (&v != this) {
      delete[] data_;
      size_ = v.size_;
      capacity_ = v.capacity_;
      data_ = v.data_;
      v.size_ = v.capacity_ = 0;
      v.data_ = nullptr;
    }
  }

  reference at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("Index out of range!");

    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }
  const_reference front() { return at(0); }
  const_reference back() { return at(size_ - 1); }
  iterator data() { return data_; }

  iterator begin() { return data_; }
  iterator end() { return data_ + size_; }

  bool empty() { return size_ == 0; }
  size_type size() { return size_; }
  size_type max_size() { return max_size_; }

  void reserve(size_type size) {
    if (size > 0 && size <= max_size_) {
      T* tmp_data = new T[size];
      for (size_type i = 0; i < size_; ++i) {
        tmp_data[i] = std::move(data_[i]);
      }
      capacity_ = size;
      delete[] data_;
      data_ = tmp_data;
    }
  }

  size_type capacity() { return capacity_; }

  void shrink_to_fit() {
    if (capacity_ && capacity_ > size_) {
      T* tmp_data = new T[size_];
      for (size_type i = 0; i < size_; ++i) {
        tmp_data[i] = data_[i];
      }
      delete[] data_;
      capacity_ = size_;
      data_ = tmp_data;
    }
  }

  void clear() {
    delete[] data_;
    size_ = capacity_ = 0;
    data_ = nullptr;
  }

  iterator insert(iterator pos, const_reference value) {
    const size_t diff = pos - begin();
    if (pos >= end() || pos < begin() || diff > size_) {
      throw std::out_of_range("Position out of array range!");
    }
    if (size_ == capacity_) {
      reallocate();
    }
    size_type count = size_++;
    for (; count > diff; --count) {
      data_[count] = data_[count - 1];
    }
    data_[count] = value;
    return begin() + diff;
  }

  void erase(iterator pos) {
    if (pos >= end() || pos < begin()) {
      throw std::out_of_range("Position out of array range!");
    }
    const size_t number_of_items = end() - data_ - 1;
    for (size_t i = pos - data_; i < number_of_items; ++i) {
      data_[i] = data_[i + 1];
    }
    --size_;
  }

  void push_back(const_reference value) {
    if (size_ == capacity_) {
      reallocate();
    }
    data_[size_++] = value;
  }

  void pop_back() { size_--; }

  void swap(vector& other) {
    if (&other != this) {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(data_, other.data_);
    }
  }

  bool operator==(const vector<T>& v) {
    const double Eps_ = 1e-6;
    bool result = true;
    if (size_ == v.size_) {
      for (size_type i = 0; i < size_; i++)
        if (fabs(data_[i] - v.data_[i]) > Eps_) result = false;
    } else {
      result = false;
    }
    return result;
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    if (pos >= end() || pos < begin()) {
      throw std::out_of_range("position is out of range");
    }

    int newpos = (pos - begin());

    if (size_ == capacity_ || size_ < capacity_) {
      reallocate();
    }
    iterator it = data_ + newpos;
    const size_t size = sizeof...(Args);
    value_type data[size] = {args...};
    for (size_t i = 0; i < size; i++) {
      insert(it++, data[i]);
    }
    return it;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    std::initializer_list<T> arguments = {args...};
    for (size_t it : arguments) {
      push_back(it);
    }
  }

 private:
  size_type size_ = 0;
  size_type max_size_ = LLONG_MAX / sizeof(value_type);
  size_type capacity_ = 0;
  T* data_ = nullptr;

  void reallocate() {
    if (size_ >= capacity_) {
      capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
      T* tmp_data = new T[capacity_]{};
      for (size_type i = 0; i < size_; ++i) {
        tmp_data[i] = data_[i];
      }
      delete[] data_;
      data_ = tmp_data;
    }
  }
};

}  // namespace s21

#endif  // SRC_S21_VECTOR_H_