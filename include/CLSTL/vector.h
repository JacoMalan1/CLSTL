#ifndef CLSTL_VECTOR_H
#define CLSTL_VECTOR_H

#include <cstdlib>
#include <iterator>
#include <stdexcept>

namespace clstl {
template <typename T, typename Allocator = std::allocator<T>> class vector;

template <typename T> class vector_iter {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using const_pointer = T *;
  using reference = T &;
  using const_reference = const T &;

  friend class vector<T>;
  vector_iter(const vector_iter<T> &other) = default;
  ~vector_iter() {}

  vector_iter<T> &operator=(const vector_iter<T> &other) = default;

  bool operator==(const vector_iter<T> &other) const {
    return this->m_Ptr == other.m_Ptr;
  }

  bool operator!=(const vector_iter<T> &other) const {
    return this->m_Ptr != other.m_Ptr;
  }

  vector_iter<T> &operator++() {
    ++this->m_Ptr;
    return *this;
  }

  vector_iter<T> &operator+=(const difference_type &movement) {
    this->m_Ptr += movement;
    return *this;
  }

  vector_iter<T> &operator-=(const difference_type &movement) {
    this->m_Ptr -= movement;
    return *this;
  }

  vector_iter<T> &operator--() {
    --this->m_Ptr;
    return *this;
  }

  vector_iter<T> operator++(int) {
    auto temp = *this;
    this->m_Ptr++;
    return temp;
  }

  vector_iter<T> operator--(int) {
    auto temp = *this;
    this->m_Ptr--;
    return temp;
  }

  reference operator*() { return *this->m_Ptr; }
  const_reference operator*() const { return *this->m_Ptr; }

  pointer operator->() { return this->m_Ptr; }
  const_pointer operator->() const { return this->m_Ptr; }

private:
  vector_iter(pointer data) : m_Ptr(data) {}

  T *m_Ptr;
};

template <typename T, typename Allocator> class vector {
public:
  using allocator_type = Allocator;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  typedef vector_iter<T> iterator;
  typedef vector_iter<const T> const_iterator;

  vector() : m_Data(nullptr), m_Capacity(0), m_Length(0), m_Alloc() {}
  vector(const vector &other)
      : m_Capacity(other.m_Length), m_Length(other.m_Length), m_Data(nullptr),
        m_Alloc() {
    this->m_Data = this->m_Alloc.allocate(this->m_Length);
    for (std::size_t i = 0; i < other.m_Length; i++) {
      this->m_Data[i] = other[i];
    }
  }

  vector(vector &&other)
      : m_Capacity(other.m_Capacity), m_Length(other.m_Length),
        m_Data(other.m_Data) {
    other.m_Data = nullptr;
  }

  vector &operator=(const vector &other);
  ~vector() {
    if (this->m_Data) {
      this->m_Alloc.deallocate(this->m_Data, this->m_Capacity);
    }
  }

  allocator_type get_allocator() const { return this->m_Alloc; }

  reference front() { return this->m_Data[0]; }
  const_reference front() const { return this->m_Data[0]; }

  reference back() { return this->m_Data[this->m_Length - 1]; }
  const_reference back() const { return this->m_Data[this->m_Length - 1]; }

  iterator begin() { return iterator(this->m_Data); }
  iterator end() { return iterator(this->m_Data + this->m_Length); }

  const_iterator begin() const { return iterator(this->m_Data); }
  const_iterator end() const { return iterator(this->m_Data + this->m_Length); }

  void push_back(const_reference val) {
    if (this->m_Length == this->m_Capacity) {
      this->grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);
    }

    this->m_Data[this->m_Length++] = val;
  }

  void push_back(T &&val) {
    if (this->m_Length == this->m_Capacity) {
      this->grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);
    }

    this->m_Data[this->m_Length++] = std::move(val);
  }

  void pop_back() { this->m_Data[this->m_Length--].~T(); }
  pointer data() { return this->m_Data; }
  const_pointer data() const { return this->m_Data; }

  void clear() {
    for (std::size_t i = 0; i < this->m_Length; i++) {
      this->pop_back();
    }
  }

  template <typename... Args> void emplace_back(Args &&...args) {
    if (this->m_Length == this->m_Capacity) {
      this->grow(this->m_Capacity == 0 ? 1 : this->m_Capacity * 2);
    }

#if __cplusplus < 202002L
    this->m_Alloc.construct(this->m_Data + this->m_Length++, args...);
#else
    std::construct_at(this->m_Data + this->m_Length++, args...);
#endif
  }

  std::size_t capacity() const { return this->m_Capacity; }
  std::size_t size() const { return this->m_Length; }
  bool empty() const { return this->size() == 0; }

  reference at(std::size_t idx) {
    if (idx >= this->m_Length) {
      throw std::out_of_range("Attempted to index vector out of range");
    }

    return this->m_Data[idx];
  }

  const_reference at(std::size_t idx) const {
    if (idx >= this->m_Length) {
      throw std::out_of_range("Attempted to index vector out of range");
    }

    return this->m_Data[idx];
  }

  void reserve(std::size_t cap) {
    if (this->m_Capacity < cap) {
      this->grow(cap);
    }
  }

  void resize(std::size_t count) {
    if (this->m_Length > count) {
      for (std::size_t i = 0; i < this->m_Length - count; i++) {
        this->pop_back();
      }
    } else if (this->m_Length < count) {
      this->reserve(count);
      for (std::size_t i = this->m_Length; i < count; i++) {
        this->m_Data[i] = T();
      }
      this->m_Length += count - this->m_Length;
    }
  }

  reference operator[](std::size_t idx) { return this->m_Data[idx]; }
  const_reference operator[](std::size_t idx) const {
    return this->m_Data[idx];
  }

private:
  void grow(std::size_t new_cap) {
    pointer new_data = this->m_Alloc.allocate(new_cap);

    if (this->m_Data) {
      for (std::size_t i = 0; i < this->m_Length; i++) {
        new_data[i] = this->m_Data[i];
      }
      this->m_Alloc.deallocate(this->m_Data, this->m_Capacity);
    }

    this->m_Capacity = new_cap;
    this->m_Data = new_data;
  }

  pointer m_Data;
  std::size_t m_Capacity;
  std::size_t m_Length;
  Allocator m_Alloc;
};
} // namespace clstl

#endif
