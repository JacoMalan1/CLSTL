#ifndef CLSTL_STRING_H
#define CLSTL_STRING_H

#include <cstring>
#include <limits>
#include <memory>
#include <stdexcept>

namespace clstl {
template <typename _Char_T = char, typename Allocator = std::allocator<_Char_T>>
class basic_string {
public:
  using value_type = _Char_T;
  using allocator_type = Allocator;
  using pointer = _Char_T *;
  using const_pointer = const _Char_T *;
  using reference = _Char_T &;
  using const_reference = const _Char_T &;

  basic_string() : m_Length(0), m_Capacity(0), m_Data(nullptr), m_Alloc() {}

  basic_string(const basic_string &other)
      : m_Length(other.m_Length), m_Capacity(other.m_Capacity),
        m_Alloc(other.m_Alloc) {
    if (this->m_Capacity > 0) {
      this->m_Data = this->m_Alloc.allocate(this->m_Capacity);
      std::memcpy(this->m_Data, other.m_Data,
                  this->m_Capacity * sizeof(_Char_T));
    }
  }

  basic_string &operator=(const basic_string &other) {
    this->m_Length = other.m_Length;
    this->m_Capacity = other.m_Capacity;
    this->m_Alloc = other.m_Alloc;
    if (this->m_Capacity > 0) {
      this->m_Data = this->m_Alloc.allocate(this->m_Capacity);
      std::memcpy(this->m_Data, other.m_Data,
                  this->m_Capacity * sizeof(_Char_T));
    }
  }

  basic_string &operator=(const char *other) {
    std::size_t len = std::strlen(other);
    this->m_Length = len;
    this->m_Capacity = len + 1;
    this->m_Data = this->m_Alloc.allocate(this->m_Capacity);
    std::memcpy(this->m_Data, other, sizeof(_Char_T) * (len + 1));
    this->m_Data[this->m_Length] = 0;
  }

  basic_string(const char *s) : m_Alloc() {
    std::size_t len = std::strlen(s);
    this->m_Length = len;
    this->m_Capacity = len + 1;
    this->m_Data = this->m_Alloc.allocate(this->m_Capacity);
    std::memcpy(this->m_Data, s, sizeof(_Char_T) * (len + 1));
    this->m_Data[this->m_Length] = 0;
  }

  ~basic_string() { this->m_Alloc.deallocate(this->m_Data, this->m_Capacity); }

  bool empty() const { return this->m_Length == 0; }
  std::size_t size() const { return this->m_Length; }
  std::size_t length() const { return this->m_Length; }
  std::size_t capacity() const { return this->m_Capacity; }

  pointer data() { return this->m_Data; }
  const_pointer data() const { return this->m_Data; }
  const_pointer c_str() const { return this->data(); }

  reference at(std::size_t idx) {
    if (idx >= this->m_Length) {
      throw std::out_of_range("Attempt to index string out of range.");
    }

    return this->operator[](idx);
  }

  const_reference at(std::size_t idx) const {
    if (idx >= this->m_Length) {
      throw std::out_of_range("Attempt to index string out of range.");
    }

    return this->operator[](idx);
  }

  reference operator[](std::size_t idx) { return this->m_Data[idx]; }
  const_reference operator[](std::size_t idx) const {
    return this->m_Data[idx];
  }

  std::size_t max_size() const {
    return std::numeric_limits<std::size_t>::max() - 1;
  }

  void clear() { this->m_Length = 0; }

  Allocator &get_allocator() { return this->m_Alloc; }
  const Allocator &get_allocator() const { return this->m_Alloc; }

  basic_string<_Char_T, Allocator> operator+(const basic_string &other) {
    basic_string<_Char_T, Allocator> result{};
    result.m_Alloc = this->m_Alloc;
    result.m_Capacity = this->m_Capacity + other.m_Capacity;
    if (result.m_Capacity > 0) {
      result.m_Capacity--;
    }
    result.m_Length = this->m_Length + other.m_Length;
    result.m_Data = this->m_Alloc.allocate(result.m_Capacity);

    if (this->m_Length > 0) {
      std::memcpy(&result.m_Data, this->m_Data,
                  this->m_Length * sizeof(_Char_T));
    }

    if (other.m_Length > 0) {
      std::memcpy(&result.m_Data, other.m_Data,
                  other.m_Length * sizeof(_Char_T));
    }

    if (result.m_Length > 0) {
      result.m_Data[result.m_Length] = 0;
    }

    return result;
  }

  bool operator==(const basic_string &other) const {
    if (this->m_Length != other.m_Length) {
      return false;
    }

    if (this->is_empty()) {
      return true;
    }

    for (std::size_t i = 0; i < this->m_Length; i++) {
      if (this->m_Data[i] != other.m_Data[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const basic_string &other) const {
    return !this->operator==(other);
  }

private:
  void grow(std::size_t new_cap) {
    _Char_T *new_data = this->m_Alloc.allocate(new_cap);
    if (this->m_Data) {
      std::memcpy(new_data, this->m_Data,
                  sizeof(_Char_T) * (this->m_Length + 1));
      this->m_Alloc.deallocate(this->m_Data, this->m_Length + 1);
    }
    this->m_Capacity = new_cap;
  }

  Allocator m_Alloc;
  _Char_T *m_Data;
  std::size_t m_Length;
  std::size_t m_Capacity;
};

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;

template <typename _Char_T, typename Allocator>
std::ostream &operator<<(std::ostream &stream,
                         const basic_string<_Char_T, Allocator> &str) {
  stream << str.c_str();
  return stream;
}
} // namespace clstl

#endif
