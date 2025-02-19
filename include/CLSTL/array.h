#ifndef CLSTL_ARRAY_H
#define CLSTL_ARRAY_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace clstl {
template <typename T> class array_iter {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using const_pointer = T *;
  using reference = T &;
  using const_reference = const T &;

  array_iter(const array_iter<T> &other) = default;
  ~array_iter() {}

  array_iter<T> &operator=(const array_iter<T> &other) = default;

  bool operator==(const array_iter<T> &other) const {
    return this->m_Ptr == other.m_Ptr;
  }

  bool operator!=(const array_iter<T> &other) const {
    return this->m_Ptr != other.m_Ptr;
  }

  array_iter<T> &operator++() {
    ++this->m_Ptr;
    return *this;
  }

  array_iter<T> &operator+=(const difference_type &movement) {
    this->m_Ptr += movement;
    return *this;
  }

  array_iter<T> &operator-=(const difference_type &movement) {
    this->m_Ptr -= movement;
    return *this;
  }

  array_iter<T> &operator--() {
    --this->m_Ptr;
    return *this;
  }

  array_iter<T> operator++(int) {
    auto temp = *this;
    this->m_Ptr++;
    return temp;
  }

  array_iter<T> operator--(int) {
    auto temp = *this;
    this->m_Ptr--;
    return temp;
  }

  reference operator*() { return *this->m_Ptr; }
  const_reference operator*() const { return *this->m_Ptr; }

  pointer operator->() { return this->m_Ptr; }
  const_pointer operator->() const { return this->m_Ptr; }

private:
  array_iter(pointer data) : m_Ptr(data) {}

  T *m_Ptr;
};

template <typename T, std::size_t N> class array {
public:
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = array_iter<T>;

  array() = default;
  array(std::initializer_list<T> l) {
    for (std::size_t i = 0; i < l.size(); i++) {
      this->m_Data[i] = *(l.begin() + i);
    }
  }

  iterator begin() { return array_iter(this->m_Data); }
  iterator end() { return array_iter(this->m_Data + N); }

  reference front() { return this->m_Data[0]; }
  const_reference front() const { return this->m_Data[0]; }

  reference back() { return this->m_Data[N - 1]; }
  const_reference back() const { return this->m_Data[N - 1]; }

  std::size_t size() const { return N; }

  pointer data() { return this->m_Data; }
  const_pointer data() const { return this->m_Data; }

  reference at(std::size_t idx) {
    if (idx >= N) {
      throw std::out_of_range("Attempted to index array out of bounds");
    }

    return this->m_Data[idx];
  }

  const_reference at(std::size_t idx) const {
    if (idx >= N) {
      throw std::out_of_range("Attempted to index array out of bounds");
    }

    return this->m_Data[idx];
  }

  reference operator[](std::size_t idx) { return this->m_Data[idx]; }
  const_reference operator[](std::size_t idx) const {
    return this->m_Data[idx];
  }

  bool empty() const { return N == 0; }

private:
  T m_Data[N];
};
} // namespace clstl

#endif
